#pragma once
#include "../ResourceDescriptions.h"

namespace p3d {
	class BufferI {
	public:
		virtual ~BufferI() {};
		virtual const BufferDesc& getDescription() const = 0;
		virtual unsigned int getSizeBytes() const = 0;
	};
}
