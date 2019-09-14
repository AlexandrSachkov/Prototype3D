#pragma once

#include "../ResourceDescriptions.h"
#include <string>

namespace p3d {
    namespace util {
        enum class TEX_BLOCK_COMPRESSION {
            NONE,
            BC1,
            BC2,
            BC3,
            BC4,
            BC5,
            BC6,
            BC7
        };

        // Merges two textures into one where the first texture is used for color and the second texture is used for alpha
        bool importTexture(
            const std::string& colorTexPath, 
            const std::string& alphaTexPath, 
            TEX_BLOCK_COMPRESSION compression,
            bool generateMipMaps,
            TextureDesc& out
        );

        bool importTexture(
            const std::string& texPath,
            TEX_BLOCK_COMPRESSION compression,
            bool generateMipMaps,
            TextureDesc& out
        );
    }
}