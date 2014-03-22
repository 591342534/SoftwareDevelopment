#include "tools.h"

/************************Delay Func***************************/
void delay(uint ms)
{
  uint i=0, j=0;
  for(i=0;i<ms;i++)
  {
    for(j=0;j<370;j++)
    {
      __asm nop;
    }
  }
}

//��ȷ��ʱ����λ10ms���迪��RTC
void wait(uint ten_ms)
{
   ulong temp = T;
   while(T - temp <= ten_ms);
}
/************************Math Func************************/
int abs(int x)
{
  if(x < 0)
    return (0-x);
  else
    return x;
}

byte average(byte* data, uint len)
{
  uint i = 0;
  uint sum = 0;
  for(i=0; i<len; i++)
  {
     sum += data[i];
  }
  return sum/len;
}

byte bcd2dec(byte bcd)
{
  byte t = 0;//ʮλ
  byte u = 0;//��λ
  
  u = bcd & 0x0F;
  t = bcd >> 4;
  return (t * 10 + u);
}

/************************SCI***************************/
void sci_senddata(byte d)
{
   SCI1D = d;
   while(!SCI1S1_TDRE);
}

void sci_sendchar(char c)
{
   SCI1D = c;
   while(!SCI1S1_TDRE);
}

void sci_sendstr(char *s)
{
  while(*s)
  {
    sci_sendchar(*s++);
  }
}

/************************IIC***************************/
                                                
void delay_s(void)
{               //ÿ�������ݶ�ȡ֮����Ҫ���һ��ʱ��
  uchar i;
  for(i=0;i<10;i++);
}

void IIC_WRITE(byte DeviceAdd,byte RegAdd,byte Cnt,byte* p)
{
    byte temp,i;
   
    temp = IICS;       //���IIC�жϱ�־λ                                          
    IICS_IICIF = 1;
    
    IICC1_MST=0;       
    IICC1_TX=1;         //IIC��������ʹ��
    IICC1_MST=1;        //��Ϊ��ģʽ��MST��0���1����S�ź�
         
    IICD=DeviceAdd;     //����оƬ��ַ��д����
    while(!IICS_IICIF );
    IICS_IICIF=1;        //�ȴ�������ɣ����ж�
    while(IICS_RXAK);    //�ȴ���������ӦACK
    
    IICD=RegAdd;       //���ͼĴ�����ַ
    while(!IICS_IICIF);
    IICS_IICIF=1;        //�ȴ�������ɣ����ж�
    while(IICS_RXAK);   //�ȴ���������ӦACK
    
    for(i=0;i<Cnt;i++) 
    {   //��������N�ֽ�����
      IICD = p[i];
      while(!IICS_IICIF);
      IICS_IICIF=1;
      while(IICS_RXAK);  //�ȴ���������ӦACK������дN���ֽڣ�ÿ�ζ�ҪӦ��    
    }
    
    IICC1_MST=0;      //MST��1��Ϊ0������P�ź�  
    delay_s();
}

void IIC_READ(byte DeviceAdd,byte RegAdd,byte Cnt,byte* p)
{
    byte i,temp;
    IICC1_RSTA=0;     //���ظ���ʼ�źŸ�λ
     
    temp = IICS;       //���ж��ź����
    IICS_IICIF = 1;
    
    IICC1_MST=0;
    IICC1_TX=1;       //дʹ��
    IICC1_MST=1;      //��Ϊ��ģʽ������S�ź�
    
    IICD=DeviceAdd ;   //����оƬ��ַ��д����
    while(!IICS_IICIF);
    IICS_IICIF=1;       //�ȴ�������ɣ����ж�
    while(IICS_RXAK);   //�ȴ���������ӦACK		
    
    IICD=RegAdd;        //���ͼĴ�����ַ
    while(!IICS_IICIF);     //�ȴ�������ɣ����ж�
    IICS_IICIF=1;
    while(IICS_RXAK);     //�ȴ���������ӦACK

    if(Cnt>1) IICC1_TXAK = 0;      //�������ֻ����һ���ֽڣ�������NACK�ź�
    else      IICC1_TXAK = 1; 	  //��������������ͼ����ֽڣ�����ACK�ź�
    
    IICC1_RSTA=1;         //�����ظ���ʼ�ź�
    
    IICD = DeviceAdd+1;   //����оƬ��ַ,������   
    while(!IICS_IICIF);      //�ȴ�������ɣ����ж�
    IICS_IICIF=1;
    while(IICS_RXAK);     //�ȴ���������ӦACK
    
    temp=IICD;    //���������豸��ʹ��ʱ����Ҫ�ȶ�IICD��������������Ҫ�Ĵ���
    IICC1_TX=0;    //��ʹ��

    if(Cnt>1)
    {
        for(i=0;i<Cnt-1;i++)
        {
          while(!IICS_IICIF); //�Ƿ���յ�����
          IICS_IICIF=1;
          p[i] = IICD;        //��������
        }
        IICC1_TXAK = 1;     //���һ���ֽڣ�����NACK
    }
    while(!IICS_IICIF);      //�Ƿ���յ�����
    IICS_IICIF=1;
    IICC1_MST=0;         //����ֹͣ�ź�
    p[Cnt-1] = IICD;        //��������
}

                          

