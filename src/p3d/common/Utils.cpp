#include "Utils.h"
#include "../assert.h"

#include <fstream>

namespace p3d {
    namespace util {
        bool readFile(const std::string& path, std::string& data) {
            std::ifstream ifs(path, std::ios::binary);
            P3D_ASSERT_R(ifs.good(), "Failed to open file");

            ifs.seekg(0, ifs.end);
            size_t size = ifs.tellg();
            ifs.seekg(0, ifs.beg);

            data.resize(size);
            ifs.read(&data[0], size);

            P3D_ASSERT_R(ifs.good(), "Failed to read file");
            return true;
        }
    }
}