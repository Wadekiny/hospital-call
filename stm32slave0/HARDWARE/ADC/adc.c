#include "adc.h"

void AdcInit()
{
	ADC_InitTypeDef ADC_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); // 72/6 = 12
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	ADC_DeInit(ADC1);
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent; //独立模式
	ADC_InitStruct.ADC_ScanConvMode = DISABLE; 		//单通道模式
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;//单次转换模式
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//转换由软件启动
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;	//adc数据右对齐
	ADC_InitStruct.ADC_NbrOfChannel = 1; //顺序进行规则转换的adc通道数目
	ADC_Init(ADC1,&ADC_InitStruct);
	ADC_Cmd(ADC1,ENABLE);
	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1));
}

u16 GetADC(u8 ch)
{
	ADC_RegularChannelConfig(ADC1,ch,1,ADC_SampleTime_239Cycles5);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	while (!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
	return ADC_GetConversionValue(ADC1);
}

u16 GetADCAve(u8 ch, u8 times)
{
	u32 temp_val = 0;
	u8 t;
	for (t=0; t<times; t++)
	{
		temp_val += GetADC(ch);
		delay_ms(5);
	}
	return temp_val/times;
}
