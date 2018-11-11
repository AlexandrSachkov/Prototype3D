#pragma once

#include "util_GlfwWindowManager.h"
#include <functional>

namespace p3d {
    namespace util {
        class DefaultSampleRunner : public GlfwWindowManager {
        public:
            DefaultSampleRunner();
            ~DefaultSampleRunner();

            bool initialize(
                const std::string title,
                const unsigned int screenDim[2],
                bool fullscreen,
                bool decorated,
                bool resizable,
                bool enableCursor
            );

            void setRunProcedure(std::function<void()> procedure);
            void start();
            void stop();

        private:
            bool _running;
            std::function<void()> _runProcedure;
        };
    }
}
