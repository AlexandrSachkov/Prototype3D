#include "StandardShapes.h"

namespace p3d {
    namespace shapes {
        MeshDesc createCube() {
            MeshDesc desc;
            desc.topology = P3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
            desc.verticesSize = 8;
            desc.vertices.reset(new glm::vec3[desc.verticesSize]{
                {0.5,0.5,0.5},
                {-0.5,0.5,0.5},
                {-0.5,-0.5,0.5},
                {0.5,-0.5,0.5},
                {0.5,0.5,-0.5},
                {0.5,-0.5,-0.5},
                {-0.5,0.5,-0.5},
                {-0.5,-0.5,-0.5}
            });

            desc.indicesSize = 36;
            desc.indices.reset(new unsigned int[desc.indicesSize]{
               0,1,2, 0,2,3,
               4,0,3, 4,3,5,
               4,6,1, 4,1,0,
               6,7,2, 1,6,2,
               2,7,5, 3,2,5,
               4,5,7, 6,4,7
            });

            return desc;
        }
    }
}