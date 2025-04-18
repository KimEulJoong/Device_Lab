#include "device_driver.h"

static void Sys_Init(void)
{
	Clock_Init();
	LED_Init();
	Uart_Init(115200);
	Key_Poll_Init();
}
# if 0
void Main(void)
{
	volatile int i;

	Sys_Init();
	Uart_Printf("UART Echo-Back Test\n");

	for(;;)
	{
		Uart1_Printf("\n키보드로 받는 글자 10개를 echo-back\n");
		Uart1_Printf("\n키보드를 누르면 다시 PC로 글자를 전송한다 (10글자)\n");

		for(i=0; i<10; i++)
		{
			Uart1_Printf("\n>");
			Uart1_Printf("%c", Uart1_Get_Char());
		}

		Uart1_Printf("\n계속 LED Toggling하고 있으며 키보드의 x를 누르면 종료 함");

		do
		{
			LED_Display(0x1);
			for(i=0; i<0x100000; i++);
			LED_Display(0x2);
			for(i=0; i<0x100000; i++);
		}while(Uart1_Get_Pressed() != 'x');
	}
}
#endif
//과제1


void Main(void)
{
	Sys_Init();
	Uart_Printf("Uart_Get_Int_Num() Test\n");

	for(;;)
	{
		int n;

		Uart_Printf("Input number: ");

		n = Uart_Get_Int_Num();
		
		Uart_Printf("\n %d \n", n);
	}
}


#if 0  //ASCII 코드 변환 예제
#include <stdio.h>
 
int Change_Char_to_Int(char num)
{
    // 코드 작성
    return num - '0';//return num - 0x30;
}
 
void main(void)
{
    char a;
 
    scanf("%c", &a);
    printf("%d\n", Change_Char_to_Int(a));
}
#endif