#include "device_driver.h"

static void Sys_Init(void)
{
	Clock_Init();
	LED_Init();
	Uart_Init(115200);
}

/* Key 인식 */

#if 0

void Main(void)
{
	Sys_Init();
	Uart_Printf("KEY Input Test #1\n");

	// KEY[1:0], GPB[7:6]을 GPIO 입력으로 선언
	Macro_Write_Block(GPIOB->CRL,0xFF,0x44,24);

	for(;;)
	{	//함수로로
		LED_Display((~(GPIOB->IDR)>>6) & 0x3);

		// KEY0이 눌렸으면 LED0를 ON, 안 눌렸으면 OFF, if문으로로
		/*if((GPIOB->IDR>>6 & 1) == 0) {
			Macro_Clear_Bit(GPIOB->ODR, 8);
		} else {
			Macro_Set_Bit(GPIOB->ODR, 8);
		}
		// KEY1이 눌렸으면 LED1를 ON, 안 눌렸으면 OFF
		if((GPIOB->IDR>>7 & 1) == 0) {
			Macro_Clear_Bit(GPIOB->ODR, 9);
		} else {
			Macro_Set_Bit(GPIOB->ODR, 9);
		}*/
	}
}

#endif

/* Key에 의한 LED Toggling */

#if 1

void Main(void)
{
	Sys_Init();
	Uart_Printf("KEY Input Toggling #1\n");


	// KEY[1:0], GPB[7:6]을 GPIO 입력으로 선언
	Macro_Write_Block(GPIOB->CRL,0xFF,0x88,24);
	Macro_Set_Bit(GPIOA->ODR, 6); //pull up, 안쓰는 A포트 LED0에 해당하는 곳에 1 줘서 확인
	Macro_Set_Bit(GPIOA->ODR, 7);

	int lock= 1;
	int lock1 = 1;
	for(;;)
	{
		// KEY0가 눌릴때마다 LED0의 값을 Toggling
		if((lock!=0) && ((GPIOB->IDR>>6 & 1) == 0)) {
			Macro_Invert_Bit(GPIOB->ODR, 8);
			lock=0;
		}
		else if((lock==0)&& ((GPIOB->IDR>>6 & 1) == 1)) {
			lock=1;
		}
		if((lock1!=0) && ((GPIOB->IDR>>7 & 1) == 0)) {
			Macro_Invert_Bit(GPIOB->ODR, 9);
			lock1=0;
		}
		else if((lock1==0)&& ((GPIOB->IDR>>7 & 1) == 1)) {
			lock1=1;
		}
	}
}

#endif
