#pragma once

#include <chrono>
#include <assert.h>

namespace p3d {
    namespace util {
        class Timer {
        public:
            Timer() {
                _previousTime = std::chrono::high_resolution_clock::now();
            }

            ~Timer() {
            }

            double deltaT() {
                auto time = std::chrono::high_resolution_clock::now();
                double deltaT = std::chrono::duration_cast<std::chrono::duration<double>>(time - _previousTime).count() * 1000000000;
                _previousTime = time;
                return deltaT;
            }

        private:
            std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds> _previousTime;
        };
    }
}
