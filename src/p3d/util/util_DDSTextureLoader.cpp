#include "util_DDSTextureLoader.h"
#include "../assert.h"

#include <fstream>
#include <algorithm>
#include <memory>

namespace p3d {
    namespace util {
        bool DDSTextureLoader::loadDDSTextureFromFile(
            const std::string& path,
            TextureDesc& texDesc
        ) {
            DDSHeader ddsHeader;
            DDSHeaderDxt10 ddsHeaderDxt10;
            bool dxt10 = false;
            std::shared_ptr<char> data = nullptr;
            unsigned int dataSize = 0;

            std::memset(&ddsHeader, 0, sizeof(DDSHeader));
            std::memset(&ddsHeaderDxt10, 0, sizeof(DDSHeaderDxt10));

            P3D_ASSERT_R(readDDSFile(path, ddsHeader, ddsHeaderDxt10, dxt10, data, dataSize), "Failed to read DDS file");
            if (!loadDDSData(ddsHeader, ddsHeaderDxt10, dxt10, data, dataSize, texDesc)) {
                P3D_ASSERT_R(false, "Failed to load DDS data");
            }
            return true;
        }

        bool DDSTextureLoader::readDDSFile(
            const std::string& path,
            DDSHeader& ddsHeader,
            DDSHeaderDxt10& ddsHeaderDxt10,
            bool& dxt10,
            std::shared_ptr<char>& data,
            unsigned int& dataSize) {

            std::ifstream ifs(path, std::ifstream::in | std::ifstream::binary);
            P3D_ASSERT_R(ifs.good(), "Failed to open file stream");

            ifs.seekg(0, ifs.end);
            const unsigned int fileSizeBytes = (unsigned int)ifs.tellg();
            P3D_ASSERT_R(fileSizeBytes >= BASIC_HEADER_SIZE_BYTES, "Invalid header");
            ifs.seekg(0, ifs.beg);

            char magic[MAGIC_SIZE_BYTES];
            ifs.read(magic, MAGIC_SIZE_BYTES);
            P3D_ASSERT_R(magic[0] == 'D' && magic[1] == 'D' && magic[2] == 'S', "Not a DDS file");

            ifs.read(reinterpret_cast<char*>(&ddsHeader), sizeof(DDSHeader));

            dxt10 = false;
            dataSize = fileSizeBytes - BASIC_HEADER_SIZE_BYTES;
            if (ddsHeader.ddsPixelFormat.flags == DDPF_FOURCC
                && compareFourCC(ddsHeader.ddsPixelFormat.fourCC, 'D', 'X', '1', '0')) {
                P3D_ASSERT_R(fileSizeBytes >= DX10_HEADER_SIZE_BYTES, "Invalid header");
                ifs.read(reinterpret_cast<char*>(&ddsHeaderDxt10), sizeof(DDSHeaderDxt10));
                dxt10 = true;
                dataSize = fileSizeBytes - DX10_HEADER_SIZE_BYTES;
            }

            data.reset(new char[dataSize]);
            ifs.read(data.get(), dataSize);

            return true;
        }

