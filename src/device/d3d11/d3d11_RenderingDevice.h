#pragma once

#include "../RenderingDevice.h"
#include "../GPUMemoryManager.h"

namespace p3d {
	namespace d3d11 {

		class RenderingDevice :
			public p3d::RenderingDevice<RenderingDevice>,
			public p3d::GPUMemoryManager<RenderingDevice>{

		public:
			static RenderingDevice& instance() {
				static RenderingDevice instance;
				return instance;
			}

			bool initialize();
			void release();

		protected:
			RenderingDevice();
			~RenderingDevice();

		private:


			RenderingDevice(RenderingDevice const&) = delete;             // Copy construct
			RenderingDevice(RenderingDevice&&) = delete;                  // Move construct
			RenderingDevice& operator=(RenderingDevice const&) = delete;  // Copy assign
			RenderingDevice& operator=(RenderingDevice &&) = delete;      // Move assign
		};
	}
}