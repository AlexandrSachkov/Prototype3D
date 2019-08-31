#pragma once

#include <vector>
#include <memory>
#include "../ResourceHandles.h"
#include "../common/Uuid.h"

namespace p3d {
    template <typename Res, typename Desc, typename H>
    class ResourceBank {
    public:
        H insert(std::unique_ptr<Res>& res, const Desc& desc, UUID uuid) {
            for (unsigned int i = 0; i < _handles.size(); i++) {
                if (_handles[i].getUUID() == 0) {
                    _resources[i] = std::move(res);
                    _descriptions[i] = desc;
                    _handles[i] = H(i, uuid);
                    return _handles[i];
                }
            }

            try {
                _resources.emplace_back(std::move(res));
                _descriptions.emplace_back(desc);
                _handles.emplace_back(H((unsigned int)(_resources.size() - 1), uuid));
            } catch (std::bad_alloc&) {
                return H();
            }
            return _handles[_handles.size() - 1];
        }

        const Res* getResource(H handle) const {
            if (handle.getBuffPosition() >= _handles.size() ||
                handle.getUUID() != _handles[handle.getBuffPosition()].getUUID()) {
                return false;
            }

            return _resources[handle.getBuffPosition()].get();
        }

        const Desc* getDesc(H handle) const {
            if (handle.getBuffPosition() >= _handles.size() ||
                handle.getUUID() != _handles[handle.getBuffPosition()].getUUID()) {
                return false;
            }

            return &_descriptions[handle.getBuffPosition()];
        }

        const std::vector<H>& getAllHandles() const {
            return _handles;
        }

        bool update(H handle, std::unique_ptr<Res>& res, const Desc& desc) {
            if (handle.getBuffPosition() >= _handles.size() ||
                handle.getUUID() != _handles[handle.getBuffPosition()].getUUID()) {
                return false;
            }

            _resources[handle.getBuffPosition()] = std::move(res);
            _descriptions[handle.getBuffPosition()] = desc;
            return true;
        }

        bool remove(H handle) {
            if (handle.getBuffPosition() >= _handles.size() ||
                handle.getUUID() != _handles[handle.getBuffPosition()].getUUID()) {
                return false;
            }

            _resources[handle.getBuffPosition()].release();
            _handles[handle.getBuffPosition()] = H();
            return true;
        }
    private:
        std::vector<std::unique_ptr<Res>> _resources;
        std::vector<Desc> _descriptions;
        std::vector<H> _handles;
    };
}