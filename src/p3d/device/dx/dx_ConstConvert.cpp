#include "dx_ConstConvert.h"

namespace p3d {
    namespace dx {
        DXGI_FORMAT convertVectorFormat(P3D_VECTOR_FORMAT format) {
            switch (format) {
            case P3D_FORMAT_UNKNOWN:
                return DXGI_FORMAT_UNKNOWN;
            case P3D_FORMAT_R32G32B32A32_TYPELESS:
                return DXGI_FORMAT_R32G32B32A32_TYPELESS;
            case P3D_FORMAT_R32G32B32A32_FLOAT:
                return DXGI_FORMAT_R32G32B32A32_FLOAT;
            case P3D_FORMAT_R32G32B32A32_UINT:
                return DXGI_FORMAT_R32G32B32A32_UINT;
            case P3D_FORMAT_R32G32B32A32_SINT:
                return DXGI_FORMAT_R32G32B32A32_SINT;
            case P3D_FORMAT_R32G32B32_TYPELESS:
                return DXGI_FORMAT_R32G32B32_TYPELESS;
            case P3D_FORMAT_R32G32B32_FLOAT:
                return DXGI_FORMAT_R32G32B32_FLOAT;
            case P3D_FORMAT_R32G32B32_UINT:
                return DXGI_FORMAT_R32G32B32_UINT;
            case P3D_FORMAT_R32G32B32_SINT:
                return DXGI_FORMAT_R32G32B32_SINT;
            case P3D_FORMAT_R16G16B16A16_TYPELESS:
                return DXGI_FORMAT_R16G16B16A16_TYPELESS;
            case P3D_FORMAT_R16G16B16A16_FLOAT:
                return DXGI_FORMAT_R16G16B16A16_FLOAT;
            case P3D_FORMAT_R16G16B16A16_UNORM:
                return DXGI_FORMAT_R16G16B16A16_UNORM;
            case P3D_FORMAT_R16G16B16A16_UINT:
                return DXGI_FORMAT_R16G16B16A16_UINT;
            case P3D_FORMAT_R16G16B16A16_SNORM:
                return DXGI_FORMAT_R16G16B16A16_SNORM;
            case P3D_FORMAT_R16G16B16A16_SINT:
                return DXGI_FORMAT_R16G16B16A16_SINT;
            case P3D_FORMAT_R32G32_TYPELESS:
                return DXGI_FORMAT_R32G32_TYPELESS;
            case P3D_FORMAT_R32G32_FLOAT:
                return DXGI_FORMAT_R32G32_FLOAT;
            case P3D_FORMAT_R32G32_UINT:
                return DXGI_FORMAT_R32G32_UINT;
            case P3D_FORMAT_R32G32_SINT:
                return DXGI_FORMAT_R32G32_SINT;
            case P3D_FORMAT_R32G8X24_TYPELESS:
                return DXGI_FORMAT_R32G8X24_TYPELESS;
            case P3D_FORMAT_D32_FLOAT_S8X24_UINT:
                return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
            case P3D_FORMAT_R32_FLOAT_X8X24_TYPELESS:
                return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
            case P3D_FORMAT_X32_TYPELESS_G8X24_UINT:
                return DXGI_FORMAT_X32_TYPELESS_G8X24_UINT;
            case P3D_FORMAT_R10G10B10A2_TYPELESS:
                return DXGI_FORMAT_R10G10B10A2_TYPELESS;
            case P3D_FORMAT_R10G10B10A2_UNORM:
                return DXGI_FORMAT_R10G10B10A2_UNORM;
            case P3D_FORMAT_R10G10B10A2_UINT:
                return DXGI_FORMAT_R10G10B10A2_UINT;
            case P3D_FORMAT_R11G11B10_FLOAT:
                return DXGI_FORMAT_R11G11B10_FLOAT;
            case P3D_FORMAT_R8G8B8A8_TYPELESS:
                return DXGI_FORMAT_R8G8B8A8_TYPELESS;
            case P3D_FORMAT_R8G8B8A8_UNORM:
                return DXGI_FORMAT_R8G8B8A8_UNORM;
            case P3D_FORMAT_R8G8B8A8_UNORM_SRGB:
                return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
            case P3D_FORMAT_R8G8B8A8_UINT:
                return DXGI_FORMAT_R8G8B8A8_UINT;
            case P3D_FORMAT_R8G8B8A8_SNORM:
                return DXGI_FORMAT_R8G8B8A8_SNORM;
            case P3D_FORMAT_R8G8B8A8_SINT:
                return DXGI_FORMAT_R8G8B8A8_SINT;
            case P3D_FORMAT_R16G16_TYPELESS:
                return DXGI_FORMAT_R16G16_TYPELESS;
            case P3D_FORMAT_R16G16_FLOAT:
                return DXGI_FORMAT_R16G16_FLOAT;
            case P3D_FORMAT_R16G16_UNORM:
                return DXGI_FORMAT_R16G16_UNORM;
            case P3D_FORMAT_R16G16_UINT:
                return DXGI_FORMAT_R16G16_UINT;
            case P3D_FORMAT_R16G16_SNORM:
                return DXGI_FORMAT_R16G16_SNORM;
            case P3D_FORMAT_R16G16_SINT:
                return DXGI_FORMAT_R16G16_SINT;
            case P3D_FORMAT_R32_TYPELESS:
                return DXGI_FORMAT_R32_TYPELESS;
            case P3D_FORMAT_D32_FLOAT:
                return DXGI_FORMAT_D32_FLOAT;
            case P3D_FORMAT_R32_FLOAT:
                return DXGI_FORMAT_R32_FLOAT;
            case P3D_FORMAT_R32_UINT:
                return DXGI_FORMAT_R32_UINT;
            case P3D_FORMAT_R32_SINT:
                return DXGI_FORMAT_R32_SINT;
            case P3D_FORMAT_R24G8_TYPELESS:
                return DXGI_FORMAT_R24G8_TYPELESS;
            case P3D_FORMAT_D24_UNORM_S8_UINT:
                return DXGI_FORMAT_D24_UNORM_S8_UINT;
            case P3D_FORMAT_R24_UNORM_X8_TYPELESS:
                return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
            case P3D_FORMAT_X24_TYPELESS_G8_UINT:
                return DXGI_FORMAT_X24_TYPELESS_G8_UINT;
            case P3D_FORMAT_R8G8_TYPELESS:
                return DXGI_FORMAT_R8G8_TYPELESS;
            case P3D_FORMAT_R8G8_UNORM:
                return DXGI_FORMAT_R8G8_UNORM;
            case P3D_FORMAT_R8G8_UINT:
                return DXGI_FORMAT_R8G8_UINT;
            case P3D_FORMAT_R8G8_SNORM:
                return DXGI_FORMAT_R8G8_SNORM;
            case P3D_FORMAT_R8G8_SINT:
                return DXGI_FORMAT_R8G8_SINT;
            case P3D_FORMAT_R16_TYPELESS:
                return DXGI_FORMAT_R16_TYPELESS;
            case P3D_FORMAT_R16_FLOAT:
                return DXGI_FORMAT_R16_FLOAT;
            case P3D_FORMAT_D16_UNORM:
                return DXGI_FORMAT_D16_UNORM;
            case P3D_FORMAT_R16_UNORM:
                return DXGI_FORMAT_R16_UNORM;
            case P3D_FORMAT_R16_UINT:
                return DXGI_FORMAT_R16_UINT;
            case P3D_FORMAT_R16_SNORM:
                return DXGI_FORMAT_R16_SNORM;
            case P3D_FORMAT_R16_SINT:
                return DXGI_FORMAT_R16_SINT;
            case P3D_FORMAT_R8_TYPELESS:
                return DXGI_FORMAT_R8_TYPELESS;
            case P3D_FORMAT_R8_UNORM:
                return DXGI_FORMAT_R8_UNORM;
            case P3D_FORMAT_R8_UINT:
                return DXGI_FORMAT_R8_UINT;
            case P3D_FORMAT_R8_SNORM:
                return DXGI_FORMAT_R8_SNORM;
            case P3D_FORMAT_R8_SINT:
                return DXGI_FORMAT_R8_SINT;
            case P3D_FORMAT_A8_UNORM:
                return DXGI_FORMAT_A8_UNORM;
            case P3D_FORMAT_R1_UNORM:
                return DXGI_FORMAT_R1_UNORM;
            case P3D_FORMAT_B8G8R8A8_UNORM:
                return DXGI_FORMAT_B8G8R8A8_UNORM;
            case P3D_FORMAT_B8G8R8X8_UNORM:
                return DXGI_FORMAT_B8G8R8X8_UNORM;
            case P3D_FORMAT_B8G8R8A8_TYPELESS:
                return DXGI_FORMAT_B8G8R8A8_TYPELESS;
            case P3D_FORMAT_B8G8R8A8_UNORM_SRGB:
                return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
            case P3D_FORMAT_B8G8R8X8_TYPELESS:
                return DXGI_FORMAT_B8G8R8X8_TYPELESS;
            case P3D_FORMAT_B8G8R8X8_UNORM_SRGB:
                return DXGI_FORMAT_B8G8R8X8_UNORM_SRGB;

            default:
                return DXGI_FORMAT_UNKNOWN;
            }
        }

