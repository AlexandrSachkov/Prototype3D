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
                unsigned int screenDim[2],
                bool fullscreen,
                bool decorated,
                bool resizable,
                bool enableCursor,
                std::string title
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
