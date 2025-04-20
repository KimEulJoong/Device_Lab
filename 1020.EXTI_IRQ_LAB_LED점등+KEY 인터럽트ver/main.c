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

void Main(void)
{
	Sys_Init();
	Uart1_Printf("EXTI Test\n");

	// AFIO, Port-B Clock Enable
	RCC->APB2ENR |= (1<<3) | (1<<0);
	// PB[7:6]을 입력으로 선언
	GPIOB->CRL |= (0x4<<28) | (0x4<<24);
	// EXTI[7:6] Pending Clear
	EXTI->PR = (1<<7) | (1<<6);
	// PB[7:6]을 EXTI 소스로 설정하고 Falling edge 선택, EXTI[7:6] 인터럽트 허용
	AFIO->EXTICR[1] = (1<<12) | (1<<8);
	EXTI->FTSR |= (1<<7) | (1<<6);
	EXTI->IMR |= (1<<7) | (1<<6);
	// NVIC의 인터럽트 Pending clear
	NVIC_ClearPendingIRQ(23);  //EXTI->PR = 0x3 << 6;
	// EXTI9_5 인터럽트 허용
	NVIC_EnableIRQ(23);

	for(;;)
	{
		LED_Display(1);
		TIM2_Delay(500);
		LED_Display(2);
		TIM2_Delay(500);
	}
}