        bool DDSTextureLoader::loadDDSData(
            const DDSHeader& ddsHeader,
            const DDSHeaderDxt10& dx10Header,
            bool dtx10,
            std::shared_ptr<char> data,
            unsigned int dataSize,
            TextureDesc& texDesc
        ) {

            const unsigned int requiredFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
            P3D_ASSERT_R(ddsHeader.flags & requiredFlags, "Header does not set required flags");

            unsigned int width = ddsHeader.width;
            unsigned int height = ddsHeader.height > 0 ? ddsHeader.height : 1;
            unsigned int depth = 1;
            if (ddsHeader.flags & DDSD_DEPTH && ddsHeader.depth > 1) {
                depth = ddsHeader.depth;
            }

            DXGI_FORMAT dxgiFormat;
            if (dtx10) {
                dxgiFormat = dx10Header.dxgiFormat;
            } else {
                dxgiFormat = getDxgiFormat(ddsHeader.ddsPixelFormat);
            }

            Format format;
            P3D_ASSERT_R(convertFormat(dxgiFormat, format), "Unable to convert to P3D format");

            unsigned int numMipMaps = 1;
            if (ddsHeader.flags & DDSD_MIPMAPCOUNT) {
                numMipMaps = ddsHeader.mipMapCount;
            }

            unsigned int arraySize = 1;
            if (dtx10) {
                arraySize = dx10Header.arraySize;
            }

            TextureDesc desc;
            desc.format = format;
            desc.data = data;
            desc.dataSize = dataSize;

            unsigned int dataOffset = 0;
            for (unsigned int iTex = 0; iTex < arraySize; iTex++) {
                unsigned int mmWidth = width;
                unsigned int mmHeight = height;
                unsigned int mmDepth = depth;
                std::vector<TextureDesc::SurfaceDesc> mipMaps;
                for (unsigned int iMipMap = 0; iMipMap < numMipMaps; iMipMap++) {
                    unsigned int rowSizeBytes, surface2dSizeBytes;
                    P3D_ASSERT_R(getSurfaceSizes(mmWidth, mmHeight, format, rowSizeBytes, surface2dSizeBytes),
                        "Failed to obtain surface size");

                    unsigned int surfaceSizeBytes = surface2dSizeBytes * mmDepth;
                    P3D_ASSERT_R(dataOffset + surfaceSizeBytes <= dataSize,
                        "Data array out of bounds");

                    TextureDesc::SurfaceDesc surfaceDesc;
                    surfaceDesc.surfaceDim[0] = mmWidth;
                    surfaceDesc.surfaceDim[1] = mmHeight;
                    surfaceDesc.surfaceDim[2] = mmDepth;
                    surfaceDesc.rowSizeBytes = rowSizeBytes;
                    surfaceDesc.slice2DSizeBytes = surface2dSizeBytes;
                    surfaceDesc.surfaceSizeBytes = surfaceSizeBytes;
                    mipMaps.push_back(surfaceDesc);

                    mmWidth = mmWidth >> 1;
                    mmHeight = mmHeight >> 1;
                    mmDepth = mmDepth >> 1;
                    if (mmWidth == 0) {
                        mmWidth = 1;
                    }
                    if (mmHeight == 0) {
                        mmHeight = 1;
                    }
                    if (mmDepth == 0) {
                        mmDepth = 1;
                    }
                }
                desc.surfaceMatrix.push_back(mipMaps);
            }

            texDesc = desc;
            return true;
        }

        bool DDSTextureLoader::compareFourCC(unsigned int fourCC, char ch1, char ch2, char ch3, char ch4) {
            char chars[4];
            std::memcpy(chars, &fourCC, sizeof(unsigned int));
            if (chars[0] == ch1 && chars[1] == ch2 && chars[2] == ch3 && chars[3] == ch4) {
                return true;
            }
            return false;
        }

        bool DDSTextureLoader::getSurfaceSizes(
            unsigned int width,
            unsigned int height,
            Format format,
            unsigned int& rowSizeBytes,
            unsigned int& surfaceSizeBytes
        ) {
            if (format.type == P3D_FORMAT_TYPE_VECTOR) {
                unsigned int bitsPerElement = getVecFormatSizeBits(format.format.vecFormat);
                P3D_ASSERT_R(bitsPerElement > 0, "Failed to get format size");

                rowSizeBytes = (width * bitsPerElement + 7) / 8;
                surfaceSizeBytes = rowSizeBytes * height;
            } else if (format.type == P3D_FORMAT_TYPE_BLOCK_COMPRESSION) {
                unsigned int bytesPerElement = getBlockCompressionFormatSizeBytes(format.format.bcFormat);
                P3D_ASSERT_R(bytesPerElement > 0, "Failed to get format size");

                unsigned int numBlocksWide = (unsigned int)std::max<size_t>(1, (width + 3) / 4);
                unsigned int numBlocksHigh = (unsigned int)std::max<size_t>(1, (height + 3) / 4);

                rowSizeBytes = numBlocksWide * bytesPerElement;
                surfaceSizeBytes = rowSizeBytes * numBlocksHigh;
            } else {
                P3D_ASSERT_R(false, "Unsupported format type");
            }

            return true;
        }

