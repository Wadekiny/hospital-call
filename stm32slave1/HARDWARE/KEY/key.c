#include "key.h"
#include "delay.h"
void KEY_Init()
{
	GPIO_InitTypeDef GPIO_init_structure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	
	GPIO_init_structure.GPIO_Pin = GPIO_Pin_2;
	GPIO_init_structure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOE, &GPIO_init_structure);
}

unsigned char KEY_Scan()
{
	static unsigned char key_flag = 0;
	if (!key_flag && !KEY0)
	{
		key_flag = 1;
		delay_ms(10);
		if (!KEY0) 
		{
			while (!KEY0);
			return KEY0P;
		}
	}
	else
	{
		key_flag = 0;
	}
	return NONE;
}
