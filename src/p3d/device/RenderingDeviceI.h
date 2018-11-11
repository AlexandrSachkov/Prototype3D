#pragma once

#ifdef P3D_API_D3D11
#include "d3d11/d3d11_RenderingDevice.h"
namespace p3d {
    typedef RenderingDevice<d3d11::RenderingDevice> RenderingDeviceI;
}

#endif
