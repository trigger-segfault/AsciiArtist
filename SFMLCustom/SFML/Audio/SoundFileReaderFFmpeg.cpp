////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2017 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio/SoundFileReaderFFmpeg.hpp>
#include <SFML/System/MemoryInputStream.hpp>
#include <SFML/System/Err.hpp>
#include <algorithm>
#include <cctype>
#include <cassert>
#include <iostream>

namespace {
int read(void* opaque, uint8_t* buf, int buf_size) {
	sf::InputStream* stream = (sf::InputStream*)opaque;
	return (int)stream->read(buf, buf_size);
}

int64_t seek(void* opaque, int64_t offset, int whence) {
	sf::InputStream* stream = (sf::InputStream*)opaque;
	switch (whence) {
	case SEEK_SET:
		break;
	case SEEK_CUR:
		offset += stream->tell();
		break;
	case SEEK_END:
		offset = stream->getSize() - offset;
		break;
	case AVSEEK_SIZE:
		return (int64_t)stream->getSize();
	}
	return (int64_t)stream->seek(offset);
}
}

namespace sf {
namespace priv {
////////////////////////////////////////////////////////////
bool SoundFileReaderFFmpeg::check(InputStream& stream) {
	return true;
}


////////////////////////////////////////////////////////////
SoundFileReaderFFmpeg::SoundFileReaderFFmpeg() {

	m_formatContext = NULL;
	m_ioContext = NULL;
	m_outBuffer = NULL;
	m_outBufferSize = 0;
	m_codec = NULL;
	m_codecContext = NULL;
	m_ioContextBufferSize = 4096;
	m_ioContextBuffer = NULL;
	m_outSampleFormat = AV_SAMPLE_FMT_S16;
	m_outChannelLayout = AV_CH_FRONT_LEFT | AV_CH_FRONT_RIGHT;
	m_convertContext = NULL;
}


////////////////////////////////////////////////////////////
SoundFileReaderFFmpeg::~SoundFileReaderFFmpeg() {
	close();
}


////////////////////////////////////////////////////////////
void SoundFileReaderFFmpeg::close() {
	if (m_codecContext) {
		avcodec_close(m_codecContext);
		m_codecContext = NULL;
	}
	if (m_formatContext) {
		avformat_close_input(&m_formatContext);
	}
	if (m_ioContext) {
		av_freep(&m_ioContext->buffer);
		av_freep(&m_ioContext);
		m_ioContextBuffer = NULL;
	}
	if (m_outBuffer) {
		av_freep(&m_outBuffer);
	}
}


////////////////////////////////////////////////////////////
//bool SoundFileReaderFFmpeg::open(const std::string& url, Info& info) {
bool SoundFileReaderFFmpeg::open(InputStream& stream, Info& info) {

	av_register_all();
	avformat_network_init();
	m_formatContext = avformat_alloc_context();

	m_ioContextBuffer = (uint8_t*)av_malloc(m_ioContextBufferSize);
	if (!m_ioContextBuffer) {
		close();
		return false;
	}
	m_ioContext = avio_alloc_context(
		m_ioContextBuffer, m_ioContextBufferSize,
		0, &stream, &::read, NULL, &::seek
	);
	if (!m_ioContext) {
		close();
		return false;
	}
	m_formatContext->pb = m_ioContext;

	if (avformat_open_input(&m_formatContext, NULL, NULL, NULL) != 0) {
	//if (avformat_open_input(&m_formatContext, R"(C:\Users\Onii-chan\Music\iTunes\iTunes Media\Music\Ylvis\The Fox (What Does The Fox Say_) - Singl\02 The Fox (Instrumental).m4a)", NULL, NULL) != 0) {
		close();
		return false;
	}

	if (avformat_find_stream_info(m_formatContext, NULL) < 0) {
		close();
		return false;
	}

	m_audioStream = -1;
	for (int i = 0; i < m_formatContext->nb_streams; i++) {
		if (m_formatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
			m_audioStream = i;
			break;
		}
	}

	m_codecContext = m_formatContext->streams[m_audioStream]->codec;

	m_codec = avcodec_find_decoder(m_codecContext->codec_id);
	if (!m_codec) {
		close();
		return false;
	}

	if (avcodec_open2(m_codecContext, m_codec, NULL) < 0) {
		close();
		return false;
	}
	info.channelCount = m_codecContext->channels;
	info.sampleRate = m_codecContext->sample_rate;
	info.sampleCount = m_formatContext->duration * m_codecContext->sample_rate / AV_TIME_BASE * m_codecContext->channels;

	m_packet = (AVPacket*)av_malloc(sizeof(AVPacket));
	av_init_packet(m_packet);

	m_outBufferSize = av_samples_get_buffer_size(NULL, m_codecContext->channels, m_codecContext->frame_size, m_outSampleFormat, 1);
	m_outBuffer = (uint8_t*)av_malloc(192000 * m_codecContext->channels);

	m_frame = av_frame_alloc();

	m_convertContext = swr_alloc();
	m_convertContext = swr_alloc_set_opts(m_convertContext,
		m_outChannelLayout, m_outSampleFormat, m_codecContext->sample_rate,
		m_codecContext->channel_layout, m_codecContext->sample_fmt, m_codecContext->sample_rate,
		0, NULL
	);
	swr_init(m_convertContext);

	/*if (av_read_frame(m_formatContext, m_packet) < 0) {
		close();
		return false;
	}
	int gotFrame = 0;
	int ret = avcodec_decode_audio4(m_codecContext, m_frame, &gotFrame, m_packet);
	if (ret < 0) {
		close();
		return false;
	}*/
	return true;
}


////////////////////////////////////////////////////////////
void SoundFileReaderFFmpeg::seek(Uint64 sampleOffset) {
	int64_t seek_target = (int64_t)sampleOffset;

	AVRational av_time_base_q = { 1, AV_TIME_BASE };
	
	if (m_audioStream >= 0)
		seek_target = av_rescale_q((int64_t)sampleOffset * AV_TIME_BASE / m_codecContext->sample_rate / m_codecContext->channels, av_time_base_q, m_formatContext->streams[m_audioStream]->time_base);
	
	av_seek_frame(m_formatContext, m_audioStream, seek_target, AVSEEK_FLAG_BACKWARD | AVSEEK_FLAG_ANY);

	avcodec_flush_buffers(m_codecContext);
	m_packet->size = 0;
	av_free_packet(m_packet);
	swr_init(m_convertContext);
}


////////////////////////////////////////////////////////////
Uint64 SoundFileReaderFFmpeg::read(Int16* samples, Uint64 maxCount) {
	
	Uint64 count = 0;
	while (count < maxCount) {
		if (av_read_frame(m_formatContext, m_packet) < 0) {
			break;
		}

		if (m_packet->stream_index == m_audioStream) {
			int readCount = 0;
			while (m_packet->size > 0) {
				int gotFrame = 0;
				int result = avcodec_decode_audio4(m_codecContext, m_frame, &gotFrame, m_packet);
				if (result >= 0 && gotFrame) {
					int samplesToRead = static_cast<int>(maxCount - count) / m_codecContext->channels;
					m_packet->size -= result;
					m_packet->data += result;
					result = swr_convert(m_convertContext, (uint8_t**)&samples, samplesToRead, (const uint8_t**)m_frame->data, m_frame->nb_samples);
					if (result > 0) {
						count += result * m_codecContext->channels;
						samples += result * m_codecContext->channels;
					}
					else {
						m_packet->size = 0;
						m_packet->data = NULL;
					}
				}
				else {
					m_packet->size = 0;
					m_packet->data = NULL;
				}
			}
		}
		av_free_packet(m_packet);
	}

	return count;
}

} // namespace priv

} // namespace sf
