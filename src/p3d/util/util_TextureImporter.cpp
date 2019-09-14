#include "util_TextureImporter.h"

#include "FreeImage.h"

namespace p3d {
    namespace util {
        bool importTexture(
            const std::string& colorTexPath,
            const std::string& alphaTexPath,
            TEX_BLOCK_COMPRESSION compression,
            bool generateMipMaps,
            TextureDesc& out
        ) {
            return false;
        }

        bool importTexture(
            const std::string& texPath,
            TEX_BLOCK_COMPRESSION compression,
            bool generateMipMaps,
            TextureDesc& out
        ) {
            return false;
        }
    }
}