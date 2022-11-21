#include "timer.h"


uInt32_t tick = 0;

static void timer_callback(registers_t regs)
{
	Console* con = GetConsole();

	tick++;
	
    if (tick % 18 == 0)
    {
		/*unsigned char buff[30];
		ConvertInt2String(tick, buff);
		k_print("One second has passed ", ColorBlack | ColorGreen, con);		
		k_print(buff, ColorBlack | ColorYellow, con);*/
    }
}


void timer_install(uInt32_t frequency)
{	
	register_interrupt_handler(IRQ0, &timer_callback);	
}

void timer_uninstall()
{
	unregister_interrupt_handler(IRQ0);
}