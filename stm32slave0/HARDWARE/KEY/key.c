#include "key.h"
#include "delay.h"
void KEY_Init()
{
	GPIO_InitTypeDef GPIO_init_structure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOE, ENABLE);
	
	GPIO_init_structure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_init_structure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOE, &GPIO_init_structure);
	
	GPIO_init_structure.GPIO_Pin = GPIO_Pin_0;
	GPIO_init_structure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOA, &GPIO_init_structure);
}

unsigned char KEY_Scan()
{
	static unsigned char key_flag = 0;
	if (!key_flag && (!KEY0 || !KEY1 || KEYUP))
	{
		key_flag = 1;
		delay_ms(10);
		if (!KEY0) 
		{
			while (!KEY0);
			return KEY0P;
		}
		else if (!KEY1) return KEY1P;
		else if (KEYUP) return KEYUPP;
	}
	else
	{
		key_flag = 0;
	}
	return NONE;
}
