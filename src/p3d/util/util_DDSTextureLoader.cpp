#include "util_DDSTextureLoader.h"
#include "../assert.h"

#include <fstream>

namespace p3d {
    namespace util {
        bool DDSTextureLoader::loadDDSTextureFromFile(
            const std::string& path
        ) {
            DDSHeader ddsHeader;
            DDSHeaderDxt10 ddsHeaderDxt10;
            bool dxt10 = false;
            void* data = nullptr;
            unsigned int dataSize = 0;

            std::memset(&ddsHeader, 0, sizeof(DDSHeader));
            std::memset(&ddsHeaderDxt10, 0, sizeof(DDSHeaderDxt10));

            P3D_ASSERT_R(readDDSFile(path, ddsHeader, ddsHeaderDxt10, dxt10, data, dataSize), L"Failed to read DDS file");
            P3D_ASSERT_R(loadDDSData(ddsHeader, ddsHeaderDxt10, dxt10, data, dataSize), L"Failed to load DDS data");
            return true;
        }

        bool DDSTextureLoader::readDDSFile(
            const std::string& path,
            DDSHeader& ddsHeader,
            DDSHeaderDxt10& ddsHeaderDxt10,
            bool& dxt10,
            void*& data,
            unsigned int& dataSize) {

            std::ifstream ifs(path, std::ifstream::in | std::ifstream::binary);
            P3D_ASSERT_R(ifs.good(), "Failed to open file stream");

            ifs.seekg(0, ifs.end);
            const unsigned int fileSizeBytes = (unsigned int)ifs.tellg();
            P3D_ASSERT_R(fileSizeBytes >= BASIC_HEADER_SIZE_BYTES, "Invalid header");
            ifs.seekg(0, ifs.beg);

            char magic[MAGIC_SIZE_BYTES];
            ifs.read(magic, MAGIC_SIZE_BYTES);
            P3D_ASSERT_R(magic[0] == 'D' && magic[1] == 'D' && magic[2] == 'S', "Not a DDS file");

            ifs.read(reinterpret_cast<char*>(&ddsHeader), sizeof(DDSHeader));

            dxt10 = false;
            const char* fourCC = ddsHeader.ddsPixelFormat.fourCC;
            if (ddsHeader.ddsPixelFormat.flags == DDPF_FOURCC
                && fourCC[0] == 'D' && fourCC[1] == 'X' && fourCC[2] == '1' && fourCC[3] == '0') {
                P3D_ASSERT_R(fileSizeBytes >= DX10_HEADER_SIZE_BYTES, "Invalid header");
                ifs.read(reinterpret_cast<char*>(&ddsHeaderDxt10), sizeof(DDSHeaderDxt10));
                dxt10 = true;
            }

            if (dxt10) {
                dataSize = fileSizeBytes - DX10_HEADER_SIZE_BYTES;
                data = new char[dataSize];
                ifs.read(reinterpret_cast<char*>(data), dataSize);
            } else {
                dataSize = fileSizeBytes - BASIC_HEADER_SIZE_BYTES;
                data = new char[dataSize];
                ifs.read(reinterpret_cast<char*>(data), dataSize);
            }
            
            return true;
        }

        bool DDSTextureLoader::loadDDSData(
            const DDSHeader& ddsHeader,
            const DDSHeaderDxt10& dx10Header,
            bool dtx10,
            const void* data,
            unsigned int dataSize) {

            return true;
        }
    }
}