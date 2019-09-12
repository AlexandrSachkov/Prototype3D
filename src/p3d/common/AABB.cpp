#include "AABB.h"
#include <limits>
#include <glm/gtc/matrix_transform.hpp>

namespace p3d {
    AABB::AABB(float minX, float maxX, float minY, float maxY, float minZ, float maxZ) 
    : _minX(minX), _maxX(maxX), _minY(minY), _maxY(maxY), _minZ(minZ), _maxZ(maxZ){

    }

    float AABB::getMinX() const {
        return _minX;
    }

    float AABB::getMaxX() const {
        return _maxX;
    }

    float AABB::getMinY() const {
        return _minY;
    }

    float AABB::getMaxY() const {
        return _maxY;
    }

    float AABB::getMinZ() const {
        return _minZ;
    }

    float AABB::getMaxZ() const {
        return _maxZ;
    }

    glm::vec3 AABB::getCenter() const {
        float deltaX = glm::abs(getMaxX() - getMinX());
        float deltaY = glm::abs(getMaxY() - getMinY());
        float deltaZ = glm::abs(getMaxZ() - getMinZ());

        return {
            getMinX() + deltaX / 2,
            getMinY() + deltaY / 2,
            getMinZ() + deltaZ / 2
        };
    }

    AABB AABB::build(const glm::vec3* vertices, unsigned int numVertices, const glm::mat4x4& worldTransform) {
        float minX, minY, minZ, maxX, maxY, maxZ;
        minX = minY = minZ = FLT_MAX;
        maxX = maxY = maxZ =  -FLT_MAX;

        glm::vec4 vertexFinal;
        for (unsigned int i = 0; i < numVertices; i++) {
            vertexFinal = worldTransform * glm::vec4(vertices[i], 1.0f);
            if (vertexFinal.x <= minX) {
                minX = vertexFinal.x;
            }
            if (vertexFinal.x >= maxX) {
                maxX = vertexFinal.x;
            }

            if (vertexFinal.y <= minY) {
                minY = vertexFinal.y;
            }
            if (vertexFinal.y >= maxY) {
                maxY = vertexFinal.y;
            }

            if (vertexFinal.z <= minZ) {
                minZ = vertexFinal.z;
            }
            if (vertexFinal.z >= maxZ) {
                maxZ = vertexFinal.z;
            }
        }

        return AABB(minX, maxX, minY, maxY, minZ, maxZ);
    }
}