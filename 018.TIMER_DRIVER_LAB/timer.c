#include "device_driver.h"

#define TIM2_TICK         	(20) 				// usec
#define TIM2_FREQ 	  		(1000000/TIM2_TICK)	// Hz
#define TIME2_PLS_OF_1ms  	(1000/TIM2_TICK)
#define TIM2_MAX	  		(0xffffu)

#define TIM4_TICK	  		(20) 				// usec
#define TIM4_FREQ 	  		(1000000/TIM4_TICK) // Hz
#define TIME4_PLS_OF_1ms  	(1000/TIM4_TICK)
#define TIM4_MAX	  		(0xffffu)

void TIM2_Stopwatch_Start(void)
{
	Macro_Set_Bit(RCC->APB1ENR, 0);

	// TIM2 CR1 설정: down count, one pulse
	TIM2->CR1 = (1<<4)|(1<<3);
	// PSC 초기값 설정 => 20usec tick이 되도록 설계 (50KHz)
	TIM2->PSC = (unsigned int)(TIMXCLK/50000.0 + 0.5)-1;
	// ARR 초기값 설정 => 최대값 0xFFFF 설정
	TIM2->ARR = 0xffff;
	// UG 이벤트 발생
	TIM2->EGR = 1<<0;
	// TIM2 start
	Macro_Set_Bit(TIM2->CR1, 0);
}

unsigned int TIM2_Stopwatch_Stop(void)
{
	unsigned int time;

	// TIM2 stop
	Macro_Clear_Bit(TIM2->CR1, 0);
	// CNT 초기 설정값 (0xffff)와 현재 CNT의 펄스수 차이를 구하고
	// 그 펄스수 하나가 20usec이므로 20을 곱한값을 time에 저장
	time = (TIM2->ARR - TIM2->CNT) * 20.0;
	// 계산된 time 값을 리턴(단위는 usec)
	return time;

}

// time msec 만큼 delay(깃발), 종료까지
void TIM2_Delay(int time)    
{
	Macro_Set_Bit(RCC->APB1ENR, 0);

	// TIM2 CR1 설정: down count, one pulse
	TIM2->CR1 = (1<<4)|(1<<3);
	// PSC 초기값 설정 => 20usec tick이 되도록 설계 (50KHz)
	TIM2->PSC = (unsigned int)(TIMXCLK/50000.0 + 0.5)-1;    //0.5: 반올림 고려, -1: 0부터 시작하므로
	// ARR 초기값 설정 => 요청한 time msec에 해당하는 초기값 설정
	TIM2->ARR = (unsigned int)(time*50000.0/1000.0 + 0.5)-1;
	// UG 이벤트 발생
	TIM2->EGR = 1<<0;
	// Update Interrupt Pending Clear(pending: flag가 발생하면)
	Macro_Clear_Bit(TIM2->SR, 0);
	// Update Interrupt Enable(임시보류)

	// TIM2 start
	Macro_Set_Bit(TIM2->CR1, 0);
	// Wait timeout (flag가 발생하면)
	while(Macro_Check_Bit_Clear(TIM2->SR, 0));
	// Stop and Power off
	Macro_Clear_Bit(TIM2->CR1, 0);
	Macro_Clear_Bit(TIM2->DIER, 0);
}



// 시작만
void TIM2_Delay2(int time)
{
	Macro_Set_Bit(RCC->APB1ENR, 0);

	// TIM2 CR1 설정: down count, one pulse
	TIM2->CR1 = (1<<4)|(1<<3);
	// PSC 초기값 설정 => 20usec tick이 되도록 설계 (50KHz)
	TIM2->PSC = (unsigned int)(TIMXCLK/50000.0 + 0.5)-1;
	// ARR 초기값 설정 => 요청한 time msec에 해당하는 초기값 설정
	TIM2->ARR = (unsigned int)(time*50000.0/1000.0 + 0.5)-1;
	// UG 이벤트 발생
	TIM2->EGR = 1<<0;
	// Update Interrupt Pending Clear(pending: flag가 발생하면)
	Macro_Clear_Bit(TIM2->SR, 0);
	// Update Interrupt Enable(임시보류)

	// TIM2 start
	Macro_Set_Bit(TIM2->CR1, 0);

}

//타임아웃(falg) 발생하면 1, 아니면 0 리턴
int TIM2_Check_Timeout(void)  
{
	if(Macro_Check_Bit_Set(TIM2->SR, 0)) //Timeout(1) 되었으면
	{
		Macro_Clear_Bit(TIM2->SR, 0);   //Timeout flag clear
		return 1;
	}
	else
	{
		return 0;
	}
}

void TIM4_Repeat(int time)
{
	Macro_Set_Bit(RCC->APB1ENR, 2);

	// TIM4 CR1: ARPE=0, down counter, repeat mode
	TIM4->CR1 = (0<<7)|(1<<4)|(0<<3);
	// PSC(50KHz),  ARR(reload시 값) 설정
	TIM4->PSC = (unsigned int)(TIMXCLK/50000.0 + 0.5)-1;
	TIM4->ARR = (unsigned int)(time*50000.0/1000.0 + 0.5)-1;
	// UG 이벤트 발생
	TIM4->EGR = 1<<0;
	// Update Interrupt Pending Clear
	Macro_Clear_Bit(TIM4->SR, 0);
	// Update Interrupt Enable

	// TIM4 start
	Macro_Set_Bit(TIM4->CR1, 0);
}

int TIM4_Check_Timeout(void)
{
	// 타이머가 timeout 이면 1 리턴, 아니면 0 리턴
	if(Macro_Check_Bit_Set(TIM4->SR, 0))
	{
		Macro_Clear_Bit(TIM4->SR, 0);
		return 1;
	}
	else
	{
		return 0;
	}

}

void TIM4_Stop(void)
{
	Macro_Clear_Bit(TIM4->CR1, 0);
	
}

// 가변적인 주기 delay함수 
void TIM4_Change_Value(int time)
{
	TIM4->ARR = 50 * time;
}

void TIM4_Change_Duty(unsigned int duty)
{
	TIM4->CCR3 = (unsigned int)(TIM4->ARR*(duty/10.) + 0.5)-1;
	TIM4->EGR = 1<<0;
	Macro_Set_Bit(TIM4->CR1, 0);
}

void TIM4_Out_PWM_Generation(unsigned int freq, unsigned int duty)
{
	// Down Counter, Repeat Mode
	TIM4->CR1 = (1<<4)|(0<<3);
	// Timer 주파수가 TIM4_FREQ가 되도록 PSC 설정
	TIM4->PSC = (unsigned int)(TIMXCLK/(double)TIM4_FREQ + 0.5)-1;
	// 요청한 주파수가 되도록 ARR 설정
	TIM4->ARR = (unsigned int)((double)TIM4_FREQ/freq + 0.5)-1;
	// Duty Rate 10%가 되도록 CCR3 설정
	TIM4->CCR3 = (unsigned int)(TIM4->ARR*(duty/10.) + 0.5)-1;
	// Manual Update(UG 발생)
	TIM4->EGR = 1<<0;
	// Timer Start
	Macro_Set_Bit(TIM4->CR1, 0);
}

void TIM4_Out_Init(void)
{
	Macro_Set_Bit(RCC->APB1ENR, 2);
	Macro_Set_Bit(RCC->APB2ENR, 3);
	Macro_Write_Block(GPIOB->CRH,0xf,0xe,0);
	Macro_Write_Block(TIM4->CCMR2,0xFF,0x68,0);
	TIM4->CCER = (0<<9)|(1<<8);
}