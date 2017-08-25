/*=================================================================|
|* File:				WICWrapper.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef WIC_WRAPPER_H
#define WIC_WRAPPER_H

#include <wincodec.h>
#include <vector>
#include <memory>
#include <PowerConsole/Declarations.h>
#include <PowerConsole/Geometry/Point.h>
#include <PowerConsole/Drawing/Color.h>
#include <PowerConsole/IO/Clipboard.h>
#include <AsciiArtist/COM/COM.h>

using namespace PowerConsole::Geometry;
using namespace PowerConsole::IO;
using namespace PowerConsole::Drawing;

namespace AsciiArtist::COM {
//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region BitmapFrame

struct BitmapFrame {
	//=========== MEMBERS ============
	#pragma region Members

	/* The size of the frame. */
	Point2I size;
	/* The 1D array of pixels in the frame. */
	ColorB* pixels;
	/* The animation speed of the frame in milliseconds. */
	int frameSpeed;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the bitmap frame. */
	BitmapFrame();
	/* Deconstructs the bitmap frame. */
	~BitmapFrame();

	#pragma endregion
};

typedef std::shared_ptr<BitmapFrame> BitmapFrameSPtr;

#pragma endregion
//=================================================================|
#pragma region BitmapFrameCollection

struct BitmapFrameCollection {
	//=========== MEMBERS ============
	#pragma region Members

	/* The list of frames in the image. */
	std::vector<BitmapFrameSPtr> frames;

	#pragma endregion
};

typedef std::shared_ptr<BitmapFrameCollection> BitmapFrameCollectionSPtr;

#pragma endregion
//=================================================================|
#pragma region WIC

typedef COMObject<IWICBitmapDecoder> WICBitmapDecoder;
typedef COMObject<IWICBitmapEncoder> WICBitmapEncoder;
typedef COMObject<IWICBitmapSource> WICBitmapSource;
typedef std::shared_ptr<WICBitmapDecoder> WICBitmapDecoderSPtr;
typedef std::shared_ptr<WICBitmapEncoder> WICBitmapEncoderSPtr;
typedef std::shared_ptr<WICBitmapSource> WICBitmapSourceSPtr;

/* The different types of scaling interpolations available with scaleSource. */
enum class BitmapScaleModes {
	NearestNeighbor = WICBitmapInterpolationModeNearestNeighbor,
	Linear = WICBitmapInterpolationModeLinear,
	Cubic = WICBitmapInterpolationModeCubic,
	Fant = WICBitmapInterpolationModeFant
};

class WIC {
	//=========== READING ============
	#pragma region Functions
	//--------------------------------
	#pragma region Decoder
public:
	/* Creates a WIC bitmap decoder from a file. */
	static WICBitmapDecoderSPtr createDecoderFromFile(const std::string& path);
	/* Creates a WIC bitmap decoder from a resource. */
	static WICBitmapDecoderSPtr createDecoderFromResource(unsigned int id, unsigned int typeId);
	/* Creates a WIC bitmap decoder from a resource. */
	static WICBitmapDecoderSPtr createDecoderFromResource(unsigned int id, const std::string& type);
	/* Creates a gif encoder from a file. */
	static WICBitmapEncoderSPtr createGifEncoderFromStream(COMStreamSPtr stream, Point2I size);
	/* Writes a bitmap frame to the gif encoder. */
	static bool writeBitmapToGifEncoder(WICBitmapEncoderSPtr encoder, BitmapFrameSPtr bitmap);
	/* Fixes the gif after it has been written to. */
	static bool writeFixesToGifFile(const std::string& path, int initialFrameSpeed);

	#pragma endregion
	//--------------------------------
	#pragma region Single Source

	/* Writes the source to a file. */
	static bool writeSourceToFile(const std::string& path, WICBitmapSourceSPtr source);
	/* Writes the source to the clipboard. */
	static bool writeSourceToClipboard(WICBitmapSourceSPtr source);
	/* Reads a single WIC bitmap source from a file. */
	static WICBitmapSourceSPtr readSourceFromFile(const std::string& path);
	/* Reads a single WIC bitmap source from a resource. */
	static WICBitmapSourceSPtr readSourceFromResource(unsigned int id, unsigned int typeId);
	/* Reads a single WIC bitmap source from a resource. */
	static WICBitmapSourceSPtr readSourceFromResource(unsigned int id, const std::string& type);
	/* Reads a single WIC bitmap source from clipboard. */
	static WICBitmapSourceSPtr readSourceFromClipboard();
	/* Reads a single WIC bitmap source from a WIC bitmap decoder. */
	static WICBitmapSourceSPtr readSourceFromDecoder(WICBitmapDecoderSPtr decoder, int frameIndex = 0);

	#pragma endregion
	//--------------------------------
	#pragma region Frame Count

	/* Reads the frame count from a file. */
	static int readFrameCountFromFile(const std::string& path);
	/* Reads the frame count from a resource. */
	static int readFrameCountFromResource(unsigned int id, unsigned int typeId);
	/* Reads the frame count from a resource. */
	static int readFrameCountFromResource(unsigned int id, const std::string& type);
	/* Reads the frame count from a WIC bitmap decoder. */
	static int readFrameCountFromDecoder(WICBitmapDecoderSPtr decoder);

