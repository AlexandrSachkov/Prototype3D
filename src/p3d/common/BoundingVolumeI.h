#pragma once

namespace p3d {
    class BoundingVolumeI {
    public:
        virtual ~BoundingVolumeI() {}
        virtual bool intersects(const BoundingVolumeI* other) const = 0;
        virtual float getShortestDistance() const = 0;
    };
}
