#pragma once

#include <vector>
#include <memory>

#include "glm/mat4x4.hpp"

namespace p3d {
    namespace util {
        class SceneGraphNodeI {
        public:
            virtual ~SceneGraphNodeI() {}

            virtual void setParent(SceneGraphNodeI* parent) = 0;
            virtual SceneGraphNodeI* getParent() const = 0;

            virtual void addChild(SceneGraphNodeI* child) = 0;
            virtual void removeChild(SceneGraphNodeI* child) = 0;
            virtual const std::vector<std::unique_ptr<SceneGraphNodeI>>& getChildren() const = 0;
            virtual void clearChildren() = 0;

            virtual void setTransform(const glm::mat4x4& transform) = 0;
            virtual const glm::mat4x4& getTransform() const = 0;
            virtual const glm::mat4x4& getAbsoluteTransform() const = 0;
        };

        class SceneGraphI {
        public:
            virtual ~SceneGraphI() {}
        };
    }
}