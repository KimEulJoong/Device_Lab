#include "device_driver.h"

#define TIM2_TICK         	(20) 				// usec
#define TIM2_FREQ 	  		(1000000/TIM2_TICK)	// Hz
#define TIME2_PLS_OF_1ms  	(1000/TIM2_TICK)
#define TIM2_MAX	  		(0xffffu)

#if 0
#define TIM4_TICK	  		(20) 				// usec
#define TIM4_FREQ 	  		(1000000/TIM4_TICK) // Hz
#define TIME4_PLS_OF_1ms  	(1000/TIM4_TICK)
#endif

#define TIM4_MAX	  		(0xffffu)

#if 1 //과제용 4번
#define TIM4_TICK	  		(1000000/TIM4_FREQ) 				// usec
#define TIM4_FREQ 	  		(1000000) // Hz
#define TIME4_PLS_OF_1ms  	(1000/TIM4_TICK)
#endif


void TIM2_Stopwatch_Start(void)
{
	Macro_Set_Bit(RCC->APB1ENR, 0);

	TIM2->CR1 = (1<<4)|(1<<3);
	TIM2->PSC = (unsigned int)(TIMXCLK/50000.0 + 0.5)-1;
	TIM2->ARR = TIM2_MAX;

	Macro_Set_Bit(TIM2->EGR,0);
	Macro_Set_Bit(TIM2->CR1, 0);
}

unsigned int TIM2_Stopwatch_Stop(void)
{
	unsigned int time;

	Macro_Clear_Bit(TIM2->CR1, 0);
	time = (TIM2_MAX - TIM2->CNT) * TIM2_TICK;
	return time;
}

/* Delay Time Max = 65536 * 20use = 1.3sec */

#if 0

void TIM2_Delay(int time)
{
	Macro_Set_Bit(RCC->APB1ENR, 0);

	TIM2->CR1 = (1<<4)|(1<<3);
	TIM2->PSC = (unsigned int)(TIMXCLK/(double)TIM2_FREQ + 0.5)-1;
	TIM2->ARR = TIME2_PLS_OF_1ms * time;

	Macro_Set_Bit(TIM2->EGR,0);
	Macro_Clear_Bit(TIM2->SR, 0);
	Macro_Set_Bit(TIM2->DIER, 0);
	Macro_Set_Bit(TIM2->CR1, 0);

	while(Macro_Check_Bit_Clear(TIM2->SR, 0));

	Macro_Clear_Bit(TIM2->CR1, 0);
	Macro_Clear_Bit(TIM2->DIER, 0);
}

#else

/* Delay Time Extended */

// time msec 만큼 delay(깃발), 종료까지
void TIM2_Delay(int time)
{
	int i;
	unsigned int t = TIME2_PLS_OF_1ms * time;

	Macro_Set_Bit(RCC->APB1ENR, 0);

	TIM2->PSC = (unsigned int)(TIMXCLK/(double)TIM2_FREQ + 0.5)-1;
	TIM2->CR1 = (1<<4)|(1<<3);
	TIM2->ARR = 0xffff;
	Macro_Set_Bit(TIM2->EGR,0);
	Macro_Set_Bit(TIM2->DIER, 0);

	for(i=0; i<(t/0xffffu); i++)
	{
		Macro_Set_Bit(TIM2->EGR,0);
		Macro_Clear_Bit(TIM2->SR, 0);
		Macro_Set_Bit(TIM2->CR1, 0);
		while(Macro_Check_Bit_Clear(TIM2->SR, 0));
	}

	TIM2->ARR = t % 0xffffu;
	Macro_Set_Bit(TIM2->EGR,0);
	Macro_Clear_Bit(TIM2->SR, 0);
	Macro_Set_Bit(TIM2->CR1, 0);
	while (Macro_Check_Bit_Clear(TIM2->SR, 0));

	Macro_Clear_Bit(TIM2->CR1, 0);
	Macro_Clear_Bit(TIM2->DIER, 0);
}

