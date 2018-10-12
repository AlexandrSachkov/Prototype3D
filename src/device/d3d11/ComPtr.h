#pragma once

#include <memory>

namespace p3d {
	namespace d3d11 {

		template<class T>
		class ComPtr {
		public:
			ComPtr();

		private:
			T* _ptr;
		};
	}
}