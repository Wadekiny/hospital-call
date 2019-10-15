#include "sys.h"
#include "led.h"
#include "delay.h"
#include "key.h"
#include <stdio.h>
#include "usart.h"

#define MCUADD 0X01
#define FINDADD 0X81
#define CMDADD 0X20
u8 solvesig = 0;
u8 now_state = 0;

int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();
	KEY_Init();
	uart_init(115200);
	LED_Init();
	
	while (1)
	{
		if (KEY_Scan() == KEY0P)
		{
			now_state = 1;
		} 
		if (solvesig == 1)
		{
			solvesig = 0;
			now_state = 0;
		}
		PCout(0) = !now_state;
	}	
}

