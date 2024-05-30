//
// Created by n3dry on 24.05.24.
//

#include "Application.h"
#include "Event.h"
#include "Screen.h"
#include "WindowManager.h"

namespace GUI {

class Application::EventLoopImpl {
public:
    int exec()
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
                case SDL_MOUSEMOTION: {
                    int mouseX = event.motion.x;
                    int mouseY = event.motion.y;
                    GUI::WindowManager::the().onMouseMove(mouseX, mouseY);
                    break;
                }
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    handleKey(event.key);
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

private:
    void handleKey(const SDL_KeyboardEvent& event)
    {
        Key key = Key::Unknown;
        ADS::String text;

        switch (event.keysym.sym) {
        case SDLK_LEFT:
            key = Key::Left;
            break;
        case SDLK_RIGHT:
            key = Key::Right;
            break;
        case SDLK_UP:
            key = Key::Up;
            break;
        case SDLK_DOWN:
            key = Key::Down;
            break;
        case SDLK_BACKSPACE:
            key = Key::Backspace;
            break;
        case SDLK_RETURN:
            key = Key::Enter;
            break;
        default:
            text = static_cast<char>(event.keysym.sym);
            break;
        }

        // FIXME: Handle key modifiers
        if (event.type == SDL_KEYDOWN) {
            GUI::WindowManager::the().onKeyDown(KeyEvent { Event::Type::KeyDown, key, text });
        } else if (event.type == SDL_KEYUP) {
            GUI::WindowManager::the().onKeyUp(KeyEvent { Event::Type::KeyDown, key, text });
        }
    }
};

Application::Application()
    : m_impl(new Application::EventLoopImpl())
{
}

Application::~Application() = default;

int Application::exec()
{
    return m_impl->exec();
}

} // GUI