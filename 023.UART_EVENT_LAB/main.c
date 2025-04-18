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

extern volatile int Key_Value;
extern volatile int Uart1_Rx_In;
extern volatile int Uart1_Rx_Data;

void Main(void)
{
	Sys_Init();
	TIM4_Out_Init();
	Uart1_RX_Interrupt_Enable(1);

	for(;;)
	{
		

	}
	Uart_Printf(".");
	TIM2_Delay(500);
}
