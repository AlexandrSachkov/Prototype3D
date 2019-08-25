#include "util_SceneGraph.h"

namespace p3d {
    namespace util {
        SceneGraphNode::SceneGraphNode(SceneGraphNodeI* parent) {
            setParent(parent);
        }

        void SceneGraphNode::setParent(SceneGraphNodeI* parent) {
            _parent = parent;
            parent->addChild(this); //this will in turn call applyTransform() on this node
        }

        SceneGraphNodeI* SceneGraphNode::getParent() const {
            return _parent;
        }

        void SceneGraphNode::addChild(SceneGraphNodeI* child) {
            
        }

        void SceneGraphNode::removeChild(SceneGraphNodeI* child) {

        }

        const std::vector<std::unique_ptr<SceneGraphNodeI>>& SceneGraphNode::getChildren() const {
            return _children;
        }

        void SceneGraphNode::clearChildren() {
            _children.clear();
        }

        void SceneGraphNode::setTransform(const glm::mat4x4& transform) {

        }

        const glm::mat4x4& SceneGraphNode::getTransform() const {
            return _transform;
        }

        const glm::mat4x4& SceneGraphNode::getAbsoluteTransform() const {
            return _absoluteTransform;
        }

        void SceneGraphNode::applyTransform(const glm::mat4x4& parentTransform) {
            _absoluteTransform = parentTransform * _transform;
        }
    }
}