        DXGI_FORMAT convertBlockCompressionFormat(P3D_BLOCK_COMPRESSION_FORMAT format) {
            switch (format) {
            case P3D_FORMAT_BC1_TYPELESS:
                return DXGI_FORMAT_BC1_TYPELESS;
            case P3D_FORMAT_BC1_UNORM:
                return DXGI_FORMAT_BC1_UNORM;
            case P3D_FORMAT_BC1_UNORM_SRGB:
                return DXGI_FORMAT_BC1_UNORM_SRGB;
            case P3D_FORMAT_BC2_TYPELESS:
                return DXGI_FORMAT_BC2_TYPELESS;
            case P3D_FORMAT_BC2_UNORM:
                return DXGI_FORMAT_BC2_UNORM;
            case P3D_FORMAT_BC2_UNORM_SRGB:
                return DXGI_FORMAT_BC2_UNORM_SRGB;
            case P3D_FORMAT_BC3_TYPELESS:
                return DXGI_FORMAT_BC3_TYPELESS;
            case P3D_FORMAT_BC3_UNORM:
                return DXGI_FORMAT_BC3_UNORM;
            case P3D_FORMAT_BC3_UNORM_SRGB:
                return DXGI_FORMAT_BC3_UNORM_SRGB;
            case P3D_FORMAT_BC4_TYPELESS:
                return DXGI_FORMAT_BC4_TYPELESS;
            case P3D_FORMAT_BC4_UNORM:
                return DXGI_FORMAT_BC4_UNORM;
            case P3D_FORMAT_BC4_SNORM:
                return DXGI_FORMAT_BC4_SNORM;
            case P3D_FORMAT_BC5_TYPELESS:
                return DXGI_FORMAT_BC5_TYPELESS;
            case P3D_FORMAT_BC5_UNORM:
                return DXGI_FORMAT_BC5_UNORM;
            case P3D_FORMAT_BC5_SNORM:
                return DXGI_FORMAT_BC5_SNORM;
            case P3D_FORMAT_BC6H_TYPELESS:
                return DXGI_FORMAT_BC6H_TYPELESS;
            case P3D_FORMAT_BC6H_UF16:
                return DXGI_FORMAT_BC6H_UF16;
            case P3D_FORMAT_BC6H_SF16:
                return DXGI_FORMAT_BC6H_SF16;
            case P3D_FORMAT_BC7_TYPELESS:
                return DXGI_FORMAT_BC7_TYPELESS;
            case P3D_FORMAT_BC7_UNORM:
                return DXGI_FORMAT_BC7_UNORM;
            case P3D_FORMAT_BC7_UNORM_SRGB:
                return DXGI_FORMAT_BC7_UNORM_SRGB;
            default:
                return DXGI_FORMAT_UNKNOWN;
            }
        }

        bool convertFormat(Format format, DXGI_FORMAT& dxgiFormat) {
            switch (format.type) {
            case P3D_FORMAT_TYPE_VECTOR:
                dxgiFormat = convertVectorFormat(format.format.vecFormat);
                return true;
            case P3D_FORMAT_TYPE_BLOCK_COMPRESSION:
                dxgiFormat = convertBlockCompressionFormat(format.format.bcFormat);
                return true;
            default:
                return false;
            }
        }
    }
}