#pragma once

#include <vector>
#include <memory>
#include "../ResourceHandles.h"
#include "../common/Uuid.h"

namespace p3d {
    template <typename Res, typename Desc>
    class ResourceBank {
    public:
        HResource insert(std::unique_ptr<Res>& res, const Desc& desc, UUID uuid) {
            for (unsigned int i = 0; i < _uuids.size(); i++) {
                if (_uuids[i] == 0) {
                    _resources[i] = std::move(res);
                    _descriptions[i] = desc;
                    _uuids[i] = uuid;
                    return HResource(i, uuid);
                }
            }

            try {
                _resources.emplace_back(std::move(res));
                _descriptions.emplace_back(desc);
                _uuids.emplace_back(uuid);
            } catch (std::bad_alloc&) {
                return HResource();
            }
            return HResource((unsigned int)(_uuids.size() - 1), uuid);
        }

        const Res* getResource(HResource handle) const {
            if (handle.uuid != _uuids[handle.buffPosition]) {
                return false;
            }

            return _resources[handle.buffPosition].get();
        }

        const Desc* getDesc(HResource handle) const {
            if (handle.uuid != _uuids[handle.buffPosition]) {
                return false;
            }

            return &_descriptions[handle.buffPosition];
        }

        bool update(HResource handle, std::unique_ptr<Res>& res, const Desc& desc) {
            if (handle.uuid != _uuids[handle.buffPosition]) {
                return false;
            }

            _resources[handle.buffPosition] = std::move(res);
            _descriptions[handle.buffPosition] = desc;
            return true;
        }

        bool remove(HResource handle) {
            if (handle.uuid != _uuids[handle.buffPosition]) {
                return false;
            }

            _resources[handle.buffPosition].release();
            _uuids[handle.buffPosition] = 0;
            return true;
        }
    private:
        std::vector<std::unique_ptr<Res>> _resources;
        std::vector<Desc> _descriptions;
        std::vector<UUID> _uuids;
    };
}