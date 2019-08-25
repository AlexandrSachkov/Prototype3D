#pragma once

#include "util_SceneGraphI.h"
#include "../ResourceHandles.h"

#include "glm/mat4x4.hpp"

namespace p3d {
    class SceneI;
    namespace util {
        class SceneGraphNode : public SceneGraphNodeI {
        public:
            SceneGraphNode(SceneGraphNodeI* parent);

            void setParent(SceneGraphNodeI* parent) override;
            SceneGraphNodeI* getParent() const override;

            void addChild(SceneGraphNodeI* child) override;
            void removeChild(SceneGraphNodeI* child) override;
            const std::vector<std::unique_ptr<SceneGraphNodeI>>& getChildren() const override;
            void clearChildren() override;

            void setTransform(const glm::mat4x4& transform) override;
            const glm::mat4x4& getTransform() const override;
            const glm::mat4x4& getAbsoluteTransform() const override;
        private:
            void applyTransform(const glm::mat4x4& parentTransform);

            glm::mat4x4 _transform;
            glm::mat4x4 _absoluteTransform;
            SceneGraphNodeI* _parent;
            std::vector<std::unique_ptr<SceneGraphNodeI>> _children;
        };

        class SceneGraph : public SceneGraphI {
        public:

        private:
        };
    }
} 