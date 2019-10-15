/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       main.c
 * @brief      山外K60 平台主程序
 * @author     山外科技
 * @version    v5.0
 * @date       2013-08-28
 */

#include "common.h"
#include "include.h"
#include "oled.h"
#define SLAVENUM 2

#define CMDADD 0x20  

uint8 slave_add[SLAVENUM] = {0x80,0x81};//床的地址
uint8 warning_bed[SLAVENUM] = {0};//呼叫的床
uint8 note_bed[SLAVENUM] = {0};//已经给电脑发送消息
uint8 solve_bed[SLAVENUM] = {0};//解决问题的床
uint8 ask_bed = 0;
void main()
{
    uart_init(UART3,115200);
    uart_init(UART4,115200);
    set_vector_handler(UART3_RX_TX_VECTORn,uart3_test_handler);
    set_vector_handler(UART4_RX_TX_VECTORn,uart4_test_handler);
    uart_rx_irq_en(UART3);
    uart_rx_irq_en(UART4);
    OLED_Init();
   OLED_ShowNum_Inverse(0,0,1,1,12,1);
   OLED_Refresh_Gram();

    while (1)
    {
        ask_bed++;      
        if (ask_bed == SLAVENUM) ask_bed = 0;//询问床号递增
        
        uart_putchar(UART4,slave_add[ask_bed]); //串口4发出床号地址
        DELAY_MS(10);
        //短暂延时等待从机回发状态值
        //对应床号退出静默模式，返回ch == now_state,
        //进入uart4中断，warning_bed[ask_bed]更新状态（0 || 1）；
        if (solve_bed[ask_bed] == 1) //如果ask_bed已经被解决
        {
            solve_bed[ask_bed] = 0;   //清空
            note_bed[ask_bed] = 0;
            warning_bed[ask_bed] = 0;
            uart_putchar(UART4,CMDADD);//串口4发送解决指令
        }
        uart_putchar(UART4,0x85);   //所有从机进入静默模式
        
        //如果没被解决，没有发送提示消息
        if (warning_bed[ask_bed]==1 && note_bed[ask_bed]==0) 
        {
            printf("bed %d need help  ",ask_bed);//串口3给电脑发送提示消息
            note_bed[ask_bed] = 1;         //更新提示状态
        }   
   
        if (!warning_bed[0] && !warning_bed[1])//如果两个床都没有问题 灭灯
        {
            gpio_init(PTA17,GPO,1);
        }
        else
        {
           gpio_init(PTA17,GPO,0);
        }
    }
}
