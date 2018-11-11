#include "util_DefaultSampleRunner.h"
#include "../assert.h"

namespace p3d {
    namespace util {
        DefaultSampleRunner::DefaultSampleRunner()
            : _running(false), _runProcedure([]() {}) {}

        DefaultSampleRunner::~DefaultSampleRunner() {}

        bool DefaultSampleRunner::initialize(
            const std::string title,
            const unsigned int screenDim[2],
            bool fullscreen,
            bool decorated,
            bool resizable,
            bool enableCursor
        ) {
            P3D_ASSERT_R(GlfwWindowManager::initialize(
                title,
                screenDim,
                fullscreen,
                decorated,
                resizable,
                enableCursor
            ), "Failed to initialize window manager");

            addWindowCloseEvCbk([&]() {
                stop();
            });
            addKeyEvCbk([&](int key, int scancode, int action, int mod) {
                if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                    stop();
                }    
            });

            return true;
        }

        void DefaultSampleRunner::setRunProcedure(std::function<void()> procedure) {
            _runProcedure = procedure;
        }

        void DefaultSampleRunner::start() {
            _running = true;
            do {
                pollEvents();
                _runProcedure();
            } while (_running);
        }

        void DefaultSampleRunner::stop() {
            _running = false;
        }
    }
}