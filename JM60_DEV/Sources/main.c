#include "include.h"

//ȫ��ʱ�����,ÿ10ms��1
ulong T = 0;
//��Ļ��ʾ�ַ�������
char buff[20]= {0};

//�������ʾ���ñ�־
//0 ��ʾʱ��оƬ����
//1 ��ʾSCI���յ�������
//ͬʱ����key4 key5�ı��λ
//byte g_segflag = 0;

void main(void) 
{
  byte segnum = 0;
  int DispCnt = 0;
  

  DisableInterrupts;
  ALL_Init();
  EnableInterrupts; 

  
   /*
  
  //HCS08_EE_WriteRecord(&segnum);  //Init segnum
  HCS08_EE_ReadRecord(&segnum);
  seg_display(segnum);
  
  sprintf(buff, "The %dth time on", segnum); 
  LCD_CLS_ALL();   //����
  LCD_Print(0,0,buff);        //����������д��Һ����һ��
  
  segnum++;
  HCS08_EE_WriteRecord(&segnum);
  
  wait(200);
   */
  while(TRUE)
  { 
  
  
    //sci_sendstr("What the fuck!\r\n");
    //sci_senddata(0x20);
    
    //flash_led();
    
    PCF8563_Read_Info();
    //seg_display((uint)bcd2dec(Seconds));
    
    sprintf(buff, "%02d:%02d", (uint)bcd2dec(Minutes),(uint)bcd2dec(Seconds)); 
    LCD_CLS_ALL();   //�������
    LCD_Print(0,0,buff);        //����������д��Һ����һ��
    /*
    LCD_Print(1,0,Text+(DispCnt%16));     //�����ݵڶ�����ʾ����forѭ����//�ù�����Ч��
    DispCnt++;
    
    delay(500); */
  }

}
