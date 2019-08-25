#include "d3d11_ConstConvert.h"
#include "../../assert.h"

namespace p3d {
    namespace d3d11 {
        bool convertBindFlag(P3D_BIND_FLAG flag, D3D11_BIND_FLAG& d3d11Flag) {
            switch (flag) {
            case P3D_BIND_VERTEX_BUFFER:
                d3d11Flag = D3D11_BIND_VERTEX_BUFFER;
                return true;
            case P3D_BIND_INDEX_BUFFER:
                d3d11Flag = D3D11_BIND_INDEX_BUFFER;
                return true;
            case P3D_BIND_CONSTANT_BUFFER:
                d3d11Flag = D3D11_BIND_CONSTANT_BUFFER;
                return true;
            case P3D_BIND_SHADER_RESOURCE:
                d3d11Flag = D3D11_BIND_SHADER_RESOURCE;
                return true;
            case P3D_BIND_STREAM_OUTPUT:
                d3d11Flag = D3D11_BIND_STREAM_OUTPUT;
                return true;
            case P3D_BIND_RENDER_TARGET:
                d3d11Flag = D3D11_BIND_RENDER_TARGET;
                return true;
            case P3D_BIND_DEPTH_STENCIL:
                d3d11Flag = D3D11_BIND_DEPTH_STENCIL;
                return true;
            case P3D_BIND_UNORDERED_ACCESS:
                d3d11Flag = D3D11_BIND_UNORDERED_ACCESS;
                return true;
            default:
                P3D_ASSERT_R(false, "Unable to convert bind flag to d3d11 equivalent");
            }
        }

        bool convertUsageFlag(P3D_USAGE flag, UsageDesc& usageDesc) {
            switch (flag) {
            case P3D_USAGE_GPU_R:
                usageDesc = { D3D11_USAGE_IMMUTABLE, (D3D11_CPU_ACCESS_FLAG)0 };
                return true;
            case P3D_USAGE_CPU_UPDATE_GPU_RW:
                usageDesc = { D3D11_USAGE_DEFAULT, (D3D11_CPU_ACCESS_FLAG)0 };
                return true;
            case P3D_USAGE_CPU_W_GPU_R:
                usageDesc = { D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE };
                return true;
            case P3D_USAGE_CPU_R_GPU_UPDATE:
                usageDesc = { D3D11_USAGE_STAGING, D3D11_CPU_ACCESS_READ };
                return true;
            default:
                P3D_ASSERT_R(false, "Unable to convert usage flag to d3d11 equivalent");
            }
        }

        bool convertFillMode(P3D_FILL_MODE fill, D3D11_FILL_MODE& d3d11Fill) {
            switch (fill) {
            case P3D_FILL_SOLID:
                d3d11Fill = D3D11_FILL_SOLID;
                return true;
            case P3D_FILL_WIREFRAME:
                d3d11Fill = D3D11_FILL_WIREFRAME;
                return true;
            default:
                P3D_ASSERT_R(false, "Unable to convert fill mode to d3d11 equivalent");
            }
        }

        bool convertCullMode(P3D_CULL_MODE cull, D3D11_CULL_MODE& d3d11Cull) {
            switch (cull) {
            case P3D_CULL_NONE:
                d3d11Cull = D3D11_CULL_NONE;
                return true;
            case P3D_CULL_BACK:
                d3d11Cull = D3D11_CULL_BACK;
                return true;
            case P3D_CULL_FRONT:
                d3d11Cull = D3D11_CULL_FRONT;
                return true;
            default:
                P3D_ASSERT_R(false, "Unable to convert cull mode to d3d11 equivalent");
            }
        }

        D3D11_PRIMITIVE_TOPOLOGY convertPrimitiveTopology(P3D_PRIMITIVE_TOPOLOGY tp) {
            switch (tp) {
            case P3D_PRIMITIVE_TOPOLOGY_UNDEFINED:
                return D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
            case P3D_PRIMITIVE_TOPOLOGY_POINTLIST:
                return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
            case P3D_PRIMITIVE_TOPOLOGY_LINELIST:
                return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
            case P3D_PRIMITIVE_TOPOLOGY_LINESTRIP:
                return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
            case P3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
                return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
            case P3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
                return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
            case P3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ:
                return D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ;
            case P3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ:
                return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ;
            case P3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ:
                return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;
            case P3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ:
                return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ;
            default:
                return D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
            }
        }

        bool convertTextureMapMode(P3D_TEX_MAP_MODE mode, D3D11_TEXTURE_ADDRESS_MODE& d3d11MapMode) {
            switch (mode) {
            case P3D_TEX_MAP_WRAP:
                d3d11MapMode = D3D11_TEXTURE_ADDRESS_WRAP;
                return true;
            case P3D_TEX_MAP_CLAMP:
                d3d11MapMode = D3D11_TEXTURE_ADDRESS_CLAMP;
                return true;
            case P3D_TEX_MAP_MIRROR:
                d3d11MapMode = D3D11_TEXTURE_ADDRESS_MIRROR;
                return true;
            default:
                return false;
            }
        }
    }
}