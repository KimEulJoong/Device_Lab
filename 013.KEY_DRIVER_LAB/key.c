#include "device_driver.h"

void Key_Poll_Init(void)
{
	Macro_Set_Bit(RCC->APB2ENR, 3); //B포트사용
	Macro_Write_Block(GPIOB->CRL, 0xff, 0x44, 24);
}

#define KEY0_PUSH() 	(Macro_Check_Bit_Clear(GPIOB->IDR, 6))
#define KEY0_REL() 		(Macro_Check_Bit_Set(GPIOB->IDR, 6))
#define KEY1_PUSH() 	(Macro_Check_Bit_Clear(GPIOB->IDR, 7))
#define KEY1_REL() 		(Macro_Check_Bit_Set(GPIOB->IDR, 7))

#define KEY_VALUE()  (Macro_Extract_Area(~GPIOB->IDR, 0x3, 6))

int Key_Get_Pressed(void)
{	
	if( KEY0_REL() &&  KEY1_REL() ) return 0;
	if( KEY0_PUSH() && KEY1_REL() ) return 1;
	if( KEY0_REL() && KEY1_PUSH() ) return 2;
	if( KEY0_PUSH() && KEY1_PUSH() ) return 3;

	return 0;
}

void Key_Wait_Key_Released(void)
{

	while (Key_Get_Pressed() != 0);
	
}

int Key_Wait_Key_Pressed(void)
{

	int key;
    while ((key = Key_Get_Pressed()) == 0); 
    return key;
}
