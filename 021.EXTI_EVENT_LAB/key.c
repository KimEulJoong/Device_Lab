#include "device_driver.h"

void Key_Poll_Init(void)
{
	Macro_Set_Bit(RCC->APB2ENR, 3);
	Macro_Write_Block(GPIOB->CRL, 0xff, 0x44, 24);
}

#define COUNT_FOR_CHAT 		20000

static int Key_Check_Input(void)
{
	return ~Macro_Extract_Area(GPIOB->IDR, 0x3, 6) & 0x3;
}

int Key_Get_Pressed(void)
{
	unsigned int i, k;

	for(;;)
	{
		k = Key_Check_Input();

		for(i=0; i<COUNT_FOR_CHAT; i++)
		{
			if(k != Key_Check_Input())
			{
				break;
			}
		}

		if(i == COUNT_FOR_CHAT) break;
	}

	return k;
}

void Key_Wait_Key_Released(void)
{
	while(Key_Get_Pressed());
}

int Key_Wait_Key_Pressed(void)
{
	int k;

	while((k = Key_Get_Pressed()) == 0);
	return k;
}

void Key_ISR_Enable(int en)
{
	if(en)
	{
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
	}

	else
	{
		NVIC_DisableIRQ((IRQn_Type)23);
		
	}
}
