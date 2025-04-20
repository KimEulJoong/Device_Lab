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

#if 0  //USART1에 수신있으면 ISR에서 에코백, key0,1도 포함, led 점등
void Main(void)
{
	Sys_Init();
	Uart1_Printf("USART1 IRQ Test\n");

	Key_ISR_Enable(1);

	// NVIC USART1 Pending clear
	NVIC_ClearPendingIRQ(37);
	// USART1 RX interrupt enable
	USART1->CR1 |= (1<<5);
	// NVIC USART1 interrupt enable
	NVIC_EnableIRQ(37);

	char x;
	x = Key_Get_Pressed();


	TIM4_Out_Init();

	for(;;)
	{
		if (x == '0')
		{
			TIM4_Out_Stop();
		}
		else if (x >= '1' && x <= '9')
		{
			TIM4_Out_PWM_Generation(1000, x - '0'); //주파수랑 duty받고 타이머 시작
			
			TIM4_Change_Duty(x - '0');
			
		}
	}
}
#endif

#if 1
//키보드 및 KEY 입력시 터미널에 에코백 , LED 독립적으로 번갈아가며 점등 , KEY 입력 인터럽트로 처리
void Main(void)
{
	Sys_Init();
	Uart1_Printf("USART1 IRQ Test\n");

	Key_ISR_Enable(1);

	// NVIC USART1 Pending clear
	NVIC_ClearPendingIRQ(37);
	// USART1 RX interrupt enable
	USART1->CR1 |= (1<<5);
	// NVIC USART1 interrupt enable
	NVIC_EnableIRQ(37);

	for(;;)
	{
		if(Key_Value)
		{
			Uart1_Printf("KEY=%d\n", Key_Value);
			Key_Value = 0;
		}

		LED_Display(1);
		TIM2_Delay(100);
		LED_Display(2);
		TIM2_Delay(100);
	}
}
#endif

#if 0
// 참고
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