#pragma once

#include "BoundingVolumeI.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

namespace p3d {
    class AABB : public BoundingVolumeI {
    public:
        AABB(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);
        bool intersects(const BoundingVolumeI* other) const override;
        float getShortestDistance() const override;

        static AABB build(const glm::vec3* vertices, unsigned int numVertices, const glm::mat4x4& transform);
    private:
        float _minX;
        float _maxX; 
        float _minY; 
        float _maxY; 
        float _minZ; 
        float _maxZ;
    };
}