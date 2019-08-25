#pragma once

#include "Texture1dArrayI.h"
#include "Texture2dArrayI.h"
#include "Texture3dI.h"
#include "VertexShaderI.h"
#include "PixelShaderI.h"
#include "../ResourceDescriptions.h"
#include "../scene/SceneI.h"
#include "../scene/CameraI.h"
#include "GPUResourceProviderI.h"

#include <memory>

namespace p3d {
    class RendererI : public GPUResourceProviderI {
    public:
        virtual void renderFrame() = 0;
        virtual void renderFrame(const SceneI* scene, const CameraI* camera) = 0;
    };
}
