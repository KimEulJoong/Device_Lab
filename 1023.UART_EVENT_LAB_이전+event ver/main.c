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

#define BASE 500

enum key {
	C1, C1_, D1, D1_, E1, F1, F1_, G1, G1_, A1, A1_, B1,
	C2, C2_, D2, D2_, E2, F2, F2_, G2, G2_, A2, A2_, B2, REST
};

enum note {
	N16 = BASE / 4, N8 = BASE / 2, N4 = BASE, N2 = BASE * 2, N1 = BASE * 4
};

const int song1[][2] = {
	{G1,N4},{G1,N4},{E1,N8},{F1,N8},{G1,N4},{A1,N4},{A1,N4},{G1,N2},
	{G1,N4},{C2,N4},{E2,N4},{D2,N8},{C2,N8},{D2,N2}
};

const int song_len = sizeof(song1) / sizeof(song1[0]);

const unsigned short tone_value[] = {
	261,277,293,311,329,349,369,391,415,440,466,493,
	523,554,587,622,659,698,739,783,830,880,932,987
};

void Main(void)
{

/* 	enum key {
		C1, C1_, D1, D1_, E1, F1, F1_, G1, G1_, A1, A1_, B1,
		C2, C2_, D2, D2_, E2, F2, F2_, G2, G2_, A2, A2_, B2, REST
	};

	enum note {
		N16 = BASE / 4, N8 = BASE / 2, N4 = BASE, N2 = BASE * 2, N1 = BASE * 4
	};

	const int song1[][2] = {
		{G1,N4},{G1,N4},{E1,N8},{F1,N8},{G1,N4},{A1,N4},{A1,N4},{G1,N2},
		{G1,N4},{C2,N4},{E2,N4},{D2,N8},{C2,N8},{D2,N2}
	};

	const int song_len = sizeof(song1) / sizeof(song1[0]);

	const unsigned short tone_value[] = {
		261,277,293,311,329,349,369,391,415,440,466,493,
		523,554,587,622,659,698,739,783,830,880,932,987
	}; */



	Sys_Init();
	TIM3_Out_Init();
	TIM4_Out_Init();
	TIM4_Interrupt_Init(song1[0][1]);
	Uart1_RX_Interrupt_Enable(1);

	Uart1_Printf("Play\n");

	for(;;)
	{
		LED_Display(1);
		TIM2_Delay(500);
		LED_Display(2);
		TIM2_Delay(500);		

		Uart_Printf(".");
		TIM2_Delay(500);

	}
}


// 참고
#if 0
static int pwm_op = 0;
  char x = USART1->DR - '0';

  if(x == 0)
  {
    pwm_op = 0;
    TIM4_Out_Stop();
  }
  else if(x >= 1 && x <= 9)
  {
    if(pwm_op == 0)
    {
      TIM4Out_PWM_Generation(1000, x);
      pwm_op = 1;
    }
    else
    {
      TIM4_Change_Duty(x);
    }
  }
  NVIC_ClearPendingIRQ(37);
  #endif