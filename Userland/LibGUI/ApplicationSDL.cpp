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
            updateTimer();

            if (!m_eventQueue.empty()) {
                ADS::Vector<QueuedEvent> queue = std::move(m_eventQueue);
                for (auto& queuedEvent : queue) {
                    if (queuedEvent.receiver) {
                        queuedEvent.receiver->event(*queuedEvent.event);
                    } else {
                        if (queuedEvent.event->type() == Event::Type::Quit)
                            return 0;
                        std::cerr << "event type " << static_cast<int>(queuedEvent.event->type()) << " has no receiver." << std::endl;
                        return 1;
                    }
                }
            }
        }
    }

    void postEvent(CObject* receiver, ADS::UniquePtr<Event>&& event)
    {
        m_eventQueue.push_back({ receiver, std::move(event) });
    }

    int startTimer(int intervalMs, CObject& object)
    {
        const Timer timer {
            .timerId = m_nextTimerId,
            .intervalMs = intervalMs,
            .timeout = std::chrono::steady_clock::now() + std::chrono::milliseconds(intervalMs),
            .owner = &object
        };
        m_timers[timer.timerId] = timer;

        // FIXME: Handle overflow
        ++m_nextTimerId;
        return timer.timerId;
    }

    void killTimer(int timerId)
    {
        if (!m_timers.contains(timerId))
            return;

        m_timers.erase(timerId);
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
                Application::instance().postEvent(&GUI::WindowManager::instance(), ADS::UniquePtr<MouseEvent>(new MouseEvent(Event::Type::MouseUp, event.motion.x, event.motion.y, MouseButton::Left)));
                break;
            case SDL_MOUSEBUTTONDOWN:
                // FIXME: map mouse buttons
                Application::instance().postEvent(&GUI::WindowManager::instance(), ADS::UniquePtr<MouseEvent>(new MouseEvent(Event::Type::MouseDown, event.motion.x, event.motion.y, MouseButton::Left)));
                break;
            case SDL_MOUSEMOTION:
                Application::instance().postEvent(&GUI::WindowManager::instance(), ADS::UniquePtr<MouseEvent>(new MouseEvent(Event::Type::MouseMove, event.motion.x, event.motion.y)));
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                handleKey(event.key);
                break;
            }
        }

        // FIXME: Don't repaint every "frame"
        const Rect paintRect { 0, 0, GUI::Screen::instance().width(), GUI::Screen::instance().height() };
        Application::instance().postEvent(&GUI::WindowManager::instance(), ADS::UniquePtr<PaintEvent>(new PaintEvent(paintRect)));
    }

    char handleAlt(const SDL_KeyboardEvent& event)
    {
        switch (event.keysym.sym) {
        case 'q':
            return '@';
        }
        return event.keysym.sym;
    }

    char handleShift(const SDL_KeyboardEvent& event)
    {
        if (event.keysym.sym >= 'a' && event.keysym.sym <= 'z')
            return static_cast<char>(event.keysym.sym - 32); // to ASCII uppercase

        switch (event.keysym.sym) {
        case '1':
            return '!';
        case '2':
            return '"';
        case '4':
            return '$';
        case '5':
            return '%';
        case '6':
            return '&';
        case '7':
            return '/';
        case '8':
            return '(';
        case '9':
            return ')';
        case '0':
            return '=';
        case '.':
            return ':';
        case ',':
            return ';';
        case '-':
            return '_';
        case '+':
            return '*';
        case '<':
            return '>';
        }
        return event.keysym.sym;
    }

    void handleKey(const SDL_KeyboardEvent& event)
    {
        Key key = Key::Key_Unknown;
        ADS::String text;

        switch (event.keysym.sym) {
        case SDLK_LEFT:
            key = Key::Key_Left;
            break;
        case SDLK_RIGHT:
            key = Key::Key_Right;
            break;
        case SDLK_UP:
            key = Key::Key_Up;
            break;
        case SDLK_DOWN:
            key = Key::Key_Down;
            break;
        case SDLK_BACKSPACE:
            key = Key::Key_Backspace;
            break;
        case SDLK_RETURN:
            key = Key::Key_Return;
            break;
        case SDLK_a:
            key = Key::Key_A;
            break;
        case SDLK_HOME:
            key = Key::Key_Home;
            break;
        case SDLK_END:
            key = Key::Key_End;
            break;
        }

        if (event.keysym.sym > SDLK_UNKNOWN && event.keysym.sym <= SDLK_z) {
            if (event.keysym.mod & KMOD_SHIFT) {
                text = handleShift(event);
            } else if (event.keysym.mod & KMOD_RALT) {
                text = handleAlt(event);
            } else {
                text = static_cast<char>(event.keysym.sym);
            }
        }

        unsigned int modifier = KeyboardModifier::NoModifier;
        if (event.keysym.mod & KMOD_SHIFT)
            modifier |= KeyboardModifier::ShiftModifier;
        if (event.keysym.mod & KMOD_ALT)
            modifier |= KeyboardModifier::AltModifier;
        if (event.keysym.mod & KMOD_CTRL)
            modifier |= KeyboardModifier::CtrlModifier;

        if (event.type == SDL_KEYDOWN) {
            Application::instance().postEvent(&GUI::WindowManager::instance(), ADS::UniquePtr<KeyEvent>(new KeyEvent(Event::Type::KeyDown, key, static_cast<KeyboardModifier>(modifier), text)));
        } else if (event.type == SDL_KEYUP) {
            Application::instance().postEvent(&GUI::WindowManager::instance(), ADS::UniquePtr<KeyEvent>(new KeyEvent(Event::Type::KeyUp, key, static_cast<KeyboardModifier>(modifier), text)));
        }
    }

    void updateTimer()
    {
        for (auto& timerEntry : m_timers) {
            Timer& timer = timerEntry.second;
            if (timer.timeout > std::chrono::steady_clock::now())
                continue;
            postEvent(timer.owner, ADS::UniquePtr<TimerEvent>(new TimerEvent()));
            timer.timeout = std::chrono::milliseconds(timer.intervalMs) + std::chrono::steady_clock::now();
        }
    }

    struct QueuedEvent {
        CObject* receiver { nullptr };
        ADS::UniquePtr<Event> event;
    };

    struct Timer {
        int timerId {};
        int intervalMs {};
        std::chrono::steady_clock::time_point timeout;
        CObject* owner {};
    };

    ADS::Vector<QueuedEvent> m_eventQueue;
    ADS::HashMap<int, Timer> m_timers;
    int m_nextTimerId { 0 };
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

int Application::startTimer(int intervalMs, CObject& object)
{
    return m_impl->startTimer(intervalMs, object);
}

void Application::killTimer(int timerId)
{
    m_impl->killTimer(timerId);
}

} // GUI