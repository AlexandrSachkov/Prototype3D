#include "AABB.h"
#include <limits>

namespace p3d {
    AABB::AABB(float minX, float maxX, float minY, float maxY, float minZ, float maxZ) 
    : _minX(minX), _maxX(maxX), _minY(minY), _maxY(maxY), _minZ(minZ), _maxZ(maxZ){

    }

    float AABB::getMinX() {
        return _minX;
    }

    float AABB::getMaxX() {
        return _maxX;
    }

    float AABB::getMinY() {
        return _minY;
    }

    float AABB::getMaxY() {
        return _maxY;
    }

    float AABB::getMinZ() {
        return _minZ;
    }

    float AABB::getMaxZ() {
        return _maxZ;
    }

    AABB AABB::build(const glm::vec3* vertices, unsigned int numVertices, const glm::mat4x4& worldTransform) {
        float minX, minY, minZ, maxX, maxY, maxZ;
        minX = minY = minZ = FLT_MAX;
        maxX = maxY = maxZ = FLT_MIN;

        glm::vec4 vertexFinal;
        for (unsigned int i = 0; i < numVertices; i++) {
            vertexFinal = worldTransform * glm::vec4(vertices[i], 1.0f);
            if (vertexFinal.x <= minX) {
                minX = vertexFinal.x;
            } else if (vertexFinal.x >= maxX) {
                maxX = vertexFinal.x;
            }

            if (vertexFinal.y <= minY) {
                minY = vertexFinal.y;
            } else if (vertexFinal.y >= maxY) {
                maxY = vertexFinal.y;
            }

            if (vertexFinal.z <= minZ) {
                minZ = vertexFinal.z;
            } else if (vertexFinal.z >= maxZ) {
                maxZ = vertexFinal.z;
            }
        }

        return AABB(minX, maxX, minY, maxY, minZ, maxZ);
    }
}