#include "dma.h"

DMA_InitTypeDef DMA_InitStruct; 
u16 DMA1_MEM_LEN;

void USART1DMA_Init(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	DMA_DeInit(DMA_CHx);
	DMA1_MEM_LEN = cndtr;
	DMA_InitStruct.DMA_PeripheralBaseAddr = cpar;
	DMA_InitStruct.DMA_MemoryBaseAddr = cmar;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStruct.DMA_BufferSize = cndtr;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA_CHx,&DMA_InitStruct);
}

void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx) 
{
	DMA_Cmd(DMA_CHx, DISABLE); 
	DMA_SetCurrDataCounter(DMA1_Channel4,DMA1_MEM_LEN);
	DMA_Cmd(DMA_CHx, ENABLE); 
}

