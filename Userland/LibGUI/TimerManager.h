//
// Created by n3dry on 07.06.24.
//

#pragma once

#include "Types.h"

namespace GUI {

class CObject;

class TimerManager final {
public:
    static TimerManager& instance();

    int startTimer(int intervalMs, CObject& object);
    void killTimer(int timerId);
};

}