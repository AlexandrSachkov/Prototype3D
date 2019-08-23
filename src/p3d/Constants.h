#pragma once

namespace p3d {
    enum P3D_FORMAT_TYPE {
        P3D_FORMAT_TYPE_VECTOR,
        P3D_FORMAT_TYPE_BLOCK_COMPRESSION
    };

    enum P3D_VECTOR_FORMAT {
        P3D_FORMAT_UNKNOWN,
        P3D_FORMAT_R32G32B32A32_TYPELESS,
        P3D_FORMAT_R32G32B32A32_FLOAT,
        P3D_FORMAT_R32G32B32A32_UINT,
        P3D_FORMAT_R32G32B32A32_SINT,
        P3D_FORMAT_R32G32B32_TYPELESS,
        P3D_FORMAT_R32G32B32_FLOAT,
        P3D_FORMAT_R32G32B32_UINT,
        P3D_FORMAT_R32G32B32_SINT,
        P3D_FORMAT_R16G16B16A16_TYPELESS,
        P3D_FORMAT_R16G16B16A16_FLOAT,
        P3D_FORMAT_R16G16B16A16_UNORM,
        P3D_FORMAT_R16G16B16A16_UINT,
        P3D_FORMAT_R16G16B16A16_SNORM,
        P3D_FORMAT_R16G16B16A16_SINT,
        P3D_FORMAT_R32G32_TYPELESS,
        P3D_FORMAT_R32G32_FLOAT,
        P3D_FORMAT_R32G32_UINT,
        P3D_FORMAT_R32G32_SINT,
        P3D_FORMAT_R32G8X24_TYPELESS,
        P3D_FORMAT_D32_FLOAT_S8X24_UINT,
        P3D_FORMAT_R32_FLOAT_X8X24_TYPELESS,
        P3D_FORMAT_X32_TYPELESS_G8X24_UINT,
        P3D_FORMAT_R10G10B10A2_TYPELESS,
        P3D_FORMAT_R10G10B10A2_UNORM,
        P3D_FORMAT_R10G10B10A2_UINT,
        P3D_FORMAT_R11G11B10_FLOAT,
        P3D_FORMAT_R8G8B8A8_TYPELESS,
        P3D_FORMAT_R8G8B8A8_UNORM,
        P3D_FORMAT_R8G8B8A8_UNORM_SRGB,
        P3D_FORMAT_R8G8B8A8_UINT,
        P3D_FORMAT_R8G8B8A8_SNORM,
        P3D_FORMAT_R8G8B8A8_SINT,
        P3D_FORMAT_R16G16_TYPELESS,
        P3D_FORMAT_R16G16_FLOAT,
        P3D_FORMAT_R16G16_UNORM,
        P3D_FORMAT_R16G16_UINT,
        P3D_FORMAT_R16G16_SNORM,
        P3D_FORMAT_R16G16_SINT,
        P3D_FORMAT_R32_TYPELESS,
        P3D_FORMAT_D32_FLOAT,
        P3D_FORMAT_R32_FLOAT,
        P3D_FORMAT_R32_UINT,
        P3D_FORMAT_R32_SINT,
        P3D_FORMAT_R24G8_TYPELESS,
        P3D_FORMAT_D24_UNORM_S8_UINT,
        P3D_FORMAT_R24_UNORM_X8_TYPELESS,
        P3D_FORMAT_X24_TYPELESS_G8_UINT,
        P3D_FORMAT_R8G8_TYPELESS,
        P3D_FORMAT_R8G8_UNORM,
        P3D_FORMAT_R8G8_UINT,
        P3D_FORMAT_R8G8_SNORM,
        P3D_FORMAT_R8G8_SINT,
        P3D_FORMAT_R16_TYPELESS,
        P3D_FORMAT_R16_FLOAT,
        P3D_FORMAT_D16_UNORM,
        P3D_FORMAT_R16_UNORM,
        P3D_FORMAT_R16_UINT,
        P3D_FORMAT_R16_SNORM,
        P3D_FORMAT_R16_SINT,
        P3D_FORMAT_R8_TYPELESS,
        P3D_FORMAT_R8_UNORM,
        P3D_FORMAT_R8_UINT,
        P3D_FORMAT_R8_SNORM,
        P3D_FORMAT_R8_SINT,
        P3D_FORMAT_A8_UNORM,
        P3D_FORMAT_R1_UNORM,
        P3D_FORMAT_B8G8R8A8_UNORM,
        P3D_FORMAT_B8G8R8X8_UNORM,
        P3D_FORMAT_B8G8R8A8_TYPELESS,
        P3D_FORMAT_B8G8R8A8_UNORM_SRGB,
        P3D_FORMAT_B8G8R8X8_TYPELESS,
        P3D_FORMAT_B8G8R8X8_UNORM_SRGB
    };

