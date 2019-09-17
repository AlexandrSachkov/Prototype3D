#include "util_TextureImporter.h"
#include "util_DDSTextureLoader.h"
#include "../assert.h"

#include <experimental/filesystem>
#include <memory>

#include "FreeImage.h"
#include "Compressonator.h"

namespace p3d {
    namespace util {
        bool TextureImporter::importTexture(
            const std::string& colorTexPathStr,
            const std::string& alphaTexPathStr,
            TEX_BLOCK_COMPRESSION compression,
            bool generateMipMaps,
            TextureDesc& out
        ) {
            P3D_ASSERT_R(!colorTexPathStr.empty() && !alphaTexPathStr.empty(), "Texture paths cannot be empty");

            FreeImage_SetOutputMessage([](FREE_IMAGE_FORMAT fif, const char *message) {
                P3D_ERROR_PRINT(std::string(FreeImage_GetFormatFromFIF(fif)) + ", " + std::string(message));
            });

            FREE_IMAGE_FORMAT colorTexFormat = FreeImage_GetFileType(colorTexPathStr.c_str(), 0);
            P3D_ASSERT_R(colorTexFormat != FREE_IMAGE_FORMAT::FIF_UNKNOWN, "Unsupported color texture format");
            FREE_IMAGE_FORMAT alphaTexFormat = FreeImage_GetFileType(alphaTexPathStr.c_str(), 0);
            P3D_ASSERT_R(alphaTexFormat != FREE_IMAGE_FORMAT::FIF_UNKNOWN, "Unsupported alpha texture format");

            BitmapPtr colorBitmap = createBitmapPtr(FreeImage_Load(colorTexFormat, colorTexPathStr.c_str(), colorTexFormat == FIF_JPEG ? JPEG_ACCURATE : 0));
            P3D_ASSERT_R(colorBitmap, "Failed to load color bitmap");

            BitmapPtr alphaBitmap = createBitmapPtr(FreeImage_Load(alphaTexFormat, alphaTexPathStr.c_str(), alphaTexFormat == FIF_JPEG ? JPEG_ACCURATE : 0));
            P3D_ASSERT_R(alphaBitmap, "Failed to load alpha bitmap");

            P3D_ASSERT_R(
                (FreeImage_GetWidth(colorBitmap.get()) == FreeImage_GetWidth(alphaBitmap.get())) &&
                (FreeImage_GetHeight(colorBitmap.get()) == FreeImage_GetHeight(alphaBitmap.get())),
                "Color and alpha textures are of different size"
            );

            BitmapPtr colorBitmapFinal = createBitmapPtr(FreeImage_ConvertTo32Bits(colorBitmap.get()));
            BitmapPtr alphaBitmap32 = createBitmapPtr(FreeImage_ConvertTo32Bits(alphaBitmap.get()));

            P3D_ASSERT_R(FreeImage_SetChannel(colorBitmapFinal.get(), FreeImage_GetChannel(alphaBitmap32.get(), FICC_RED), FICC_ALPHA),
                "Failed to combine textures");
            
            FREE_IMAGE_TYPE type = FreeImage_GetImageType(colorBitmapFinal.get());
            unsigned int width = FreeImage_GetWidth(colorBitmapFinal.get());
            unsigned int height = FreeImage_GetHeight(colorBitmapFinal.get());
            unsigned int bytesPerPixel = FreeImage_GetBPP(colorBitmapFinal.get()) / 8;
            unsigned int totalSize = width * height * bytesPerPixel;
            unsigned int pitch = FreeImage_GetPitch(colorBitmapFinal.get());
            BITMAPINFOHEADER* header = FreeImage_GetInfoHeader(colorBitmapFinal.get());
            BITMAPINFO* info = FreeImage_GetInfo(colorBitmapFinal.get());

            if (compression == TEX_BLOCK_COMPRESSION::NONE) {
                out.path = colorTexPathStr;
                out.dataSize = totalSize;
                out.data.reset(new char[out.dataSize]);
                memcpy(out.data.get(), colorBitmapFinal->data, out.dataSize);
                out.format.type = P3D_FORMAT_TYPE_VECTOR;
                out.format.format.vecFormat = P3D_VECTOR_FORMAT::P3D_FORMAT_B8G8R8A8_UNORM;

                TextureDesc::SurfaceDesc surfaceDesc;
                surfaceDesc.surfaceDim[0] = width;
                surfaceDesc.surfaceDim[1] = height;
                surfaceDesc.surfaceDim[2] = 1;
                surfaceDesc.rowSizeBytes = width * bytesPerPixel;
                surfaceDesc.slice2DSizeBytes = totalSize;
                surfaceDesc.surfaceSizeBytes = totalSize;

                std::vector<TextureDesc::SurfaceDesc> surfaceMipMaps;
                surfaceMipMaps.emplace_back(surfaceDesc);
                out.surfaceMatrix.emplace_back(surfaceMipMaps);

                return true;
            }
            
            return false;
        }

