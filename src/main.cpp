#include "gui.hpp"
#include "logger.hpp"
#include "renderer.hpp"
#include <iostream>

static bool application_running = true;

Renderer renderer;

bool MainSetup() {
    Logger::Init();
    renderer.Init();
    return true;
}

bool MainLoop() {
    return renderer.RenderFrame();
}

bool MainEnd() { return true; }

int main() {
    if (!MainSetup()) return 1;

    while (application_running)
        if (!MainLoop()) break;

    if (!MainEnd()) return 1;

    return 0;
}