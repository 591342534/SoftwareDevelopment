#include "include.h"

//ȫ��ʱ�����,ÿ10ms��1
ulong T = 0;

//�������ʾ���ñ�־
//0 ��ʾʱ��оƬ����
//1 ��ʾSCI���յ�������
//ͬʱ����key4 key5�ı��λ
//byte g_segflag = 0;

void main(void) 
{

  byte test[5] = {1,2,3,4,5};
  byte res[5] = {0};

  DisableInterrupts;
  ALL_Init();
  EnableInterrupts; 

  //seg_display(1234);
  
  PCF8563_Init();
  
  HCS08_EE_Init();//��ʼ��Flash���ƼĴ����
  HCS08_EE_WriteData(test, 5);
  HCS08_EE_ReadData(res, 5);
    


  while(TRUE)
  {  
     //sci_sendstr("What the fuck!\r\n");
     //sci_senddata(0x20);
     //flash_led();
     PCF8563_Read_Info();
     seg_display(100*(uint)bcd2dec(Minutes)+(uint)bcd2dec(Seconds));
  }

}
