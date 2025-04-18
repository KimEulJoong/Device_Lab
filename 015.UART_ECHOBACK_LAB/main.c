#include "device_driver.h"

static void Sys_Init(void)
{
	Clock_Init();
	LED_Init();
	Uart_Init(115200);
	Key_Poll_Init();
}

#if 0
//예제 1
void Main(void)
{
	Sys_Init();

	//A B C 전송 -> flag check -> DR에 A보내고 -> flag check -> DR에 B보내고 ...
	while((Macro_Check_Bit_Set(USART1->SR, 7) != 1));
	USART1->DR = 'A';
	while((Macro_Check_Bit_Set(USART1->SR, 7) != 1));
	USART1->DR = 'B';
	while((Macro_Check_Bit_Set(USART1->SR, 7) != 1));	
	USART1->DR = 'C';
}
#endif

#if 1 
//예제 2
void Main(void)
{
	Sys_Init();
	Uart_Printf("UART Echo-Back Test\n");

	for(;;)
	{
		// 받은 글자를 다시 UART로 출력한다
		char x;
		while(Macro_Check_Bit_Clear(USART1->SR, 5));
		x = Macro_Extract_Area(USART1->DR, 0xff, 0);
		while(Macro_Check_Bit_Clear(USART1->SR, 7));
		Macro_Write_Block(USART1->DR, 0xff, x, 0);
	}
}
#endif