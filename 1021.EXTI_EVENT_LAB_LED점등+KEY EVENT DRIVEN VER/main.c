#include "device_driver.h"

static void Sys_Init(void)
{
	Clock_Init();
	LED_Init();
	Uart_Init(115200);
	Key_Poll_Init();

	SCB->VTOR = 0x08003000;
	SCB->SHCSR = 0;
}


// KEY0, 1번에 해당하는 EVENT DRIVEN
extern volatile int Key_Value;

void Main(void)
{
	Sys_Init();
	Uart1_Printf("EXTI KEY ISR Test\n");

	Key_ISR_Enable(1); // EXTI[7:6] 인터럽트 허용 (key0, 1)

	for(;;)
	{
		if(Key_Value)              //IRQ 발생
		{
			Uart1_Printf("KEY=%d\n", Key_Value);
			Key_Value = 0;
		}

		LED_Display(1);
		TIM2_Delay(500);
		LED_Display(2);
		TIM2_Delay(500);
	}
}