    enum P3D_BLOCK_COMPRESSION_FORMAT {
        P3D_FORMAT_BC1_TYPELESS,
        P3D_FORMAT_BC1_UNORM,
        P3D_FORMAT_BC1_UNORM_SRGB,
        P3D_FORMAT_BC2_TYPELESS,
        P3D_FORMAT_BC2_UNORM,
        P3D_FORMAT_BC2_UNORM_SRGB,
        P3D_FORMAT_BC3_TYPELESS,
        P3D_FORMAT_BC3_UNORM,
        P3D_FORMAT_BC3_UNORM_SRGB,
        P3D_FORMAT_BC4_TYPELESS,
        P3D_FORMAT_BC4_UNORM,
        P3D_FORMAT_BC4_SNORM,
        P3D_FORMAT_BC5_TYPELESS,
        P3D_FORMAT_BC5_UNORM,
        P3D_FORMAT_BC5_SNORM,
        P3D_FORMAT_BC6H_TYPELESS,
        P3D_FORMAT_BC6H_UF16,
        P3D_FORMAT_BC6H_SF16,
        P3D_FORMAT_BC7_TYPELESS,
        P3D_FORMAT_BC7_UNORM,
        P3D_FORMAT_BC7_UNORM_SRGB
    };

    struct Format {
        P3D_FORMAT_TYPE type;
        union {
            P3D_VECTOR_FORMAT vecFormat;
            P3D_BLOCK_COMPRESSION_FORMAT bcFormat;
        } format;
    };

    enum P3D_USAGE {
        // Used for GPU input that will never change
        // D3D11: USAGE_IMMUTABLE
        // OGL: DRAW/STATIC
        P3D_USAGE_GPU_R,

        // Used for GPU operations when occasional CPU update is required
        // D3D11: D3D11_USAGE_DEFAULT
        // OGL: DRAW/DYNAMIC
        P3D_USAGE_CPU_UPDATE_GPU_RW,

        // Used when frequent CPU updates are required
        // D3D11: D3D11_USAGE_DYNAMIC/D3D11_CPU_ACCESS_WRITE
        // OGL: DRAW/STREAM
        P3D_USAGE_CPU_W_GPU_R,

        // Used when data transfer from GPU to CPU is required
        // D3D11: D3D11_USAGE_STAGING/D3D11_CPU_ACCESS_READ
        // OGL: READ
        P3D_USAGE_CPU_R_GPU_UPDATE
    };

    enum P3D_BIND_FLAG {
        P3D_BIND_VERTEX_BUFFER,
        P3D_BIND_INDEX_BUFFER,
        P3D_BIND_CONSTANT_BUFFER,
        P3D_BIND_SHADER_RESOURCE,
        P3D_BIND_STREAM_OUTPUT,
        P3D_BIND_RENDER_TARGET,
        P3D_BIND_DEPTH_STENCIL,
        P3D_BIND_UNORDERED_ACCESS
    };

    enum P3D_INPUT_CLASSIFICATION {
        P3D_INPUT_PER_VERTEX,
        P3D_INPUT_PER_INSTANCE
    };

    enum P3D_FILL_MODE {
        P3D_FILL_SOLID,
        P3D_FILL_WIREFRAME
    };

    enum P3D_CULL_MODE {
        P3D_CULL_NONE,
        P3D_CULL_FRONT,
        P3D_CULL_BACK
    };

    enum P3D_BLEND_MODE {
        P3D_BLEND_ADD,
        P3D_BLEND_SUBTRACT,
        P3D_BLEND_MULTIPLY,
        P3D_BLEND_DIVIDE,
        P3D_BLEND_SMOOTH_ADD,
        P3D_BLEND_SIGNED_ADD
    };

    enum TEX_MAP_MODE {
        TEX_MAP_WRAP,
        TEX_MAP_CLAMP,
        TEX_MAP_DECAL,
        TEX_MAP_MIRROR
    };

    enum P3D_PRIMITIVE_TOPOLOGY {
        P3D_PRIMITIVE_TOPOLOGY_UNDEFINED,
        P3D_PRIMITIVE_TOPOLOGY_POINTLIST,
        P3D_PRIMITIVE_TOPOLOGY_LINELIST,
        P3D_PRIMITIVE_TOPOLOGY_LINESTRIP,
        P3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        P3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
        P3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ,
        P3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ,
        P3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ,
        P3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ
    };

    unsigned int getVecFormatSizeBits(P3D_VECTOR_FORMAT format);
    unsigned int getBlockCompressionFormatSizeBytes(P3D_BLOCK_COMPRESSION_FORMAT format);
}