#include "oled.h"
#include "oledfont.h"
//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127
uint8_t OLED_GRAM[128][8];	 

//�����Դ浽LCD		 
void OLED_Refresh_Gram(void)
{
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA); 
	}   
}

//��SSD1306д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED_WR_Byte(uint8_t dat,uint8_t cmd)
{	
	uint8_t i;			  
	OLED_DC=cmd; //д���� 
	OLED_CS=0;		  
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK=0;
		if(dat&0x80)OLED_SDIN=1;
		else OLED_SDIN=0;
		OLED_SCLK=1;
		dat<<=1;   
	}				 
	OLED_CS=1;		  
	OLED_DC=1;   	  
} 
	  	  
//����OLED��ʾ    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void OLED_Clear(void)  
{  
	uint8_t i,n;  
	for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;  
	OLED_Refresh_Gram();//������ʾ
}
//���� 
//x:0~127
//y:0~63
//t:1 ��� 0,���				   
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t)
{
	uint8_t pos,bx,temp=0;
	if(x>127||y>63)return;//������Χ��.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}
//x1,y1,x2,y2 �������ĶԽ�����
//ȷ��x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,���;1,���	  
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot)  
{  
	uint8_t x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
//	OLED_Refresh_Gram();//������ʾ
}
//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 12/16/24
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode)
{      			    
	uint8_t temp,t,t1;
	uint8_t y0=y;
	uint8_t csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���
	chr=chr-' ';//�õ�ƫ�ƺ��ֵ		 
    for(t=0;t<csize;t++)
    {   
		if(size==12)temp=asc2_1206[chr][t]; 	 	//����1206����
		else if(size==16)temp=asc2_1608[chr][t];	//����1608����
		else if(size==24)temp=asc2_2412[chr][t];	//����2412����
		else return;								//û�е��ֿ�
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}

//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//size:ѡ������ 12/16/24
//mode:0,������ʾ;1,������ʾ				 
//no:����������
void OLED_ShowChinese(uint8_t x,uint8_t y,uint8_t no,uint8_t size,uint8_t mode)
{      			    
	uint8_t temp,t,t1;
	uint8_t y0=y; 
	uint8_t csize=(size/8+((size%8)?1:0))*size;			//�õ�һ�����庺�ֶ�Ӧ������ռ���ֽ���
    for(t=0;t<csize;t++)
    { 
		if(size==12)		temp=HanZi_12[2*no+t/12][t%12]; 	//����1206����
		else if(size==16)	temp=HanZi_16[2*no+t/16][t%16];		//����1608����
		else if(size==24)	temp=HanZi_24[2*no+t/24][t%24];		//����2412����
		else return;											//û�е��ֿ�
							
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==12)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }     

}

//m^n����
uint32_t mypow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}				  

//��������λ��
uint8 mybit(uint32 m)
{
	uint8 t=0;
	while(++t)
	{
		if( m/mypow(10,t)==0 )
			break;	
	}
	return t;
}

//��ʾ����
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,��λ����0;1,��λ��0
//num:��ֵ(0~4294967295);	 
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode==1)	OLED_ShowChar(x+(size/2)*t,y,'0',size,1);
				else		OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
} 


//������ʾ����
void OLED_ShowNum_Inverse(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode==1)	OLED_ShowChar(x+(size/2)*t,y,'0',size,0);
				else		OLED_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 


void OLED_Showint(uint8 x,uint8 y,int data,uint8 len,uint8 size)
{
	int temp;
	if(data<0)
	{
		temp=-data;
		OLED_ShowChar(x,y,'-',size,1);
	}
	else 
	{
		temp=data;
		OLED_ShowChar(x,y,' ',size,1);
	}
	OLED_ShowNum(x+size/2,y,temp,len,size,0);
}


//��ʾ������
//x,y :�������
//num:��ֵ
//len :�����ַ�������������С���㣩
//size:�����С
//mode:0,������ʾ;1,������ʾ;	 
void OLED_ShowNumFloat(uint8_t x,uint8_t y,float num,uint8_t len,uint8_t size,uint8_t mode)
{         	
	uint8_t t,temp;
	uint8 len_I=0;			//������С�����ֵĳ���
	uint32  n=0;				

	//��ʾ��������
	n=(uint32)num;
	len_I=mybit(n);							//�������ֵ���ʾλ��
	for(t=0;t<len_I;t++)
	{
		temp=(n/mypow(10,len_I-t-1))%10;
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,mode); 
	}
	
	//��ʾС������
	n=(uint32)(num*mypow(10,len-len_I))%mypow(10,len-len_I);		//ȡ��С������
	if(len-len_I)													//�ж���ʾС����
		OLED_ShowChar(x+(size/2)*t,y,'.',size,1);
	
	for(t=len_I;t<len;t++)
	{
		temp=(n/mypow(10,len-t-1))%10;
	 	OLED_ShowChar(x+(size/2)*(t+1),y,temp+'0',size,mode); 
	}
	
}

//��ʾ�ַ���
//x,y:�������  
//size:�����С 
//*p:�ַ�����ʼ��ַ 
void OLED_ShowString(uint8_t x,uint8_t y,const uint8_t *p,uint8_t size)
{	
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>(128-(size/2))){x=0;y+=size;}
        if(y>(64-size)){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,size,1);	 
        x+=size/2;
        p++;
    }  
}	

