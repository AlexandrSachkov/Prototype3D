#pragma once

#include <chrono>

namespace p3d
{
	typedef unsigned long long UUID;

	static UUID genUuid()
	{
		auto time = std::chrono::high_resolution_clock::now();
		return time.time_since_epoch().count();
	}
}