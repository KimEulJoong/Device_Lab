#include "device_driver.h"

static void Sys_Init(void) //랜덤함수용 -> random에 time넣으면 x
{
	Clock_Init();
	LED_Init();
	Key_Poll_Init();
	Uart_Init(115200);
}

void Main(void)
{
	volatile int i;

	Sys_Init();
	Uart_Printf("ADC Test\n");

	Adc_IN5_Init();

	for(;;)
	{
		Adc_Start(); //랜덤
		while(!Adc_Get_Status());
		Uart1_Printf("0x%.4X\n", Adc_Get_Data());
		for(i=0; i<0x400000; i++);
	}
}