//������ʾ�ַ���
void OLED_ShowString_Inverse(uint8_t x,uint8_t y,const uint8_t *p,uint8_t size)
{	
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>(128-(size/2))){x=0;y+=size;}
        if(y>(64-size)){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,size,0);	 
        x+=size/2;
        p++;
    }  
}	

//��ʾͼ����
//��img_x��img_yΪͼ�����ؿ��,�߶�
//void Display_Image(uint8_t img_x,uint8_t img_y,uint8_t *image)
//{
//		
//	uint8_t temp,t,i,j;
//	uint8_t x1;
//	uint8_t x;
//	if(img_x%8==0) x1=img_x/8;      
//	else x1=img_x/8+1;
//	
//	for(j=0;j<img_y;j++)
//	{
//		for(i=0;i<x1;i++)
//		{   
//			x=0;
//			temp=image[j*10+i]; 	 					
//			for(t=0;t<8;t++)
//			{
//				if(temp&0x80)OLED_DrawPoint(i*8+x,j,0);
//				else OLED_DrawPoint(i*8+x,j,1);
//				temp<<=1;
//				x++;
//			}  	 
//		}
//	} 
//	OLED_Refresh_Gram();
//}

//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//wide:ͼƬ���ؿ��
//hight:ͼƬ���ظ߶�
//mode:0,������ʾ;1,������ʾ	
//BMP:ͼƬ������
void OLED_ShowBMP(uint8_t x,uint8_t y,uint8_t wide,uint8_t hight,uint8_t mode,unsigned char* BMP)
{      			    
	uint8_t temp,t,i,j;
	uint8_t y0=y; 
	uint8_t x0=x;
	uint8_t y1;
	
	if(hight%8==0) y1=hight/8;      
	else y1=hight/8+1;
	
	for(i=0;i<wide;i++)
	{
		y=y0;
		x=x0+i;
		for(j=0;j<y1;j++)
		{   
			temp=BMP[i*8+j]; 	 					
			for(t=0;t<8;t++)
			{
				if(temp&0x80)OLED_DrawPoint(x,y,mode);
				else OLED_DrawPoint(x,y,!mode);
				temp<<=1;
				y++;
				if(y>64) break;
			}  	 
		}
	}     
}

//��ʼ��SSD1306					    
void OLED_Init(void)
{ 	 	

    gpio_init  (CS,  GPO, 1); //��ʼ��SCL    
    gpio_init  (RST, GPO, 1); //��ʼ��SDA    
    gpio_init  (DC,  GPO, 1); //��ʼ��SCL    
    gpio_init  (SCLK,GPO, 1); //��ʼ��SDA   
    gpio_init  (SDIN,GPO, 1); //��ʼ��SCL

    port_init_NoALT(CS,  ~ODO | PULLUP);
    port_init_NoALT(RST, ~ODO | PULLUP);
    port_init_NoALT(DC,  ~ODO | PULLUP);
    port_init_NoALT(SCLK,~ODO | PULLUP);
    port_init_NoALT(SDIN,~ODO | PULLUP);
      
	OLED_CS=1;
	OLED_DC=1;	 
//��λ���Ž�оƬ��λ���ſ�ע��	
	OLED_RST=0;
	DELAY_MS(100);
	OLED_RST=1; 
//////////////////////////////	
    
	OLED_WR_Byte(0xAE,OLED_CMD); //�ر���ʾ
	OLED_WR_Byte(0xD5,OLED_CMD); //����ʱ�ӷ�Ƶ����,��Ƶ��
	OLED_WR_Byte(80,OLED_CMD);   //[3:0],��Ƶ����;[7:4],��Ƶ��
	OLED_WR_Byte(0xA8,OLED_CMD); //��������·��
	OLED_WR_Byte(0X3F,OLED_CMD); //Ĭ��0X3F(1/64) 
	OLED_WR_Byte(0xD3,OLED_CMD); //������ʾƫ��
	OLED_WR_Byte(0X00,OLED_CMD); //Ĭ��Ϊ0

	OLED_WR_Byte(0x40,OLED_CMD); //������ʾ��ʼ�� [5:0],����.
													    
	OLED_WR_Byte(0x8D,OLED_CMD); //��ɱ�����
	OLED_WR_Byte(0x14,OLED_CMD); //bit2������/�ر�
	OLED_WR_Byte(0x20,OLED_CMD); //�����ڴ��ַģʽ
	OLED_WR_Byte(0x02,OLED_CMD); //[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
	OLED_WR_Byte(0xA1,OLED_CMD); //���ض�������,bit0:0,0->0;1,0->127;
	OLED_WR_Byte(0xC0,OLED_CMD); //����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��
	OLED_WR_Byte(0xDA,OLED_CMD); //����COMӲ����������
	OLED_WR_Byte(0x12,OLED_CMD); //[5:4]����
		 
	OLED_WR_Byte(0x81,OLED_CMD); //�Աȶ�����
	OLED_WR_Byte(0xEF,OLED_CMD); //1~255;Ĭ��0X7F (��������,Խ��Խ��)
	OLED_WR_Byte(0xD9,OLED_CMD); //����Ԥ�������
	OLED_WR_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_WR_Byte(0xDB,OLED_CMD); //����VCOMH ��ѹ����
	OLED_WR_Byte(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_WR_Byte(0xA4,OLED_CMD); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
	OLED_WR_Byte(0xA6,OLED_CMD); //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ	    						   
	OLED_WR_Byte(0xAF,OLED_CMD); //������ʾ	 
	OLED_Clear();
}  

