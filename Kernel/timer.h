#ifndef __TIMER_H__
#define __TIMER_H__


#include "video.h"
#include "irq.h"

void timer_install(uInt32_t frequency);
void timer_uninstall();

#endif