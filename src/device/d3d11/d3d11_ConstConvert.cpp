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
    }
}