#pragma once

#include "../../Constants.h"
#include <dxgiformat.h>

namespace p3d {
    namespace dx {
        DXGI_FORMAT convertFormat(P3D_FORMAT format);
    }
}