#endif

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

#if 0 //원본, 과제 진행 시 비활성화
void TIM4_Repeat(int time)
{
	Macro_Set_Bit(RCC->APB1ENR, 2);

	TIM4->CR1 = (1<<4)|(0<<3);
	TIM4->PSC = (unsigned int)(TIMXCLK/(double)TIM4_FREQ + 0.5)-1;
	TIM4->ARR = TIME4_PLS_OF_1ms * time - 1;

	Macro_Set_Bit(TIM4->EGR,0);
	Macro_Clear_Bit(TIM4->SR, 0);
	Macro_Set_Bit(TIM4->DIER, 0);
	Macro_Set_Bit(TIM4->CR1, 0);
}

int TIM4_Check_Timeout(void)
{
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
	Macro_Clear_Bit(TIM4->DIER, 0);
}

void TIM4_Change_Value(int time)
{
	TIM4->ARR = TIME4_PLS_OF_1ms * time;
}
#endif

#define TIM3_FREQ 	  		(8000000) 	      	// Hz
#define TIM3_TICK	  		(1000000/TIM3_FREQ)	// usec
#define TIME3_PLS_OF_1ms  	(1000/TIM3_TICK)


void TIM3_Out_Init(void)
{
	Macro_Set_Bit(RCC->APB1ENR, 1);
	Macro_Set_Bit(RCC->APB2ENR, 3);
	Macro_Write_Block(GPIOB->CRL,0xf,0xb,0);
	Macro_Write_Block(TIM3->CCMR2,0x7,0x6,4);
	TIM3->CCER = (0<<9)|(1<<8);
}

// PWM출력 생성 함수 -> TIM3_OUT_FREQ = 8MHz, TIM3_TICK = 0.125us, Duty Rate 50%
void TIM3_Out_Freq_Generation(unsigned short freq)
{
	// Down Counter, Repeat Mode
	TIM3->CR1 = (0<<7)|(1<<4)|(0<<3);
	// Timer 주파수가 TIM3_FREQ가 되도록 PSC 설정
	TIM3->PSC = (unsigned int)(TIMXCLK/(double)TIM3_FREQ + 0.5)-1;
	// 요청한 주파수가 되도록 ARR 설정
	TIM3->ARR = (unsigned int)((double)TIM3_FREQ/freq + 0.5)-1;
	// Duty Rate 50%가 되도록 CCR3 설정
	TIM3->CCR3 = (unsigned int)(TIM3->ARR/2.0 + 0.5)-1;
	// Manual Update(UG 발생)
	TIM3->EGR = 1<<0;
	// Timer Start
	Macro_Set_Bit(TIM3->CR1, 0);
}

void TIM3_Out_Stop(void)
{
	Macro_Clear_Bit(TIM3->CR1, 0);
	Macro_Clear_Bit(TIM3->DIER, 0);
}

// 타이머4번 ---------------------------------------


void TIM4_Out_Init(void)
{
	Macro_Set_Bit(RCC->APB1ENR, 2);
	Macro_Set_Bit(RCC->APB2ENR, 3);
	Macro_Write_Block(GPIOB->CRH,0xf,0xe,0);
	Macro_Write_Block(TIM4->CCMR2,0xFF,0x68,0);
	TIM4->CCER = (0<<9)|(1<<8);
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

void TIM4_Out_Stop(void)
{
	Macro_Clear_Bit(TIM4->CR1, 0);
}

void TIM4_Stop(void)
{
	Macro_Clear_Bit(TIM4->CR1, 0);
	
}

void TIM4_Change_Duty(unsigned int duty)
{
	TIM4->CCR3 = (unsigned int)(TIM4->ARR*(duty/10.) + 0.5)-1;
	TIM4->EGR = 1<<0;
	Macro_Set_Bit(TIM4->CR1, 0);
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