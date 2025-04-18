#include "device_driver.h"

static void Sys_Init(void)
{
	Clock_Init();
	LED_Init();
	Uart_Init(115200);
	Key_Poll_Init();
}

#define BASE  (500) //msec

#if 0
static void Buzzer_Beep(unsigned char tone, int duration)
{
	const static unsigned short tone_value[] = {261,277,293,311,329,349,369,391,415,440,466,493,523,554,587,622,659,698,739,783,830,880,932,987};

	TIM3_Out_Freq_Generation(tone_value[tone]);
	TIM2_Delay(duration);
	TIM3_Out_Stop();
}

void Main(void)
{
	int i;

	enum key{C1, C1_, D1, D1_, E1, F1, F1_, G1, G1_, A1, A1_, B1, C2, C2_, D2, D2_, E2, F2, F2_, G2, G2_, A2, A2_, B2};
	enum note{N16=BASE/4, N8=BASE/2, N4=BASE, N2=BASE*2, N1=BASE*4};
	const int song1[][2] = {{G1,N4},{G1,N4},{E1,N8},{F1,N8},{G1,N4},{A1,N4},{A1,N4},{G1,N2},{G1,N4},{C2,N4},{E2,N4},{D2,N8},{C2,N8},{D2,N2}};
	const char * note_name[] = {"C1", "C1#", "D1", "D1#", "E1", "F1", "F1#", "G1", "G1#", "A1", "A1#", "B1", "C2", "C2#", "D2", "D2#", "E2", "F2", "F2#", "G2", "G2#", "A2", "A2#", "B2"};

	Sys_Init();
	Uart1_Printf("Buzzer Test!!\n");

	TIM3_Out_Init();

	Uart1_Printf("%s ", note_name[C1]);
	Buzzer_Beep(C1,N4);
	Uart1_Printf("%s ", note_name[D1]);
	Buzzer_Beep(D1,N4);
	Uart1_Printf("%s ", note_name[E1]);
	Buzzer_Beep(E1,N4);
	Uart1_Printf("%s ", note_name[F1]);
	Buzzer_Beep(F1,N4);
	Uart1_Printf("%s ", note_name[G1]);
	Buzzer_Beep(G1,N4);
	Uart1_Printf("%s ", note_name[A1]);
	Buzzer_Beep(A1,N4);
	Uart1_Printf("%s ", note_name[B1]);
	Buzzer_Beep(B1,N4);
	Uart1_Printf("%s ", note_name[C2]);
	Buzzer_Beep(C2,N4);

	Uart1_Printf("\nSong Play\n");

	for(i=0; i<(sizeof(song1)/sizeof(song1[0])); i++)
	{
		Uart1_Printf("%s ", note_name[song1[i][0]]);
		Buzzer_Beep(song1[i][0], song1[i][1]);
	}
}
#endif

//4.15과제 베릴로그5, D4, 추가수정
#if 1

void Main(void)
{
	Sys_Init();
	TIM4_Out_Init();
	char x;
	int timer_run = 0;
	
	unsigned int duty = 0;
	
	
	TIM4_Out_PWM_Generation(1000, 5);

	for(;;)
	{

		TIM2_Delay(100);
		Uart1_Printf(".");

		x = Uart1_Get_Char();

		
		if (x != 0) 
		{
			Uart1_Printf("%c", x);

			if(x == '0')
			{
				timer_run = 0;
				TIM4_Stop();
				LED_All_Off();
				
			}
			else if(x >= '1' && x <= '9')
			{
				duty = (x - '0');
				TIM4_Change_Duty(duty);
				
				timer_run = 1;
			}
		}

		if(TIM4_Check_Timeout() && timer_run) 
		{
			if(timer_run){
				LED_All_Off();
                timer_run = 0;
			}
			else{
				LED_All_On();
                timer_run = 1;
			}
		}
	}
}
#endif

#if 0 //참고
void LED_Init(void)
{
    // PB8 (TIM4_CH3) 또는 TIM3_CH3으로 PWM 출력 설정
    // GPIO + AFIO + TIM3 설정
    // PWM 주기 및 초기 듀티 설정
}

void LED_Set_Brightness(int percent)
{
    // percent: 0~100
    // CCRx 레지스터에 비례하여 설정
}

void LED_Off(void)
{
    LED_Set_Brightness(0);
}



void USART1_IRQHandler(void)
{
    if (USART1->SR & USART_SR_RXNE) {
        char ch = USART1->DR;  // 수신 문자

        Key_ISR_Handler(ch);   // 사용자 핸들러 호출
    }
}



#include "stm32f10x.h"
#include "led.h"
#include "uart.h"
#include "key.h"
#include <stdio.h>

void Key_ISR_Handler(char ch) {
    UART1_Send_Byte(ch);  // 터미널에 입력된 키 출력

    if (ch == '0') {
        LED_Off();  // LED 끄기
    }
    else if (ch >= '1' && ch <= '9') {
        int brightness = (ch - '0') * 100 / 9;  // 1~9 → 11~100%
        LED_Set_Brightness(brightness);
    }
}

int main(void)
{
    SystemInit();
    LED_Init();         // PB8 PWM 설정
    UART1_Init(9600);   // UART 인터럽트 기반 수신 활성화
    Key_Init();         // UART1 RX 인터럽트 설정 및 Key_ISR_Handler 연동

    while (1) {
        // 메인 루프는 idle
        __WFI();  // 대기 상태 (optional)
    }
}


#endif