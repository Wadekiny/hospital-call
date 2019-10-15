/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       main.c
 * @brief      ɽ��K60 ƽ̨������
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-08-28
 */

#include "common.h"
#include "include.h"
#include "oled.h"
#define SLAVENUM 2

#define CMDADD 0x20  

uint8 slave_add[SLAVENUM] = {0x80,0x81};//���ĵ�ַ
uint8 warning_bed[SLAVENUM] = {0};//���еĴ�
uint8 note_bed[SLAVENUM] = {0};//�Ѿ������Է�����Ϣ
uint8 solve_bed[SLAVENUM] = {0};//�������Ĵ�
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
        if (ask_bed == SLAVENUM) ask_bed = 0;//ѯ�ʴ��ŵ���
        
        uart_putchar(UART4,slave_add[ask_bed]); //����4�������ŵ�ַ
        DELAY_MS(10);
        //������ʱ�ȴ��ӻ��ط�״ֵ̬
        //��Ӧ�����˳���Ĭģʽ������ch == now_state,
        //����uart4�жϣ�warning_bed[ask_bed]����״̬��0 || 1����
        if (solve_bed[ask_bed] == 1) //���ask_bed�Ѿ������
        {
            solve_bed[ask_bed] = 0;   //���
            note_bed[ask_bed] = 0;
            warning_bed[ask_bed] = 0;
            uart_putchar(UART4,CMDADD);//����4���ͽ��ָ��
        }
        uart_putchar(UART4,0x85);   //���дӻ����뾲Ĭģʽ
        
        //���û�������û�з�����ʾ��Ϣ
        if (warning_bed[ask_bed]==1 && note_bed[ask_bed]==0) 
        {
            printf("bed %d need help  ",ask_bed);//����3�����Է�����ʾ��Ϣ
            note_bed[ask_bed] = 1;         //������ʾ״̬
        }   
   
        if (!warning_bed[0] && !warning_bed[1])//�����������û������ ���
        {
            gpio_init(PTA17,GPO,1);
        }
        else
        {
           gpio_init(PTA17,GPO,0);
        }
    }
}