        bool DDSTextureLoader::convertFormat(DXGI_FORMAT dxgiFormat, Format& format) {
            switch (dxgiFormat) {
            case DXGI_FORMAT_R32G32B32A32_TYPELESS:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R32G32B32A32_TYPELESS };
                return true;
            case DXGI_FORMAT_R32G32B32A32_FLOAT:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R32G32B32A32_FLOAT };
                return true;
            case DXGI_FORMAT_R32G32B32A32_UINT:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R32G32B32A32_UINT };
                return true;
            case DXGI_FORMAT_R32G32B32A32_SINT:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R32G32B32A32_SINT };
                return true;
            case DXGI_FORMAT_R32G32B32_TYPELESS:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R32G32B32_TYPELESS };
                return true;
            case DXGI_FORMAT_R32G32B32_FLOAT:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R32G32B32_FLOAT };
                return true;
            case DXGI_FORMAT_R32G32B32_UINT:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R32G32B32_UINT };
                return true;
            case DXGI_FORMAT_R32G32B32_SINT:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R32G32B32_SINT };
                return true;
            case DXGI_FORMAT_R16G16B16A16_TYPELESS:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R16G16B16A16_TYPELESS };
                return true;
            case DXGI_FORMAT_R16G16B16A16_FLOAT:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R16G16B16A16_FLOAT };
                return true;
            case DXGI_FORMAT_R16G16B16A16_UNORM:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R16G16B16A16_UNORM };
                return true;
            case DXGI_FORMAT_R16G16B16A16_UINT:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R16G16B16A16_UINT };
                return true;
            case DXGI_FORMAT_R16G16B16A16_SNORM:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R16G16B16A16_SNORM };
                return true;
            case DXGI_FORMAT_R16G16B16A16_SINT:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R16G16B16A16_SINT };
                return true;
            case DXGI_FORMAT_R32G32_TYPELESS:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R32G32_TYPELESS };
                return true;
            case DXGI_FORMAT_R32G32_FLOAT:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R32G32_FLOAT };
                return true;
            case DXGI_FORMAT_R32G32_UINT:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R32G32_UINT };
                return true;
            case DXGI_FORMAT_R32G32_SINT:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R32G32_SINT };
                return true;
            case DXGI_FORMAT_R32G8X24_TYPELESS:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R32G8X24_TYPELESS };
                return true;
            case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_D32_FLOAT_S8X24_UINT };
                return true;
            case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R32_FLOAT_X8X24_TYPELESS };
                return true;
            case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_X32_TYPELESS_G8X24_UINT };
                return true;
            case DXGI_FORMAT_R10G10B10A2_TYPELESS:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R10G10B10A2_TYPELESS };
                return true;
            case DXGI_FORMAT_R10G10B10A2_UNORM:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R10G10B10A2_UNORM };
                return true;
            case DXGI_FORMAT_R10G10B10A2_UINT:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R10G10B10A2_UINT };
                return true;
            case DXGI_FORMAT_R11G11B10_FLOAT:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R11G11B10_FLOAT };
                return true;
            case DXGI_FORMAT_R8G8B8A8_TYPELESS:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R8G8B8A8_TYPELESS };
                return true;
            case DXGI_FORMAT_R8G8B8A8_UNORM:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R8G8B8A8_UNORM };
                return true;
            case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R8G8B8A8_UNORM_SRGB };
                return true;
            case DXGI_FORMAT_R8G8B8A8_UINT:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R8G8B8A8_UINT };
                return true;
            case DXGI_FORMAT_R8G8B8A8_SNORM:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R8G8B8A8_SNORM };
                return true;
            case DXGI_FORMAT_R8G8B8A8_SINT:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R8G8B8A8_SINT };
                return true;
            case DXGI_FORMAT_R16G16_TYPELESS:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R16G16_TYPELESS };
                return true;
            case DXGI_FORMAT_R16G16_FLOAT:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R16G16_FLOAT };
                return true;
            case DXGI_FORMAT_R16G16_UNORM:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R16G16_UNORM };
                return true;
            case DXGI_FORMAT_R16G16_UINT:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R16G16_UINT };
                return true;
            case DXGI_FORMAT_R16G16_SNORM:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R16G16_SNORM };
                return true;
            case DXGI_FORMAT_R16G16_SINT:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R16G16_SINT };
                return true;
            case DXGI_FORMAT_R32_TYPELESS:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R32_TYPELESS };
                return true;
            case DXGI_FORMAT_D32_FLOAT:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_D32_FLOAT };
                return true;
            case DXGI_FORMAT_R32_FLOAT:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R32_FLOAT };
                return true;
            case DXGI_FORMAT_R32_UINT:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R32_UINT };
                return true;
            case DXGI_FORMAT_R32_SINT:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R32_SINT };
                return true;
            case DXGI_FORMAT_R24G8_TYPELESS:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R24G8_TYPELESS };
                return true;
            case DXGI_FORMAT_D24_UNORM_S8_UINT:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_D24_UNORM_S8_UINT };
                return true;
            case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R24_UNORM_X8_TYPELESS };
                return true;
            case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_X24_TYPELESS_G8_UINT };
                return true;
            case DXGI_FORMAT_R8G8_TYPELESS:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R8G8_TYPELESS };
                return true;
            case DXGI_FORMAT_R8G8_UNORM:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R8G8_UNORM };
                return true;
            case DXGI_FORMAT_R8G8_UINT:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R8G8_UINT };
                return true;
            case DXGI_FORMAT_R8G8_SNORM:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R8G8_SNORM };
                return true;
            case DXGI_FORMAT_R8G8_SINT:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R8G8_SINT };
                return true;
            case DXGI_FORMAT_R16_TYPELESS:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R16_TYPELESS };
                return true;
            case DXGI_FORMAT_R16_FLOAT:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R16_FLOAT };
                return true;
            case DXGI_FORMAT_D16_UNORM:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_D16_UNORM };
                return true;
            case DXGI_FORMAT_R16_UNORM:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R16_UNORM };
                return true;
            case DXGI_FORMAT_R16_UINT:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R16_UINT };
                return true;
            case DXGI_FORMAT_R16_SNORM:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R16_SNORM };
                return true;
            case DXGI_FORMAT_R16_SINT:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R16_SINT };
                return true;
            case DXGI_FORMAT_R8_TYPELESS:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R8_TYPELESS };
                return true;
            case DXGI_FORMAT_R8_UNORM:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R8_UNORM };
                return true;
            case DXGI_FORMAT_R8_UINT:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R8_UINT };
                return true;
            case DXGI_FORMAT_R8_SNORM:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R8_SNORM };
                return true;
            case DXGI_FORMAT_R8_SINT:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R8_SINT };
                return true;
            case DXGI_FORMAT_A8_UNORM:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_A8_UNORM };
                return true;
            case DXGI_FORMAT_R1_UNORM:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_R1_UNORM };
                return true;
            case DXGI_FORMAT_B8G8R8A8_UNORM:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_B8G8R8A8_UNORM };
                return true;
            case DXGI_FORMAT_B8G8R8X8_UNORM:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_B8G8R8X8_UNORM };
                return true;
            case DXGI_FORMAT_B8G8R8A8_TYPELESS:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_B8G8R8A8_TYPELESS };
                return true;
            case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_B8G8R8A8_UNORM_SRGB };
                return true;
            case DXGI_FORMAT_B8G8R8X8_TYPELESS:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_B8G8R8X8_TYPELESS };
                return true;
            case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
                format = { P3D_FORMAT_TYPE_VECTOR, P3D_FORMAT_B8G8R8X8_UNORM_SRGB };
                return true;


            case DXGI_FORMAT_BC1_TYPELESS:
                format.type = P3D_FORMAT_TYPE_BLOCK_COMPRESSION; format.format.bcFormat = P3D_FORMAT_BC1_TYPELESS;
                return true;
            case DXGI_FORMAT_BC1_UNORM:
                format.type = P3D_FORMAT_TYPE_BLOCK_COMPRESSION; format.format.bcFormat = P3D_FORMAT_BC1_UNORM;
                return true;
            case DXGI_FORMAT_BC1_UNORM_SRGB:
                format.type = P3D_FORMAT_TYPE_BLOCK_COMPRESSION; format.format.bcFormat = P3D_FORMAT_BC1_UNORM_SRGB;
                return true;
            case DXGI_FORMAT_BC2_TYPELESS:
                format.type = P3D_FORMAT_TYPE_BLOCK_COMPRESSION; format.format.bcFormat = P3D_FORMAT_BC2_TYPELESS;
                return true;
            case DXGI_FORMAT_BC2_UNORM:
                format.type = P3D_FORMAT_TYPE_BLOCK_COMPRESSION; format.format.bcFormat = P3D_FORMAT_BC2_UNORM;
                return true;
            case DXGI_FORMAT_BC2_UNORM_SRGB:
                format.type = P3D_FORMAT_TYPE_BLOCK_COMPRESSION; format.format.bcFormat = P3D_FORMAT_BC2_UNORM_SRGB;
                return true;
            case DXGI_FORMAT_BC3_TYPELESS:
                format.type = P3D_FORMAT_TYPE_BLOCK_COMPRESSION; format.format.bcFormat = P3D_FORMAT_BC3_TYPELESS;
                return true;
            case DXGI_FORMAT_BC3_UNORM:
                format.type = P3D_FORMAT_TYPE_BLOCK_COMPRESSION; format.format.bcFormat = P3D_FORMAT_BC3_UNORM;
                return true;
            case DXGI_FORMAT_BC3_UNORM_SRGB:
                format.type = P3D_FORMAT_TYPE_BLOCK_COMPRESSION; format.format.bcFormat = P3D_FORMAT_BC3_UNORM_SRGB;
                return true;
            case DXGI_FORMAT_BC4_TYPELESS:
                format.type = P3D_FORMAT_TYPE_BLOCK_COMPRESSION; format.format.bcFormat = P3D_FORMAT_BC4_TYPELESS;
                return true;
            case DXGI_FORMAT_BC4_UNORM:
                format.type = P3D_FORMAT_TYPE_BLOCK_COMPRESSION; format.format.bcFormat = P3D_FORMAT_BC4_UNORM;
                return true;
            case DXGI_FORMAT_BC4_SNORM:
                format.type = P3D_FORMAT_TYPE_BLOCK_COMPRESSION; format.format.bcFormat = P3D_FORMAT_BC4_SNORM;
                return true;
            case DXGI_FORMAT_BC5_TYPELESS:
                format.type = P3D_FORMAT_TYPE_BLOCK_COMPRESSION; format.format.bcFormat = P3D_FORMAT_BC5_TYPELESS;
                return true;
            case DXGI_FORMAT_BC5_UNORM:
                format.type = P3D_FORMAT_TYPE_BLOCK_COMPRESSION; format.format.bcFormat = P3D_FORMAT_BC5_UNORM;
                return true;
            case DXGI_FORMAT_BC5_SNORM:
                format.type = P3D_FORMAT_TYPE_BLOCK_COMPRESSION; format.format.bcFormat = P3D_FORMAT_BC5_SNORM;
                return true;
            case DXGI_FORMAT_BC6H_TYPELESS:
                format.type = P3D_FORMAT_TYPE_BLOCK_COMPRESSION; format.format.bcFormat = P3D_FORMAT_BC6H_TYPELESS;
                return true;
            case DXGI_FORMAT_BC6H_UF16:
                format.type = P3D_FORMAT_TYPE_BLOCK_COMPRESSION; format.format.bcFormat = P3D_FORMAT_BC6H_UF16;
                return true;
            case DXGI_FORMAT_BC6H_SF16:
                format.type = P3D_FORMAT_TYPE_BLOCK_COMPRESSION; format.format.bcFormat = P3D_FORMAT_BC6H_SF16;
                return true;
            case DXGI_FORMAT_BC7_TYPELESS:
                format.type = P3D_FORMAT_TYPE_BLOCK_COMPRESSION; format.format.bcFormat = P3D_FORMAT_BC7_TYPELESS;
                return true;
            case DXGI_FORMAT_BC7_UNORM:
                format.type = P3D_FORMAT_TYPE_BLOCK_COMPRESSION; format.format.bcFormat = P3D_FORMAT_BC7_UNORM;
                return true;
            case DXGI_FORMAT_BC7_UNORM_SRGB:
                format.type = P3D_FORMAT_TYPE_BLOCK_COMPRESSION; format.format.bcFormat = P3D_FORMAT_BC7_UNORM_SRGB;
                return true;


            case DXGI_FORMAT_B4G4R4A4_UNORM:
            case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
            case DXGI_FORMAT_R8G8_B8G8_UNORM:
            case DXGI_FORMAT_G8R8_G8B8_UNORM:
            case DXGI_FORMAT_AYUV:
            case DXGI_FORMAT_Y410:
            case DXGI_FORMAT_Y416:
            case DXGI_FORMAT_NV12:
            case DXGI_FORMAT_P010:
            case DXGI_FORMAT_P016:
            case DXGI_FORMAT_420_OPAQUE:
            case DXGI_FORMAT_YUY2:
            case DXGI_FORMAT_Y210:
            case DXGI_FORMAT_Y216:
            case DXGI_FORMAT_NV11:
            case DXGI_FORMAT_AI44:
            case DXGI_FORMAT_IA44:
            case DXGI_FORMAT_P8:
            case DXGI_FORMAT_A8P8:
            case DXGI_FORMAT_P208:
            case DXGI_FORMAT_V208:
            case DXGI_FORMAT_V408:
            case DXGI_FORMAT_B5G6R5_UNORM:
            case DXGI_FORMAT_B5G5R5A1_UNORM:
            case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
            case DXGI_FORMAT_FORCE_UINT:
            case DXGI_FORMAT_UNKNOWN:
            default:
                P3D_ASSERT_R(false, "Unable to convert format");
            }
        }

        // this code is taken from https://github.com/Microsoft/DirectXTex/blob/master/DDSTextureLoader/DDSTextureLoader.cpp
    #define ISBITMASK( r,g,b,a ) ( ddpf.RBitMask == r && ddpf.GBitMask == g && ddpf.BBitMask == b && ddpf.ABitMask == a )
        p3d::util::DDSTextureLoader::DXGI_FORMAT DDSTextureLoader::getDxgiFormat(const DDSPixelFormat& ddpf) {
            if (ddpf.flags & DDPF_RGB) {
                // Note that sRGB formats are written using the "DX10" extended header

                switch (ddpf.RGBBitCount) {
                case 32:
                    if (ISBITMASK(0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000)) {
                        return DXGI_FORMAT_R8G8B8A8_UNORM;
                    }

                    if (ISBITMASK(0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000)) {
                        return DXGI_FORMAT_B8G8R8A8_UNORM;
                    }

                    if (ISBITMASK(0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000)) {
                        return DXGI_FORMAT_B8G8R8X8_UNORM;
                    }

                    // No DXGI format maps to ISBITMASK(0x000000ff,0x0000ff00,0x00ff0000,0x00000000) aka D3DFMT_X8B8G8R8

                    // Note that many common DDS reader/writers (including D3DX) swap the
                    // the RED/BLUE masks for 10:10:10:2 formats. We assumme
                    // below that the 'backwards' header mask is being used since it is most
                    // likely written by D3DX. The more robust solution is to use the 'DX10'
                    // header extension and specify the DXGI_FORMAT_R10G10B10A2_UNORM format directly

                    // For 'correct' writers, this should be 0x000003ff,0x000ffc00,0x3ff00000 for RGB data
                    if (ISBITMASK(0x3ff00000, 0x000ffc00, 0x000003ff, 0xc0000000)) {
                        return DXGI_FORMAT_R10G10B10A2_UNORM;
                    }

                    // No DXGI format maps to ISBITMASK(0x000003ff,0x000ffc00,0x3ff00000,0xc0000000) aka D3DFMT_A2R10G10B10

                    if (ISBITMASK(0x0000ffff, 0xffff0000, 0x00000000, 0x00000000)) {
                        return DXGI_FORMAT_R16G16_UNORM;
                    }

                    if (ISBITMASK(0xffffffff, 0x00000000, 0x00000000, 0x00000000)) {
                        // Only 32-bit color channel format in D3D9 was R32F
                        return DXGI_FORMAT_R32_FLOAT; // D3DX writes this out as a FourCC of 114
                    }
                    break;

                case 24:
                    // No 24bpp DXGI formats aka D3DFMT_R8G8B8
                    break;

                case 16:
                    if (ISBITMASK(0x7c00, 0x03e0, 0x001f, 0x8000)) {
                        return DXGI_FORMAT_B5G5R5A1_UNORM;
                    }
                    if (ISBITMASK(0xf800, 0x07e0, 0x001f, 0x0000)) {
                        return DXGI_FORMAT_B5G6R5_UNORM;
                    }

                    // No DXGI format maps to ISBITMASK(0x7c00,0x03e0,0x001f,0x0000) aka D3DFMT_X1R5G5B5

                    if (ISBITMASK(0x0f00, 0x00f0, 0x000f, 0xf000)) {
                        return DXGI_FORMAT_B4G4R4A4_UNORM;
                    }

                    // No DXGI format maps to ISBITMASK(0x0f00,0x00f0,0x000f,0x0000) aka D3DFMT_X4R4G4B4

                    // No 3:3:2, 3:3:2:8, or paletted DXGI formats aka D3DFMT_A8R3G3B2, D3DFMT_R3G3B2, D3DFMT_P8, D3DFMT_A8P8, etc.
                    break;
                }
            } else if (ddpf.flags & DDPF_LUMINANCE) {
                if (8 == ddpf.RGBBitCount) {
                    if (ISBITMASK(0x000000ff, 0x00000000, 0x00000000, 0x00000000)) {
                        return DXGI_FORMAT_R8_UNORM; // D3DX10/11 writes this out as DX10 extension
                    }

                    // No DXGI format maps to ISBITMASK(0x0f,0x00,0x00,0xf0) aka D3DFMT_A4L4
                }

                if (16 == ddpf.RGBBitCount) {
                    if (ISBITMASK(0x0000ffff, 0x00000000, 0x00000000, 0x00000000)) {
                        return DXGI_FORMAT_R16_UNORM; // D3DX10/11 writes this out as DX10 extension
                    }
                    if (ISBITMASK(0x000000ff, 0x00000000, 0x00000000, 0x0000ff00)) {
                        return DXGI_FORMAT_R8G8_UNORM; // D3DX10/11 writes this out as DX10 extension
                    }
                }
            } else if (ddpf.flags & DDPF_ALPHA) {
                if (8 == ddpf.RGBBitCount) {
                    return DXGI_FORMAT_A8_UNORM;
                }
            } else if (ddpf.flags & DDPF_FOURCC) {
                if (compareFourCC(ddpf.fourCC, 'D', 'X', 'T', '1')) {
                    return DXGI_FORMAT_BC1_UNORM;
                }
                if (compareFourCC(ddpf.fourCC, 'D', 'X', 'T', '3')) {
                    return DXGI_FORMAT_BC2_UNORM;
                }
                if (compareFourCC(ddpf.fourCC, 'D', 'X', 'T', '5')) {
                    return DXGI_FORMAT_BC3_UNORM;
                }

                // While pre-mulitplied alpha isn't directly supported by the DXGI formats,
                // they are basically the same as these BC formats so they can be mapped
                if (compareFourCC(ddpf.fourCC, 'D', 'X', 'T', '2')) {
                    return DXGI_FORMAT_BC2_UNORM;
                }
                if (compareFourCC(ddpf.fourCC, 'D', 'X', 'T', '4')) {
                    return DXGI_FORMAT_BC3_UNORM;
                }

                if (compareFourCC(ddpf.fourCC, 'A', 'T', 'I', '1')) {
                    return DXGI_FORMAT_BC4_UNORM;
                }
                if (compareFourCC(ddpf.fourCC, 'B', 'C', '4', 'U')) {
                    return DXGI_FORMAT_BC4_UNORM;
                }
                if (compareFourCC(ddpf.fourCC, 'B', 'C', '4', 'S')) {
                    return DXGI_FORMAT_BC4_SNORM;
                }

                if (compareFourCC(ddpf.fourCC, 'A', 'T', 'I', '2')) {
                    return DXGI_FORMAT_BC5_UNORM;
                }
                if (compareFourCC(ddpf.fourCC, 'B', 'C', '5', 'U')) {
                    return DXGI_FORMAT_BC5_UNORM;
                }
                if (compareFourCC(ddpf.fourCC, 'B', 'C', '5', 'S')) {
                    return DXGI_FORMAT_BC5_SNORM;
                }

                // BC6H and BC7 are written using the "DX10" extended header

                if (compareFourCC(ddpf.fourCC, 'R', 'G', 'B', 'G')) {
                    return DXGI_FORMAT_R8G8_B8G8_UNORM;
                }
                if (compareFourCC(ddpf.fourCC, 'G', 'R', 'G', 'B')) {
                    return DXGI_FORMAT_G8R8_G8B8_UNORM;
                }

                if (compareFourCC(ddpf.fourCC, 'Y', 'U', 'Y', '2')) {
                    return DXGI_FORMAT_YUY2;
                }

                // Check for D3DFORMAT enums being set here
                switch (ddpf.fourCC) {
                case 36: // D3DFMT_A16B16G16R16
                    return DXGI_FORMAT_R16G16B16A16_UNORM;

                case 110: // D3DFMT_Q16W16V16U16
                    return DXGI_FORMAT_R16G16B16A16_SNORM;

                case 111: // D3DFMT_R16F
                    return DXGI_FORMAT_R16_FLOAT;

                case 112: // D3DFMT_G16R16F
                    return DXGI_FORMAT_R16G16_FLOAT;

                case 113: // D3DFMT_A16B16G16R16F
                    return DXGI_FORMAT_R16G16B16A16_FLOAT;

                case 114: // D3DFMT_R32F
                    return DXGI_FORMAT_R32_FLOAT;

                case 115: // D3DFMT_G32R32F
                    return DXGI_FORMAT_R32G32_FLOAT;

                case 116: // D3DFMT_A32B32G32R32F
                    return DXGI_FORMAT_R32G32B32A32_FLOAT;
                }
            }

            return DXGI_FORMAT_UNKNOWN;
        }
    }
}