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
        while (true) {
            processSDLEvents();
            if (!m_eventQueue.empty()) {
                ADS::Vector<QueuedEvent> queue;
                {
                    const ADS::LockGuard<ADS::Mutex> mutex(m_mutex);
                    queue = std::move(m_eventQueue);
                }
                for (auto& queuedEvent : queue) {
                    if (queuedEvent.receiver) {
                        queuedEvent.receiver->event(*queuedEvent.event);
                    } else {
                        if (queuedEvent.event->type() == Event::Type::Quit) {
                            return 0;
                        }
                        std::cerr << "event type " << static_cast<int>(queuedEvent.event->type()) << " has no receiver." << std::endl;
                        return 1;
                    }
                }
            }
        }
    }

    void postEvent(CObject* receiver, ADS::UniquePtr<Event>&& event)
    {
        const ADS::LockGuard<ADS::Mutex> mutex(m_mutex);
        m_eventQueue.push_back({ receiver, std::move(event) });
    }

private:
    void processSDLEvents()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                Application::instance().postEvent(nullptr, ADS::UniquePtr<Event>(new Event(Event::Type::Quit)));
                break;
            case SDL_MOUSEBUTTONUP:
                // FIXME: map mouse buttons
                Application::instance().postEvent(&GUI::WindowManager::the(), ADS::UniquePtr<MouseEvent>(new MouseEvent(Event::Type::MouseUp, event.motion.x, event.motion.y, MouseButton::Left)));
                break;
            case SDL_MOUSEBUTTONDOWN:
                // FIXME: map mouse buttons
                Application::instance().postEvent(&GUI::WindowManager::the(), ADS::UniquePtr<MouseEvent>(new MouseEvent(Event::Type::MouseDown, event.motion.x, event.motion.y, MouseButton::Left)));
                break;
            case SDL_MOUSEMOTION:
                Application::instance().postEvent(&GUI::WindowManager::the(), ADS::UniquePtr<MouseEvent>(new MouseEvent(Event::Type::MouseMove, event.motion.x, event.motion.y)));
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                handleKey(event.key);
                break;
            }
        }

        // FIXME: Don't repaint every "frame"
        Application::instance().postEvent(&GUI::WindowManager::the(), ADS::UniquePtr<Event>(new Event(Event::Type::Paint)));
    }

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
            Application::instance().postEvent(&GUI::WindowManager::the(), ADS::UniquePtr<KeyEvent>(new KeyEvent(Event::Type::KeyDown, key, text)));
        } else if (event.type == SDL_KEYUP) {
            Application::instance().postEvent(&GUI::WindowManager::the(), ADS::UniquePtr<KeyEvent>(new KeyEvent(Event::Type::KeyUp, key, text)));
        }
    }

    struct QueuedEvent {
        CObject* receiver { nullptr };
        ADS::UniquePtr<Event> event;
    };

    ADS::Vector<QueuedEvent> m_eventQueue;
    ADS::Mutex m_mutex;
};

static Application* s_instance = nullptr;

Application& Application::instance()
{
    ASSERT(s_instance != nullptr);
    return *s_instance;
}

Application::Application()
    : m_impl(new Application::EventLoopImpl())
{
    ASSERT(s_instance == nullptr);
    s_instance = this;
}

Application::~Application() = default;

int Application::exec()
{
    return m_impl->exec();
}

void Application::postEvent(CObject* receiver, ADS::UniquePtr<Event>&& event)
{
    m_impl->postEvent(receiver, std::move(event));
}

} // GUI