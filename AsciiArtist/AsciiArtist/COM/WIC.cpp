/*=================================================================|
|* File:				WIC.cpp									  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "WIC.h"
#include <PowerConsole/Helpers/StringHelpers.h>
#include <PowerConsole/Geometry/GMath.h>

using namespace PowerConsole::Geometry;
using namespace PowerConsole::Helpers;
using namespace AsciiArtist::COM;
//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region BitmapFrame
//========= CONSTRUCTORS =========
#pragma region Constructors

BitmapFrame::BitmapFrame()
 :	size(Point2I::ZERO), 
	pixels(nullptr) {}
BitmapFrame::~BitmapFrame() {
	if (pixels != nullptr) {
		delete[] pixels;
	}
}

#pragma endregion
//================================
#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma region WIC
//=========== READING ============
#pragma region Functions
//--------------------------------
#pragma region Decoder

WICBitmapDecoderSPtr WIC::createDecoderFromFile(const std::string& path) {
	return createDecoderFromStream(createStreamFromFile(path, true, false, false));
}
WICBitmapDecoderSPtr WIC::createDecoderFromResource(unsigned int id, unsigned int typeId) {
	return createDecoderFromStream(createStreamFromResource(id, "#" + std::to_string(typeId)));
}
WICBitmapDecoderSPtr WIC::createDecoderFromResource(unsigned int id, const std::string& type) {
	return createDecoderFromStream(createStreamFromResource(id, type));
}
WICBitmapEncoderSPtr WIC::createGifEncoderFromStream(COMStreamSPtr stream, Point2I size) {
	if (isCOMObjectNull(stream))
		return nullptr;

	// http://pixcl.com/WIC-and-Animated-GIF-Files.htm
	IWICImagingFactory* ipFactory = NULL;
	IWICBitmapEncoder* ipEncoder = NULL;
	IWICMetadataQueryWriter* ipEncoderMetadataWriter = NULL;
	PROPVARIANT propValue;
	WICBitmapEncoderSPtr encoder = nullptr;

	PropVariantInit(&propValue);

	#pragma region Init
	// Create the factory
	if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&ipFactory))))
		goto Return;

	//if (FAILED(ipFactory->CreateEncoder(GUID_ContainerFormatGif, &GUID_VendorMicrosoft, &ipEncoder)))
	if (FAILED(ipFactory->CreateEncoder(GUID_ContainerFormatGif, NULL, &ipEncoder)))
		goto ReleaseFactory;

	if (FAILED(ipEncoder->Initialize(stream->ipObject, WICBitmapEncoderNoCache)))
		goto ReleaseEncoder;
	#pragma endregion

	#pragma region Metadata
	if (FAILED(ipEncoder->GetMetadataQueryWriter(&ipEncoderMetadataWriter)))
		goto ReleaseEncoder;

	// We have to write the "/appext/application/NETSCAPE2.0" value into the global metadata.
	// Disabling this will prevent the gif from looping
	propValue.vt = VT_UI1 | VT_VECTOR;
	propValue.caub.cElems = 11;
	propValue.caub.pElems = new UCHAR[11];
	memcpy_s(propValue.caub.pElems, 11, "NETSCAPE2.0", 11);
	if (FAILED(ipEncoderMetadataWriter->SetMetadataByName(L"/appext/Application", &propValue))) {
		delete[] propValue.caub.pElems;
		goto ReleaseEncoderMetadataWriter;
	}
	delete[] propValue.caub.pElems;
	propValue.caub.pElems = NULL;

	// Set animated GIF format 
	propValue.vt = VT_UI1 | VT_VECTOR;
	propValue.caub.cElems = 5;
	propValue.caub.pElems = new UCHAR[5];
	propValue.caub.pElems[0] = 3; // must be > 1,
	propValue.caub.pElems[1] = 1; // defines animated GIF
	propValue.caub.pElems[2] = 0; // LSB 0 = infinite loop.
	propValue.caub.pElems[3] = 0; // MSB of iteration count value
	propValue.caub.pElems[4] = 0; // NULL == end of data
	if (FAILED(ipEncoderMetadataWriter->SetMetadataByName(L"/appext/Data", &propValue))) {
		delete[] propValue.caub.pElems;
		goto ReleaseEncoderMetadataWriter;
	}
	delete[] propValue.caub.pElems;
	propValue.caub.pElems = NULL;

	PropVariantClear(&propValue);

	// Global Width and Height are written successfully.
	propValue.vt = VT_UI2;
	propValue.uiVal = (USHORT)size.x;
	if (FAILED(ipEncoderMetadataWriter->SetMetadataByName(L"/logscrdesc/Width", &propValue)))
		goto ReleaseEncoderMetadataWriter;
	PropVariantClear(&propValue);
	propValue.vt = VT_UI2;
	propValue.uiVal = (USHORT)size.y;
	if (FAILED(ipEncoderMetadataWriter->SetMetadataByName(L"/logscrdesc/Height", &propValue)))
		goto ReleaseEncoderMetadataWriter;
	PropVariantClear(&propValue);

	#pragma endregion

	fullCOMRelease(ipEncoderMetadataWriter);
	fullCOMRelease(ipFactory);

	encoder = std::make_shared<WICBitmapEncoder>(ipEncoder);

	goto Return;

ReleaseEncoderMetadataWriter:
	fullCOMRelease(ipEncoderMetadataWriter);
ReleaseEncoder:
	fullCOMRelease(ipEncoder);
ReleaseFactory:
	fullCOMRelease(ipFactory);

Return:
	return encoder;
}
bool WIC::writeBitmapToGifEncoder(WICBitmapEncoderSPtr encoder, BitmapFrameSPtr bitmap) {
	if (isCOMObjectNull(encoder))
		return false;
	if (bitmap == nullptr)
		return false;

	// http://pixcl.com/WIC-and-Animated-GIF-Files.htm
	IWICImagingFactory* ipFactory = NULL;
	IWICBitmapSource* ipSource = NULL;
	IWICBitmap* ipBitmap = NULL;
	IWICBitmapLock* ipLock = NULL;
	IWICFormatConverter* ipConverter = NULL;
	IWICPalette *ipPalette = NULL;
	IWICBitmapFrameEncode* ipFrame = NULL;
	IWICMetadataQueryWriter* ipFrameMetadataWriter = NULL;
	WICInProcPointer data = NULL;
	WICPixelFormatGUID formatGUID = GUID_WICPixelFormat32bppBGRA;
	PROPVARIANT propValue;
	UINT bufferSize;
	Point2I size = bitmap->size;
	bool error = true;

	PropVariantInit(&propValue);

	#pragma region Frames and Frame Metadata

	// Create the factory
	if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&ipFactory))))
		goto Return;

	// Write the frames

	if (FAILED(ipFactory->CreateBitmap((UINT)size.x, (UINT)size.y, formatGUID, WICBitmapCacheOnDemand, &ipBitmap)))
		goto ReleaseFactory;

	//WICRect rect = { 0, 0, size.x, size.y };
	if (FAILED(ipBitmap->Lock(NULL, WICBitmapLockWrite, &ipLock)))
		goto ReleaseBitmap;
	if (FAILED(ipLock->GetDataPointer(&bufferSize, (WICInProcPointer*)&data))) {
		ipLock->Release();
		goto ReleaseBitmap;
	}

	memcpy_s(data, bufferSize, (LPVOID)bitmap->pixels, bufferSize);
	ipLock->Release();

	if (FAILED(ipFactory->CreateFormatConverter(&ipConverter)))
		goto ReleaseBitmap;

	ipBitmap->CopyPalette(ipPalette);
	if (ipPalette == NULL) {
		if (FAILED(ipFactory->CreatePalette(&ipPalette)))
			goto ReleaseConverter;
		if (FAILED(ipPalette->InitializeFromBitmap(ipBitmap, 256, TRUE)))
			goto ReleasePalette;
	}
	if (FAILED(ipConverter->Initialize(
		ipBitmap,
		GUID_WICPixelFormat8bppIndexed,
		WICBitmapDitherTypeNone,
		ipPalette,
		0.0f,
		WICBitmapPaletteTypeFixedWebPalette)))
		goto ReleasePalette;

	if (FAILED(ipConverter->QueryInterface(IID_PPV_ARGS(&ipSource))))
		goto ReleasePalette;

	if (FAILED(encoder->ipObject->CreateNewFrame(&ipFrame, NULL)))
		goto ReleaseSource;

	if (FAILED(ipFrame->Initialize(NULL)))
		goto ReleaseFrame;

	if (FAILED(ipFrame->WriteSource(ipSource, NULL)))
		goto ReleaseFrame;

	if (FAILED(ipFrame->GetMetadataQueryWriter(&ipFrameMetadataWriter)))
		goto ReleaseFrame;

	if (FAILED(ipFrame->Commit())) // Has to be HERE!
		goto ReleaseFrameMetadataWriter;

	// Set the Frame Width and Height. WIC writes both of these.
	propValue.vt = VT_UI2;
	propValue.uiVal = (UINT)size.x;
	if (FAILED(ipFrameMetadataWriter->SetMetadataByName(L"/imgdesc/Width", &propValue)))
		goto ReleaseFrameMetadataWriter;
	PropVariantClear(&propValue);

	propValue.vt = VT_UI2;
	propValue.uiVal = (UINT)size.y;
	if (FAILED(ipFrameMetadataWriter->SetMetadataByName(L"/imgdesc/Height", &propValue)))
		goto ReleaseFrameMetadataWriter;
	PropVariantClear(&propValue);

	// Other "/grctlext/*" values written here. Writing to the root
	// metadata region is not required.

	// In the frame creation loop, write the Delay values into the
	// correct frame metadata entry.
	// Frame speed in PowerConsole is stored in the current frame. Not in the next frame as delay

	propValue.vt = VT_UI2;
	propValue.uiVal = (WORD)(bitmap->frameSpeed / 10);
	if (FAILED(ipFrameMetadataWriter->SetMetadataByName(L"/grctlext/Delay", &propValue)))
		goto ReleaseFrameMetadataWriter;
	PropVariantClear(&propValue);

	error = false;

	#pragma endregion

ReleaseFrameMetadataWriter:
	fullCOMRelease(ipFrameMetadataWriter);
ReleaseFrame:
	fullCOMRelease(ipFrame);
ReleaseSource:
	fullCOMRelease(ipSource);
ReleasePalette:
	fullCOMRelease(ipPalette);
ReleaseConverter:
	fullCOMRelease(ipConverter);
ReleaseBitmap:
	fullCOMRelease(ipBitmap);
ReleaseFactory:
	fullCOMRelease(ipFactory);
Return:
	return !error;
}
bool WIC::writeFixesToGifFile(const std::string& path, int initialFrameSpeed) {
	// http://pixcl.com/WIC-and-Animated-GIF-Files.htm
	// Fix an issue with the gif file
	HANDLE hGifFile = CreateFileA(path.c_str(),
		GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hGifFile != NULL && hGifFile != INVALID_HANDLE_VALUE) {
		DWORD dwBytesRW;
		BYTE buffer[100];
		LPBYTE ptr = buffer;
		ReadFile(hGifFile, buffer, 100, &dwBytesRW, NULL);
		SetFilePointer(hGifFile, 0, NULL, FILE_BEGIN);
		// OK, now we can fix the bug. Find 21 F9, add 2 bytes
		//
		while ((*ptr != 0x21) || (*(ptr + 1) != 0xF9))  ptr++;
		// Should be at first grctlext block.
		ptr += 4; // should be the target Delay value
		*ptr = (BYTE)(initialFrameSpeed);
		WriteFile(hGifFile, buffer, 100, &dwBytesRW, NULL);
		CloseHandle(hGifFile);

		return true;
	}
	return false;
}

#pragma endregion
//--------------------------------
#pragma region Single Source

bool WIC::writeSourceToFile(const std::string& path, WICBitmapSourceSPtr source) {
	IWICImagingFactory* ipFactory = NULL;
	IWICBitmapEncoder* ipEncoder = NULL;
	IWICBitmapFrameEncode* ipFrame = NULL;
	WICPixelFormatGUID formatGUID = GUID_WICPixelFormat32bppBGRA;
	Point2I size = readImageSizeFromSource(source);
	COMStreamSPtr stream = nullptr;
	bool error = true;

	CoInitialize(nullptr);

	if (size == Point2I::ZERO)
		goto Return;

	// Create the factory
	if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&ipFactory))))
		goto Return;

	// Create the stream to save to
	stream = WIC::createStreamFromFile(path, false, true, true);
	if (isCOMObjectNull(stream))
		goto ReleaseFactory;

	// Create the encoder to use for saving
	if (FAILED(ipFactory->CreateEncoder(GUID_ContainerFormatPng, NULL, &ipEncoder)))
		goto ReleaseFactory;

	// Initialize the encoder to the stream
	if (FAILED(ipEncoder->Initialize(stream->ipObject, WICBitmapEncoderNoCache)))
		goto ReleaseEncoder;

	// Create the image to write to
	if (FAILED(ipEncoder->CreateNewFrame(&ipFrame, NULL)))
		goto ReleaseEncoder;

	// Initialize the image
	if (FAILED(ipFrame->Initialize(NULL)))
		goto ReleaseFrame;

	// Write the bitmap source to the frame
	if (FAILED(ipFrame->WriteSource(source->ipObject, NULL)))
		goto ReleaseFrame;

	// Save the file
	if (FAILED(ipFrame->Commit()))
		goto ReleaseFrame;
	if (SUCCEEDED(ipEncoder->Commit()))
		error = false;

ReleaseFrame:
	ipFrame->Release();
ReleaseEncoder:
	ipEncoder->Release();
ReleaseFactory:
	ipFactory->Release();
Return:
	CoUninitialize();
	return !error;
}
bool WIC::writeSourceToClipboard(WICBitmapSourceSPtr source) {
	return writeHBitmapToClipboard(createHBitmapFromSource(source));
}
WICBitmapSourceSPtr WIC::readSourceFromFile(const std::string& path) {
	return readSourceFromDecoder(createDecoderFromFile(path));
}
WICBitmapSourceSPtr WIC::readSourceFromResource(unsigned int id, unsigned int typeId) {
	return readSourceFromDecoder(createDecoderFromResource(id, typeId));
}
WICBitmapSourceSPtr WIC::readSourceFromResource(unsigned int id, const std::string& type) {
	return readSourceFromDecoder(createDecoderFromResource(id, type));
}
WICBitmapSourceSPtr WIC::readSourceFromClipboard() {
	return createSourceFromHBitmap(readHBitmapFromClipboard());
}
WICBitmapSourceSPtr WIC::readSourceFromDecoder(WICBitmapDecoderSPtr decoder, int frameIndex) {
	if (isCOMObjectNull(decoder))
		return nullptr;
	if (frameIndex < 0)
		return nullptr;

	IWICBitmapFrameDecode* ipFrame = NULL;
	WICBitmapSourceSPtr source = nullptr;

	// load the first frame (i.e., the image)
	if (SUCCEEDED(decoder->ipObject->GetFrame((UINT)frameIndex, &ipFrame)))
		source = std::make_shared<WICBitmapSource>(ipFrame);

	return source;
}

#pragma endregion
//--------------------------------
#pragma region Frame Count

int WIC::readFrameCountFromFile(const std::string& path) {
	return readFrameCountFromDecoder(createDecoderFromFile(path));
}
int WIC::readFrameCountFromResource(unsigned int id, unsigned int typeId) {
	return readFrameCountFromDecoder(createDecoderFromResource(id, typeId));
}
int WIC::readFrameCountFromResource(unsigned int id, const std::string& type) {
	return readFrameCountFromDecoder(createDecoderFromResource(id, type));
}
int WIC::readFrameCountFromDecoder(WICBitmapDecoderSPtr decoder) {
	if (isCOMObjectNull(decoder))
		return 0;

	UINT nFrameCount;

	// check for the presence of the first frame in the bitmap
	if (FAILED(decoder->ipObject->GetFrameCount(&nFrameCount)))
		return 0;

	return (int)nFrameCount;
}

#pragma endregion
//--------------------------------
#pragma region Image Size

Point2I WIC::readImageSizeFromFile(const std::string& path) {
	return readImageSizeFromDecoder(createDecoderFromFile(path));
}
Point2I WIC::readImageSizeFromResource(unsigned int id, unsigned int typeId) {
	return readImageSizeFromDecoder(createDecoderFromResource(id, typeId));
}
Point2I WIC::readImageSizeFromResource(unsigned int id, const std::string& type) {
	return readImageSizeFromDecoder(createDecoderFromResource(id, type));
}
Point2I WIC::readImageSizeFromDecoder(WICBitmapDecoderSPtr decoder) {
	return readImageSizeFromSource(readSourceFromDecoder(decoder));
}
Point2I WIC::readImageSizeFromSource(WICBitmapSourceSPtr source) {
	if (isCOMObjectNull(source))
		return Point2I::ZERO;

	Point2I size;
	if (FAILED(source->ipObject->GetSize((UINT*)&size.x, (UINT*)&size.y)))
		return Point2I::ZERO;
	return size;
}

#pragma endregion
//--------------------------------
#pragma region Single Bitmap

BitmapFrameSPtr WIC::createBitmap(Point2I size) {
	auto bmp = std::make_shared<BitmapFrame>();
	bmp->size = size;
	bmp->pixels = new ColorB[size.x * size.y];
	return bmp;
}
bool WIC::writeBitmapToFile(const std::string& path, BitmapFrameSPtr bitmap) {
	IWICImagingFactory* ipFactory = NULL;
	IWICBitmapEncoder* ipEncoder = NULL;
	IWICBitmapFrameEncode* ipFrame = NULL;
	WICPixelFormatGUID formatGUID = GUID_WICPixelFormat32bppBGRA;
	UINT stride = bitmap->size.x * 4;
	UINT bufferSize = stride * bitmap->size.y;
	COMStreamSPtr stream = nullptr;
	bool error = true;

	CoInitialize(nullptr);

	// Create the factory
	if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&ipFactory))))
		goto Return;

	// Create the stream to save to
	stream = WIC::createStreamFromFile(path, false, true, true);
	if (isCOMObjectNull(stream))
		goto ReleaseFactory;

	// Create the encoder to use for saving
	if (FAILED(ipFactory->CreateEncoder(GUID_ContainerFormatPng, NULL, &ipEncoder)))
		goto ReleaseFactory;

	// Initialize the encoder to the stream
	if (FAILED(ipEncoder->Initialize(stream->ipObject, WICBitmapEncoderNoCache)))
		goto ReleaseEncoder;

	// Create the image to write to
	if (FAILED(ipEncoder->CreateNewFrame(&ipFrame, NULL)))
		goto ReleaseEncoder;

	// Initialize the image
	if (FAILED(ipFrame->Initialize(NULL)))
		goto ReleaseFrame;

	// Set the size of the image
	if (FAILED(ipFrame->SetSize((UINT)bitmap->size.x, (UINT)bitmap->size.y)))
		goto ReleaseFrame;

	// Set the pixel format and make sure the format is correct afterwards
	if (FAILED(ipFrame->SetPixelFormat(&formatGUID)))
		goto ReleaseFrame;
	if (!IsEqualGUID(formatGUID, GUID_WICPixelFormat32bppBGRA))
		goto ReleaseFrame;

	// Write the bitmap to the frame
	if (FAILED(ipFrame->WritePixels((UINT)bitmap->size.y, stride, bufferSize, (BYTE*)bitmap->pixels)))
		goto ReleaseFrame;

	// Save the file
	if (FAILED(ipFrame->Commit()))
		goto ReleaseFrame;
	if (SUCCEEDED(ipEncoder->Commit()))
		error = false;

ReleaseFrame:
	ipFrame->Release();
ReleaseEncoder:
	ipEncoder->Release();
ReleaseFactory:
	ipFactory->Release();
Return:
	CoUninitialize();
	return !error;
}
bool WIC::writeBitmapToClipboard(BitmapFrameSPtr bitmap) {
	return writeSourceToClipboard(writeBitmapToSource(bitmap));
}
WICBitmapSourceSPtr WIC::writeBitmapToSource(BitmapFrameSPtr bitmap) {
	IWICImagingFactory* ipFactory = NULL;
	IWICBitmap* ipBitmap = NULL;
	IWICBitmapLock* ipLock = NULL;
	WICInProcPointer data = NULL;
	WICPixelFormatGUID formatGUID = GUID_WICPixelFormat32bppBGRA;
	UINT bufferSize;
	WICBitmapSourceSPtr source = nullptr;

	CoInitialize(NULL);

	// Create the factory
	if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&ipFactory))))
		goto Return;

	if (FAILED(ipFactory->CreateBitmap((UINT)bitmap->size.x, (UINT)bitmap->size.y, formatGUID, WICBitmapCacheOnDemand, &ipBitmap)))
		goto ReleaseFactory;

	if (FAILED(ipBitmap->Lock(NULL, WICBitmapLockWrite, &ipLock)))
		goto ReleaseBitmap;
	if (FAILED(ipLock->GetDataPointer(&bufferSize, &data))) {
		ipLock->Release();
		goto ReleaseBitmap;
	}

	memcpy_s(data, bufferSize, (LPVOID)bitmap->pixels, bufferSize);
	ipLock->Release();

	source = std::make_shared<WICBitmapSource>(ipBitmap);
	goto ReleaseFactory;

ReleaseBitmap:
	ipBitmap->Release();
ReleaseFactory:
	ipFactory->Release();
Return:
	CoUninitialize();
	return source;
}
BitmapFrameSPtr WIC::readBitmapFromFile(const std::string& path, int frameIndex) {
	return readBitmapFromDecoder(createDecoderFromFile(path), frameIndex);
}
BitmapFrameSPtr WIC::readBitmapFromResource(unsigned int id, unsigned int typeId, int frameIndex) {
	return readBitmapFromDecoder(createDecoderFromResource(id, typeId), frameIndex);
}
BitmapFrameSPtr WIC::readBitmapFromResource(unsigned int id, const std::string& type, int frameIndex) {
	return readBitmapFromDecoder(createDecoderFromResource(id, type), frameIndex);
}
BitmapFrameSPtr WIC::readBitmapFromDecoder(WICBitmapDecoderSPtr decoder, int frameIndex) {
	return readBitmapFromSource(readSourceFromDecoder(decoder, frameIndex));
}
BitmapFrameSPtr WIC::readBitmapFromSource(WICBitmapSourceSPtr source) {
	WICBitmapSourceSPtr convertedSource = nullptr;
	BitmapFrameSPtr bmp = nullptr;

	// Convert the source format to something we can read
	convertedSource = convertSourceFormat(source);
	if (isCOMObjectNull(convertedSource))
		return nullptr;

	bmp = std::make_shared<BitmapFrame>();

	// Read the size of the image
	if (FAILED(convertedSource->ipObject->GetSize((UINT*)&bmp->size.x, (UINT*)&bmp->size.y)))
		return nullptr;

	// Create the pixel array
	bmp->pixels = new ColorB[bmp->size.x * bmp->size.y];

	// Copy the pixels to the bitmap frame
	WICRect rect = { 0, 0, bmp->size.x, bmp->size.y };
	UINT stride = bmp->size.x * 4;
	if (FAILED(convertedSource->ipObject->CopyPixels(&rect, stride, stride * bmp->size.y, (BYTE*)bmp->pixels)))
		return nullptr;

	return bmp;
}


#pragma endregion
//--------------------------------
#pragma region Multiple Bitmaps

BitmapFrameCollectionSPtr WIC::createBitmapCollection() {
	return std::make_shared<BitmapFrameCollection>();
}
bool WIC::writeBitmapCollectionToFile(const std::string& path, BitmapFrameCollectionSPtr bitmaps) {
	// http://pixcl.com/WIC-and-Animated-GIF-Files.htm

	IWICImagingFactory* ipFactory = NULL;
	IWICBitmapSource* ipSource = NULL;
	IWICBitmap* ipBitmap = NULL;
	IWICBitmapLock* ipLock = NULL;
	IWICFormatConverter* ipConverter = NULL;
	IWICPalette *ipPalette = NULL;
	IWICStream* ipStream;
	IWICBitmapEncoder* ipEncoder = NULL;
	IWICBitmapFrameEncode* ipFrame = NULL;
	IWICMetadataQueryWriter* ipEncoderMetadataWriter = NULL;
	IWICMetadataQueryWriter* ipFrameMetadataWriter = NULL;
	WICInProcPointer data = NULL;
	WICPixelFormatGUID formatGUID = GUID_WICPixelFormat32bppBGRA;
	PROPVARIANT propValue;
	UINT bufferSize;
	Point2I size = bitmaps->frames[0]->size;
	for (int i = 1; i < (int)bitmaps->frames.size(); i++)
		size = GMath::max(size, bitmaps->frames[0]->size);
	bool error = true;

	CoInitialize(NULL);
	PropVariantInit(&propValue);

	#pragma region Init
	// Create the factory
	if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&ipFactory))))
		goto Return;

	//if (FAILED(ipFactory->CreateEncoder(GUID_ContainerFormatGif, &GUID_VendorMicrosoft, &ipEncoder)))
	if (FAILED(ipFactory->CreateEncoder(GUID_ContainerFormatGif, NULL, &ipEncoder)))
		goto ReleaseFactory;

	if (FAILED(ipFactory->CreateStream(&ipStream)))
		goto ReleaseEncoder;

	if (FAILED(ipStream->InitializeFromFilename(str_to_wstr(path).c_str(), GENERIC_WRITE)))
		goto ReleaseStream;
	#pragma endregion

	#pragma region Metadata
	if (FAILED(ipEncoder->Initialize(ipStream, WICBitmapEncoderNoCache)))
		goto ReleaseStream;

	if (FAILED(ipEncoder->GetMetadataQueryWriter(&ipEncoderMetadataWriter)))
		goto ReleaseStream;

	// We have to write the "/appext/application/NETSCAPE2.0" value into the global metadata.
	// Disabling this will prevent the gif from looping
	propValue.vt = VT_UI1 | VT_VECTOR;
	propValue.caub.cElems = 11;
	propValue.caub.pElems = new UCHAR[11];
	memcpy_s(propValue.caub.pElems, 11, "NETSCAPE2.0", 11);
	if (FAILED(ipEncoderMetadataWriter->SetMetadataByName(L"/appext/Application", &propValue))) {
		delete[] propValue.caub.pElems;
		goto ReleaseEncoderMetadataWriter;
	}
	delete[] propValue.caub.pElems;
	propValue.caub.pElems = NULL;

	// Set animated GIF format 
	propValue.vt = VT_UI1 | VT_VECTOR;
	propValue.caub.cElems = 5;
	propValue.caub.pElems = new UCHAR[5];
	propValue.caub.pElems[0] = 3; // must be > 1,
	propValue.caub.pElems[1] = 1; // defines animated GIF
	propValue.caub.pElems[2] = 0; // LSB 0 = infinite loop.
	propValue.caub.pElems[3] = 0; // MSB of iteration count value
	propValue.caub.pElems[4] = 0; // NULL == end of data
	if (FAILED(ipEncoderMetadataWriter->SetMetadataByName(L"/appext/Data", &propValue))) {
		delete[] propValue.caub.pElems;
		goto ReleaseEncoderMetadataWriter;
	}
	delete[] propValue.caub.pElems;
	propValue.caub.pElems = NULL;

	PropVariantClear(&propValue);

	// Global Width and Height are written successfully.
	propValue.vt = VT_UI2;
	propValue.uiVal = (USHORT)size.x;
	if (FAILED(ipEncoderMetadataWriter->SetMetadataByName(L"/logscrdesc/Width", &propValue)))
		goto ReleaseEncoderMetadataWriter;
	PropVariantClear(&propValue);
	propValue.vt = VT_UI2;
	propValue.uiVal = (USHORT)size.y;
	if (FAILED(ipEncoderMetadataWriter->SetMetadataByName(L"/logscrdesc/Height", &propValue)))
		goto ReleaseEncoderMetadataWriter;
	PropVariantClear(&propValue);

	#pragma endregion

	#pragma region Frames and Frame Metadata

	// Write the frames
	for (int frameIndex = 0; frameIndex < (int)bitmaps->frames.size(); frameIndex++) {
		size = bitmaps->frames[frameIndex]->size;

		if (FAILED(ipFactory->CreateBitmap((UINT)size.x, (UINT)size.y, formatGUID, WICBitmapCacheOnDemand, &ipBitmap)))
			goto ReleaseEncoderMetadataWriter;

		WICRect rect = { 0, 0, size.x, size.y };
		if (FAILED(ipBitmap->Lock(NULL, WICBitmapLockWrite, &ipLock)))
			goto ReleaseBitmap;
		if (FAILED(ipLock->GetDataPointer(&bufferSize, (WICInProcPointer*)&data))) {
			ipLock->Release();
			goto ReleaseBitmap;
		}

		memcpy_s(data, bufferSize, (LPVOID)bitmaps->frames[frameIndex]->pixels, bufferSize);
		ipLock->Release();

		if (FAILED(ipFactory->CreateFormatConverter(&ipConverter)))
			goto ReleaseBitmap;

		ipBitmap->CopyPalette(ipPalette);
		if (ipPalette == NULL) {
			if (FAILED(ipFactory->CreatePalette(&ipPalette)))
				goto ReleaseConverter;
			if (FAILED(ipPalette->InitializeFromBitmap(ipBitmap, 256, TRUE)))
				goto ReleasePalette;
		}
		if (FAILED(ipConverter->Initialize(
			ipBitmap,
			GUID_WICPixelFormat8bppIndexed,
			WICBitmapDitherTypeNone,
			ipPalette,
			0.0f,
			WICBitmapPaletteTypeFixedWebPalette)))
			goto ReleasePalette;

		if (FAILED(ipConverter->QueryInterface(IID_PPV_ARGS(&ipSource))))
			goto ReleasePalette;

		if (FAILED(ipEncoder->CreateNewFrame(&ipFrame, NULL)))
			goto ReleaseSource;

		if (FAILED(ipFrame->Initialize(NULL)))
			goto ReleaseFrame;

		if (FAILED(ipFrame->WriteSource(ipSource, NULL)))
			goto ReleaseFrame;

		if (FAILED(ipFrame->GetMetadataQueryWriter(&ipFrameMetadataWriter)))
			goto ReleaseFrame;

		if (FAILED(ipFrame->Commit())) // Has to be HERE!
			goto ReleaseFrameMetadataWriter;

		// Set the Frame Width and Height. WIC writes both of these.
		propValue.vt = VT_UI2;
		propValue.uiVal = (UINT)size.x;
		if (FAILED(ipFrameMetadataWriter->SetMetadataByName(L"/imgdesc/Width", &propValue)))
			goto ReleaseFrameMetadataWriter;
		PropVariantClear(&propValue);

		propValue.vt = VT_UI2;
		propValue.uiVal = (UINT)size.y;
		if (FAILED(ipFrameMetadataWriter->SetMetadataByName(L"/imgdesc/Height", &propValue)))
			goto ReleaseFrameMetadataWriter;
		PropVariantClear(&propValue);

		// Other "/grctlext/*" values written here. Writing to the root
		// metadata region is not required.

		// In the frame creation loop, write the Delay values into the
		// correct frame metadata entry.
		// Frame speed in PowerConsole is stored in the current frame. Not in the next frame as delay

		propValue.vt = VT_UI2;
		propValue.uiVal = (WORD)(bitmaps->frames[(frameIndex + 1) % (int)bitmaps->frames.size()]->frameSpeed / 10);
		if (FAILED(ipFrameMetadataWriter->SetMetadataByName(L"/grctlext/Delay", &propValue)))
			goto ReleaseFrameMetadataWriter;
		PropVariantClear(&propValue);

		fullCOMRelease(ipFrameMetadataWriter);
		fullCOMRelease(ipFrame);
		fullCOMRelease(ipSource);
		fullCOMRelease(ipPalette);
		fullCOMRelease(ipConverter);
		fullCOMRelease(ipBitmap);
	}

	#pragma endregion

	#pragma region Fix Gif

	fullCOMRelease(ipEncoderMetadataWriter);
	fullCOMRelease(ipStream);
	fullCOMRelease(ipEncoder);
	fullCOMRelease(ipFactory);

	// Fix an issue with the gif file
	HANDLE hGifFile = CreateFileA(path.c_str(),
		GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hGifFile != NULL && hGifFile != INVALID_HANDLE_VALUE) {
		DWORD dwBytesRW;
		BYTE buffer[100];
		LPBYTE ptr = buffer;
		ReadFile(hGifFile, buffer, 100, &dwBytesRW, NULL);
		SetFilePointer(hGifFile, 0, NULL, FILE_BEGIN);
		// OK, now we can fix the bug. Find 21 F9, add 2 bytes
		//
		while ((*ptr != 0x21) || (*(ptr + 1) != 0xF9))  ptr++;
		// Should be at first grctlext block.
		ptr += 4; // should be the target Delay value
		*ptr = (BYTE)(bitmaps->frames[0]->frameSpeed / 10);
		WriteFile(hGifFile, buffer, 100, &dwBytesRW, NULL);
		CloseHandle(hGifFile);
	}
	else {
		goto Return;
	}

	error = false;
	goto Return;

	#pragma endregion

ReleaseFrameMetadataWriter:
	fullCOMRelease(ipFrameMetadataWriter);
ReleaseFrame:
	fullCOMRelease(ipFrame);
ReleaseSource:
	fullCOMRelease(ipSource);
ReleasePalette:
	fullCOMRelease(ipPalette);
ReleaseConverter:
	fullCOMRelease(ipConverter);
ReleaseBitmap:
	fullCOMRelease(ipBitmap);
ReleaseEncoderMetadataWriter:
	fullCOMRelease(ipEncoderMetadataWriter);
ReleaseStream:
	fullCOMRelease(ipStream);
ReleaseEncoder:
	fullCOMRelease(ipEncoder);
ReleaseFactory:
	fullCOMRelease(ipFactory);

Return:
	CoUninitialize();
	return !error;
}
BitmapFrameCollectionSPtr WIC::readBitmapCollectionFromFile(const std::string& path) {
	return readBitmapCollectionFromDecoder(createDecoderFromFile(path));
}
BitmapFrameCollectionSPtr WIC::readBitmapCollectionFromResource(unsigned int id, unsigned int typeId) {
	return readBitmapCollectionFromDecoder(createDecoderFromResource(id, typeId));
}
BitmapFrameCollectionSPtr WIC::readBitmapCollectionFromResource(unsigned int id, const std::string& type) {
	return readBitmapCollectionFromDecoder(createDecoderFromResource(id, type));
}
BitmapFrameCollectionSPtr WIC::readBitmapCollectionFromDecoder(WICBitmapDecoderSPtr decoder) {
	BitmapFrameCollectionSPtr bitmaps = nullptr;
	UINT nFrameCount;
	if (isCOMObjectNull(decoder))
		return nullptr;

	// check for the presence of the first frame in the bitmap
	if (FAILED(decoder->ipObject->GetFrameCount(&nFrameCount)) || nFrameCount == 0)
		goto Return;

	bitmaps = std::make_shared<BitmapFrameCollection>();

	for (int i = 0; i < (int)nFrameCount; i++) {
		BitmapFrameSPtr bmp = readBitmapFromDecoder(decoder, i);
		if (bmp == nullptr)
			return nullptr;
		bitmaps->frames.push_back(bmp);
	}

Return:
	return bitmaps;
}

#pragma endregion
//--------------------------------
#pragma region Modification

WICBitmapSourceSPtr WIC::scaleSource(WICBitmapSourceSPtr inputSource, Point2F scale, BitmapScaleModes mode) {
	if (isCOMObjectNull(inputSource))
		return nullptr;

	IWICImagingFactory* ipFactory = NULL;
	IWICBitmapScaler* ipScaler = NULL;
	WICBitmapSourceSPtr outputSource = nullptr;
	Point2I size = readImageSizeFromSource(inputSource);
	Point2I newSize = (Point2I)(scale * (Point2F)size);
	if (!(newSize >= Point2I::ONE))
		goto Return;

	// Create the factory
	if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&ipFactory))))
		goto Return;

	// Create the scaler
	if (FAILED(ipFactory->CreateBitmapScaler(&ipScaler)))
		goto ReleaseFactory;

	// Scale the bitmap
	if (SUCCEEDED(ipScaler->Initialize(inputSource->ipObject, (UINT)newSize.x, (UINT)newSize.y, (WICBitmapInterpolationMode)mode))) {
		outputSource = std::make_shared<WICBitmapSource>(ipScaler);
		goto ReleaseFactory;
	}

	ipScaler->Release();
ReleaseFactory:
	ipFactory->Release();
Return:
	return outputSource;
}

#pragma endregion
//--------------------------------
#pragma endregion
//=========== HELPERS ============
#pragma region Helpers

COMStreamSPtr WIC::createStreamFromFile(const std::string& path, bool readAccess, bool writeAccess, bool create) {
	IStream* ipStream = NULL;
	COMStreamSPtr stream = nullptr;
	CoInitialize(NULL);

	// This may fail. If that's the case the stream will remain null
	if (SUCCEEDED(SHCreateStreamOnFileA(path.c_str(),
		(readAccess ? STGM_READ : 0) | (writeAccess ? STGM_WRITE : 0) | (create ? STGM_CREATE : 0),
		&ipStream)))
		stream = std::make_shared<COMStream>(ipStream);

	CoUninitialize();
	return stream;
}
COMStreamSPtr WIC::createStreamFromResource(unsigned int id, unsigned int typeId) {
	return createStreamFromResource(id, "#" + std::to_string(typeId));
}
COMStreamSPtr WIC::createStreamFromResource(unsigned int id, const std::string& type) {
	IStream* ipStream = NULL;
	COMStreamSPtr stream = nullptr;
	CoInitialize(NULL);

	// Find the resource
	HRSRC hrsrc = FindResourceA(NULL, ("#" + std::to_string(id)).c_str(), type.c_str());
	if (hrsrc == NULL)
		goto Return;

	// Load the resource
	DWORD dwResourceSize = SizeofResource(NULL, hrsrc);
	HGLOBAL hglbImage = LoadResource(NULL, hrsrc);
	if (hglbImage == NULL)
		goto Return;

	// Lock the resource, getting a pointer to its data
	LPVOID pvSourceResourceData = LockResource(hglbImage);
	if (pvSourceResourceData == NULL)
		goto Return;

	// Allocate memory to hold the resource data
	HGLOBAL hgblResourceData = GlobalAlloc(GMEM_MOVEABLE, dwResourceSize);
	if (hgblResourceData == NULL)
		goto Return;

	// Get a pointer to the allocated memory
	LPVOID pvResourceData = GlobalLock(hgblResourceData);
	if (pvResourceData == NULL)
		goto FreeData;

	// Copy the data from the resource to the new memory block
	CopyMemory(pvResourceData, pvSourceResourceData, dwResourceSize);
	GlobalUnlock(hgblResourceData);

	// Create a stream on the HGLOBAL containing the data
	if (SUCCEEDED(CreateStreamOnHGlobal(hgblResourceData, TRUE, &ipStream))) {
		stream = std::make_shared<COMStream>(ipStream);
		goto Return;
	}

FreeData:
	// Couldn't create stream; free the memory
	GlobalFree(hgblResourceData);
Return:
	// No need to unlock or free the resource
	CoUninitialize();
	return stream;
}
WICBitmapDecoderSPtr WIC::createDecoderFromStream(COMStreamSPtr stream) {
	if (isCOMObjectNull(stream))
		return nullptr;

	IWICImagingFactory* ipFactory = NULL;
	IWICBitmapDecoder* ipDecoder = NULL;
	WICBitmapDecoderSPtr decoder = nullptr;

	// Create the factory
	if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&ipFactory))))
		goto Return;

	// Create the decoder
	if (FAILED(ipFactory->CreateDecoderFromStream(stream->ipObject, NULL, WICDecodeMetadataCacheOnDemand, &ipDecoder)))
		goto ReleaseFactory;

	decoder = std::make_shared<WICBitmapDecoder>(ipDecoder);
ReleaseFactory:
	ipFactory->Release();
Return:
	return decoder;
}
WICBitmapSourceSPtr WIC::convertSourceFormat(WICBitmapSourceSPtr inputSource) {
	if (isCOMObjectNull(inputSource))
		return nullptr;

	IWICBitmapSource* ipSource = NULL;
	WICBitmapSourceSPtr outputSource = nullptr;

	// Convert the image to 32bpp BGRA format with pre-multiplied alpha
	//   (it may not be stored in that format natively in the image resource,
	//   but we need this format to create the DIB to use on-screen)
	if (SUCCEEDED(WICConvertBitmapSource(GUID_WICPixelFormat32bppBGRA, inputSource->ipObject, &ipSource)))
		outputSource = std::make_shared<WICBitmapSource>(ipSource);

	return outputSource;
}
HBITMAP WIC::createHBitmapFromSource(WICBitmapSourceSPtr source) {
	if (isCOMObjectNull(source))
		return nullptr;
	IWICBitmapSource* ipSource = NULL;
	WICBitmapSourceSPtr outputSource = nullptr;

	if (SUCCEEDED(WICConvertBitmapSource(GUID_WICPixelFormat32bppBGRA, source->ipObject, &ipSource)))
		outputSource = std::make_shared<WICBitmapSource>(ipSource);
	if (isCOMObjectNull(outputSource))
		return nullptr;

	Point2I size = readImageSizeFromSource(outputSource);
	if (size == Point2I::ZERO)
		return nullptr;

	std::vector<BYTE> buffer(size.x * size.y * 4);
	if (FAILED(outputSource->ipObject->CopyPixels(NULL, size.x * 4, buffer.size(), buffer.data())))
		return nullptr;

	HBITMAP hBitmap = CreateBitmap(size.x, size.y, 1, 32, buffer.data());
	if (hBitmap == INVALID_HANDLE_VALUE)
		return nullptr;

	return hBitmap;
}
WICBitmapSourceSPtr WIC::createSourceFromHBitmap(HBITMAP hBitmap) {
	if (hBitmap == nullptr)
		return nullptr;

	IWICImagingFactory* ipFactory = NULL;
	IWICBitmap* ipBitmap = NULL;
	WICBitmapSourceSPtr source = nullptr;

	CoInitialize(NULL);

	// Create the factory
	if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&ipFactory))))
		goto Return;

	// Create the bitmap
	if (FAILED(ipFactory->CreateBitmapFromHBITMAP(hBitmap, NULL, WICBitmapAlphaChannelOption::WICBitmapUseAlpha, &ipBitmap)))
		goto ReleaseFactory;

	source = std::make_shared<WICBitmapSource>(ipBitmap);

ReleaseFactory:
	ipFactory->Release();
Return:
	CoUninitialize();
	return source;
}
bool WIC::writeHBitmapToClipboard(HBITMAP hBitmap) {
	if (OpenClipboard(NULL)) {
		EmptyClipboard();

		SetClipboardData(CF_BITMAP, hBitmap);

		CloseClipboard();
		return true;
	}
	return false;
}
HBITMAP WIC::readHBitmapFromClipboard() {
	HBITMAP hBitmap = NULL;
	if (Clipboard::containsFormat(CF_BITMAP)) {
		if (OpenClipboard(NULL)) {
			hBitmap = (HBITMAP)GetClipboardData(CF_BITMAP);
			CloseClipboard();
		}
	}
	return hBitmap;
}

#pragma endregion
//================================
#pragma endregion
//=================================================================|
