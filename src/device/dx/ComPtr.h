#pragma once
#include <wrl/client.h>

namespace p3d {
    template<class T>
    using ComPtr = Microsoft::WRL::ComPtr<T>;
}
    