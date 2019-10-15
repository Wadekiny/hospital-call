#include "encode.h"

void TIM4_EncodeInit()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//TIM_TimeBaseInitTypeDef  TIM_TimeBaseStruct;
	//TIM_ICInitTypeDef TIM_ICInitStruct;   	
//	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_StructInit(&GPIO_InitStruct);  
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 |GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	/*
	TIM_TimeBaseStructInit(&TIM_TimeBaseStruct); 
	TIM_DeInit(TIM4);
	TIM_TimeBaseStruct.TIM_Period = 0XFFFF;
	TIM_TimeBaseStruct.TIM_Prescaler = 0;
	TIM_TimeBaseStruct.TIM_ClockDivision =TIM_CKD_DIV1 ;
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStruct);   
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE );
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn;  //TIM3中断
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStruct);  //初始化NVIC寄存器
	
	//配置编码器模式触发源和极性
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);
	
	TIM_ICStructInit(&TIM_ICInitStruct);																																		//配置滤波器
	TIM_ICInitStruct.TIM_ICFilter = 6;
	TIM_ICInit(TIM4, &TIM_ICInitStruct);

	TIM_ClearFlag(TIM4, TIM_FLAG_Update);

	TIM_Cmd(TIM4, ENABLE);   //启动TIM4定时器
	*/
}
/*
void TIM4_IRQHandler(void)   //TIM3中断
{
	static int cnt = 0;
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx更新中断标志 
		cnt++;
		printf("cnt:%d\r\n",cnt);
	}
}
*/