	#pragma endregion
	//--------------------------------
	#pragma region Image Size

	/* Gets the size of the image a file. */
	static Point2I readImageSizeFromFile(const std::string& path);
	/* Gets the size of the image a resource. */
	static Point2I readImageSizeFromResource(unsigned int id, unsigned int typeId);
	/* Gets the size of the image a resource. */
	static Point2I readImageSizeFromResource(unsigned int id, const std::string& type);
	/* Gets the size of the image the clipboard. */
	static Point2I readImageSizeFromClipboard();
	/* Gets the size of the image from a WIC bitmap decoder. */
	static Point2I readImageSizeFromDecoder(WICBitmapDecoderSPtr decoder);
	/* Reads the image size from a WIC bitmap source. */
	static Point2I readImageSizeFromSource(WICBitmapSourceSPtr source);

	#pragma endregion
	//--------------------------------
	#pragma region Single Bitmap

	/* Creates a bitmap with the given size. */
	static BitmapFrameSPtr createBitmap(Point2I size);
	/* Writes the bitmap to a file. */
	static bool writeBitmapToFile(const std::string& path, BitmapFrameSPtr bitmap);
	/* Writes the bitmap to the clipboard. */
	static bool writeBitmapToClipboard(BitmapFrameSPtr bitmap);
	/* Writes the bitmap to the WIC bitmap source. */
	static WICBitmapSourceSPtr writeBitmapToSource(BitmapFrameSPtr bitmap);
	/* Reads a bitmap frame from a file. */
	static BitmapFrameSPtr readBitmapFromFile(const std::string& path, int frameIndex = 0);
	/* Reads a bitmap frame from a resource. */
	static BitmapFrameSPtr readBitmapFromResource(unsigned int id, unsigned int typeId, int frameIndex = 0);
	/* Reads a bitmap frame from a resource. */
	static BitmapFrameSPtr readBitmapFromResource(unsigned int id, const std::string& type, int frameIndex = 0);
	/* Reads a single bitmap frame from a WIC bitmap decoder. */
	static BitmapFrameSPtr readBitmapFromDecoder(WICBitmapDecoderSPtr decoder, int frameIndex = 0);
	/* Reads a single bitmap frame from a WIC bitmap source. */
	static BitmapFrameSPtr readBitmapFromSource(WICBitmapSourceSPtr source);

	#pragma endregion
	//--------------------------------
	#pragma region Multiple Bitmaps

	/* Creates a bitmap frame collection. */
	static BitmapFrameCollectionSPtr createBitmapCollection();
	/* Writes the bitmap collection to a gif file. */
	static bool writeBitmapCollectionToFile(const std::string& path, BitmapFrameCollectionSPtr bitmaps);
	/* Reads a bitmap frame collection from a file. */
	static BitmapFrameCollectionSPtr readBitmapCollectionFromFile(const std::string& path);
	/* Reads a bitmap frame collection from a resource. */
	static BitmapFrameCollectionSPtr readBitmapCollectionFromResource(unsigned int id, unsigned int typeId);
	/* Reads a bitmap frame collection from a resource. */
	static BitmapFrameCollectionSPtr readBitmapCollectionFromResource(unsigned int id, const std::string& type);
	/* Reads a bitmap frame collection from a WIC bitmap decoder. */
	static BitmapFrameCollectionSPtr readBitmapCollectionFromDecoder(WICBitmapDecoderSPtr decoder);

	#pragma endregion
	//--------------------------------
	#pragma region Modification

	/* Scales the WIC bitmap source. */
	static WICBitmapSourceSPtr scaleSource(WICBitmapSourceSPtr source, Point2F scale, BitmapScaleModes mode);

	#pragma endregion
	//--------------------------------
	#pragma endregion
	//=========== HELPERS ============
	#pragma region Helpers
//private:
	/* Creates a COM stream from a file. */
	static COMStreamSPtr createStreamFromFile(const std::string& path, bool readAccess, bool writeAccess, bool create);
	/* Creates a COM stream from a resource. */
	static COMStreamSPtr createStreamFromResource(unsigned int id, unsigned int typeId);
	/* Creates a COM stream from a resource. */
	static COMStreamSPtr createStreamFromResource(unsigned int id, const std::string& type);
	/* Creates a WIC bitmap decoder from a COM stream. */
	static WICBitmapDecoderSPtr createDecoderFromStream(COMStreamSPtr stream);
	/* Converts a WIC bitmap source to the correct format. */
	static WICBitmapSourceSPtr convertSourceFormat(WICBitmapSourceSPtr source);
	/* Creates an hbitmap from a WIC bitmap source. */
	static HBITMAP createHBitmapFromSource(WICBitmapSourceSPtr source);
	/* Creates a WIC bitmap source from an hbitmap. */
	static WICBitmapSourceSPtr createSourceFromHBitmap(HBITMAP hBitmap);
	/* Writes an hbitmap to the clipboard. */
	static bool writeHBitmapToClipboard(HBITMAP hBitmap);
	/* Reads an hbitmap from the clipboard. */
	static HBITMAP readHBitmapFromClipboard();

	#pragma endregion
};

#pragma endregion
//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
