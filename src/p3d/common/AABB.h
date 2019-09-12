#pragma once

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

namespace p3d {
    class AABB {
    public:
        static AABB build(const glm::vec3* vertices, unsigned int numVertices, const glm::mat4x4& worldTransform);

        AABB() = default;
        AABB(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);

        float getMinX() const;
        float getMaxX() const;
        float getMinY() const;
        float getMaxY() const;
        float getMinZ() const;
        float getMaxZ() const;

        glm::vec3 getCenter() const;

    private:
        float _minX = 0;
        float _maxX = 0;
        float _minY = 0;
        float _maxY = 0;
        float _minZ = 0;
        float _maxZ = 0;
    };
}