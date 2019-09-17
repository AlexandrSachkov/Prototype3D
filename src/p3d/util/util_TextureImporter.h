#pragma once

#include "../ResourceDescriptions.h"
#include <string>
#include <functional>

struct FIBITMAP;

namespace p3d {
    namespace util {
        

        class TextureImporter {
        public:
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
            static bool importTexture(
                const std::string& colorTexPath,
                const std::string& alphaTexPath,
                TEX_BLOCK_COMPRESSION compression,
                bool generateMipMaps,
                TextureDesc& out
            );

            static bool importTexture(
                const std::string& texPath,
                TEX_BLOCK_COMPRESSION compression,
                bool generateMipMaps,
                TextureDesc& out
            );

        private:
            typedef std::unique_ptr<FIBITMAP, std::function<void(FIBITMAP*)>> BitmapPtr;
            static std::unique_ptr<FIBITMAP, std::function<void(FIBITMAP*)>> createBitmapPtr(FIBITMAP*);
        };
        
    }
}