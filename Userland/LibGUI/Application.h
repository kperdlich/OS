//
// Created by n3dry on 24.05.24.
//

#pragma once

#include "Types.h"
#include "CObject.h"

namespace GUI {

class Application {
public:
    static Application& instance();

    Application();
    ~Application();

    int exec();

    void postEvent(CObject* receiver, ADS::OwnPtr<Event>&& event);
    int startTimer(int intervalMs, CObject& object);
    void killTimer(int timerId);

private:
    class EventLoopImpl;
    ADS::OwnPtr<EventLoopImpl> m_impl {};
};

} // GUI
