#include "device_driver.h"

void Key_Poll_Init(void)
{
    Macro_Set_Bit(RCC->APB2ENR, 3);
    Macro_Write_Block(GPIOB->CRL, 0xff, 0x88, 24);
	Macro_Set_Bit(GPIOA->ODR, 6);
	Macro_Set_Bit(GPIOA->ODR, 7);
}


/* 0: 디버깅용 설정 */
/* 1: 정상 동작용 설정 */

#if 1
    #define N   20000
#else
    #define N   3000000
#endif

#define KEY0_PUSH()   (Macro_Check_Bit_Clear(GPIOB->IDR, 6))
#define KEY1_PUSH()   (Macro_Check_Bit_Clear(GPIOB->IDR, 7))

int Key_Get_Pressed(void)
{
    int pre = 0;
    int cnt = 0;

    for (;;)
    {
        int key0 = KEY0_PUSH(); 
        int key1 = KEY1_PUSH(); 
        int new;

        if (key0 && key1) {
            new = 3;
        }
        else if (key0) {
            new = 1;
        }
        else if (key1) {
            new = 2;
        }
        else {
            new = 0;
        }

        if (new == pre)
        {
            cnt++;
            if (cnt >= N)
                return new;  
        }
        else
        {
            pre = new;
            cnt = 0; 
        }
    }
    return 0;
}

void Key_Wait_Key_Released(void)
{
    while(Key_Get_Pressed());
}

int Key_Wait_Key_Pressed(void)
{
    int k;

    while((k = Key_Get_Pressed()) == 0); //while(!(k = Key_Get_Pressed())) != 0);
    return k;
}