        bool TextureImporter::importTexture(
            const std::string& texPath,
            TEX_BLOCK_COMPRESSION compression,
            bool generateMipMaps,
            TextureDesc& out
        ) {
            P3D_ASSERT_R(!texPath.empty(), "Texture path cannot be empty");

            FreeImage_SetOutputMessage([](FREE_IMAGE_FORMAT fif, const char *message) {
                P3D_ERROR_PRINT(std::string(FreeImage_GetFormatFromFIF(fif)) + ", " + std::string(message));
            });

            FREE_IMAGE_FORMAT texFormat = FreeImage_GetFileType(texPath.c_str(), 0);
            P3D_ASSERT_R(texFormat != FREE_IMAGE_FORMAT::FIF_UNKNOWN, "Unsupported texture format");

            BitmapPtr colorBitmap = createBitmapPtr(FreeImage_Load(texFormat, texPath.c_str(), texFormat == FIF_JPEG ? JPEG_ACCURATE : 0));
            P3D_ASSERT_R(colorBitmap, "Failed to load bitmap");

            BitmapPtr colorBitmapFinal = createBitmapPtr(FreeImage_ConvertTo32Bits(colorBitmap.get()));

            FREE_IMAGE_TYPE type = FreeImage_GetImageType(colorBitmapFinal.get());
            unsigned int width = FreeImage_GetWidth(colorBitmapFinal.get());
            unsigned int height = FreeImage_GetHeight(colorBitmapFinal.get());
            unsigned int bytesPerPixel = FreeImage_GetBPP(colorBitmapFinal.get()) / 8;
            unsigned int totalSize = width * height * bytesPerPixel;
            unsigned int pitch = FreeImage_GetPitch(colorBitmapFinal.get());
            BITMAPINFOHEADER* header = FreeImage_GetInfoHeader(colorBitmapFinal.get());
            BITMAPINFO* info = FreeImage_GetInfo(colorBitmapFinal.get());

            if (compression == TEX_BLOCK_COMPRESSION::NONE) {
                out.path = texPath;
                out.dataSize = totalSize;
                out.data.reset(new char[out.dataSize]);
                FreeImage_ConvertToRawBits((BYTE*)out.data.get(), colorBitmapFinal.get(), pitch, FreeImage_GetBPP(colorBitmapFinal.get()),
                    FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, TRUE);
                out.format.type = P3D_FORMAT_TYPE_VECTOR;
                out.format.format.vecFormat = P3D_VECTOR_FORMAT::P3D_FORMAT_B8G8R8A8_UNORM;

                TextureDesc::SurfaceDesc surfaceDesc;
                surfaceDesc.surfaceDim[0] = width;
                surfaceDesc.surfaceDim[1] = height;
                surfaceDesc.surfaceDim[2] = 1;
                surfaceDesc.rowSizeBytes = width * bytesPerPixel;
                surfaceDesc.slice2DSizeBytes = totalSize;
                surfaceDesc.surfaceSizeBytes = totalSize;

                std::vector<TextureDesc::SurfaceDesc> surfaceMipMaps;
                surfaceMipMaps.emplace_back(surfaceDesc);
                out.surfaceMatrix.emplace_back(surfaceMipMaps);

                return true;
            }

            return false;
        }

        std::unique_ptr<FIBITMAP, std::function<void(FIBITMAP*)>> TextureImporter::createBitmapPtr(FIBITMAP* bitmap) {
            return { bitmap, [](FIBITMAP* ptr) { FreeImage_Unload(ptr); } };
        }
    }
}