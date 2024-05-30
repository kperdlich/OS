//
// Created by n3dry on 24.05.24.
//

#include "Application.h"
#include "Screen.h"
#include "WindowManager.h"

namespace GUI {

int Application::exec()
{
    // TODO: Map and queue sdl events to application events and call WindowManager.

    bool quit = false;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEBUTTONUP:
                GUI::WindowManager::the().onMouseUp(event.button.button, event.motion.x, event.motion.y);
                break;
            case SDL_MOUSEBUTTONDOWN:
                GUI::WindowManager::the().onMouseDown(event.button.button, event.motion.x, event.motion.y);
                break;
            case SDL_MOUSEMOTION:
                int mouseX = event.motion.x;
                int mouseY = event.motion.y;
                GUI::WindowManager::the().onMouseMove(mouseX, mouseY);
                break;
            }
        }

        // FIXME: Only handle the paint event here and remove the rest.
        GUI::Screen::the().fill(GUI::Colors::White);
        GUI::WindowManager::the().paint();
        GUI::Screen::the().update();
    }

    return 0;
}

} // GUI