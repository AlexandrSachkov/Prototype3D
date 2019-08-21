#pragma once

#include <vector>
#include "../ResourceHandles.h"

namespace p3d {
    template <typename Res, typename Desc>
    class ResourceBank {
    public:
        bool insert(const Res& res, const Desc& desc, unsigned long long uuid) {
            for (size_t i = 0; i < _uuids.size(); i++) {
                if (_uuids[i] == 0) {
                    _resources[i] = res;
                    _descriptions[i] = desc;
                    _uuids[i] = uuid;
                    return true;
                }
            }

            try {
                _resources.emplace_back(res);
                _descriptions.emplace_back(desc);
                _uuids.emplace_back(uuid);
            } catch (std::bad_alloc& e) {
                return false;
            }
            return true;
        }

        const Res* getResource(HResource handle) {
            if (handle.uuid != _uuids[handle.buffPosition]) {
                return false;
            }

            return &_resources[handle.buffPosition];
        }

        const Desc* getDesc(HResource handle) {
            if (handle.uuid != _uuids[handle.buffPosition]) {
                return false;
            }

            return &_descriptions[handle.buffPosition];
        }

        bool update(HResource handle, const Res& res, const Desc& desc) {
            if (handle.uuid != _uuids[handle.buffPosition]) {
                return false;
            }

            _resources[handle.buffPosition] = res;
            _descriptions[handle.buffPosition] = desc;
        }

        bool remove(HResource handle) {
            if (handle.uuid != _uuids[handle.buffPosition]) {
                return false;
            }

            _uuids[handle.buffPosition] = 0;
        }
    private:
        std::vector<Res> _resources;
        std::vector<Desc> _descriptions;
        std::vector<unsigned long long> _uuids;
    };
}