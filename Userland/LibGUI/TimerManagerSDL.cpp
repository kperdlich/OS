//
// Created by n3dry on 07.06.24.
//

#include "TimerManager.h"
#include "Application.h"
#include <SDL.h>

namespace GUI {

TimerManager& TimerManager::instance()
{
    static TimerManager s_manager;
    return s_manager;
}

static Uint32 timerCallback(Uint32 interval, void* param)
{
    Application::instance().postEvent(static_cast<CObject*>(param), ADS::UniquePtr<TimerEvent>(new TimerEvent()));
    return interval;
}

int TimerManager::startTimer(int intervalMs, CObject& object)
{
    return SDL_AddTimer(intervalMs, timerCallback, &object);
}

void TimerManager::killTimer(int timerId)
{
    SDL_RemoveTimer(timerId);
}

}