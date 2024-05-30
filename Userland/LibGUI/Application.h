//
// Created by n3dry on 24.05.24.
//

#pragma once

#include "Types.h"

namespace GUI {

class Application {
public:
    Application();
    ~Application();

    int exec();

private:
    class EventLoopImpl;
    ADS::UniquePtr<EventLoopImpl> m_impl {};
};

} // GUI
