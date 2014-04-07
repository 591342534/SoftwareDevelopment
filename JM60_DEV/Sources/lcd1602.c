#include "lcd1602.h"


//����CGROM�ж����ַ���λ�ú��ַ���ASCII����ͬ����˿���ֱ�Ӹ�ֵ
unsigned char const Menu1[] = "slchen@tju.edu.c";
//��ʾ�����Թ�+12345+�����Թ�
unsigned char const Text[] ={0x01,0x01,0x01,0x31,0x32,0x33,0x34,0x35,0x01,0x01,0x01,0x00};

unsigned char const CGRAM_0[8] = {0x00,0x00,0x01,0x02,0x14,0x08,0x00,0x00};

void time(uint t)
{
  uint i,j;
  for(i=0;i<t;i++)
  {
    for(j=0;j<100;j++);
  }
}

void PORT_INIT(void)
{
  PTFDD |=0x13; //F0,F1,F4Ϊ���
  PTEDD = 0xFF; //PTE����Ϊ�����
}

void LCD_INIT()   //��ʼ��
{	
   unsigned char i;
   
   PORT_INIT();
   
   LCD_COMMAND_WRITE(LCD_FUNCTION_SET);
	 time(30);                                                         
   LCD_COMMAND_WRITE(LCD_FUNCTION_SET);
	 time(6);                                                         
   LCD_COMMAND_WRITE(LCD_FUNCTION_SET);
	 time(6);                                                         
   LCD_COMMAND_WRITE(LCD_FUNCTION_SET);
	 LCD_BUSY_CHECK();
   //LCD_COMMAND_WRITE(LCD_ONOFF_SET|DisplayOn|CursorOn|CursorBlankingOn);
   LCD_COMMAND_WRITE(LCD_ONOFF_SET|DisplayOn); 
	 LCD_BUSY_CHECK();
	 LCD_COMMAND_WRITE(LCD_CLR_RET); 
	 LCD_BUSY_CHECK();
	 LCD_COMMAND_WRITE(LCD_ENTRY_MODE|ACInc);

   LCD_BUSY_CHECK();
   LCD_COMMAND_WRITE(LCD_CGRAM_ADD_SET+0x08);   
//���Զ����ַ�д��DDRAM��0x01��
	for(i = 0;i<8;i++)
	{ 	
	  LCD_BUSY_CHECK();
		LCD_DATA_WRITE(CGRAM_0[i]);	
	}
}


void LCD_COMMAND_WRITE(unsigned char LCD_COMMAND)  //д�����
{
	LCD_E	=0;
	LCD_DATAPORT = LCD_COMMAND;
	LCD_RS 	= 0;	//�����״̬
  LCD_RW 	= 0;	//ִ��д����
  asm ("nop; nop;nop;nop;"); //��ַ����ʱ��
 	LCD_E	= 1;
  asm ("nop; nop;nop;nop;"); //ʱ���������
	LCD_E	=0;		//��������
  asm ("nop; nop;nop;nop;");	//��ַ����ʱ��
  LCD_RW 	= 1;	//�ص���״̬
}

//��ȡҺ��״̬��Ϣ
unsigned char LCD_STATUS_READ()
{
	unsigned char status;
	LCD_E	=0;
	//LCD_DATAPORT = 0xff;//����Ϊ�˿ڶ�״̬
	LCD_DATA_DIR = 0;
	
	LCD_RS 	= 0;	//�����״̬
  LCD_RW 	= 1;	//ִ�ж�����
  asm ("nop; nop;nop;nop;"); //��ַ����ʱ��
	LCD_E	= 1;
  asm ("nop; nop;nop;nop;"); //ʱ���������
      //���ݽ���ʱ��
	status 	= LCD_DATAPORT;//��ȡ����
	LCD_E	= 0;		
  asm ("nop; nop;nop;nop;"); //��ַ����ʱ��
  LCD_RW 	= 1;	//�ص���״̬
	
	LCD_DATA_DIR = 0xFF;
	return status;
}


//д���ݺ���
void LCD_DATA_WRITE(unsigned char LCD_DATA)  
{
	LCD_E	=0;
	LCD_DATAPORT = LCD_DATA;
	LCD_RS 	= 1;	//���ݲ���
  LCD_RW 	= 0;	//ִ��д����
  asm ("nop; nop;nop;nop;"); //��ַ����ʱ��
	LCD_E	= 1;
  asm ("nop; nop;nop;nop;"); //ʱ���������
	LCD_E	=0;		//��������
  asm ("nop; nop;nop;nop;"); //��ַ����ʱ��
  LCD_RW 	= 1;	//�ص���״̬
}



unsigned char LCD_DATA_READ(void)
{
	unsigned char ReadData;
	LCD_E	=0;
	//LCD_DATAPORT = 0xff;//����Ϊ�˿ڶ�״̬
	LCD_DATA_DIR = 0;
	
	LCD_RS 	= 1;	//���ݲ���
  LCD_RW 	= 1;	//ִ�ж�����
  asm ("nop; nop;nop;nop;"); //��ַ����ʱ��
	LCD_E	= 1;
  asm ("nop; nop;nop;nop;"); //ʱ���������
	ReadData 	= LCD_DATAPORT;//��ȡ����
	LCD_E	= 0;		
  asm ("nop; nop;nop;nop;"); //��ַ����ʱ��
  LCD_RW 	= 1;	//�ص���״̬
	LCD_DATA_DIR = 0xFF;
	return ReadData;
}



void LCD_SET_ADDRESS(unsigned char DISP_PAGE,unsigned char DISP_COL)
{
	unsigned char col;
	col = DISP_COL;
	if( col > 15 ) col = 15;
	LCD_BUSY_CHECK();
	if(DISP_PAGE == 0)
		 LCD_COMMAND_WRITE(LCD_DDRAM_ADD_SET + col);  
 //д��ʾ��λ��
	else 	
	   LCD_COMMAND_WRITE(LCD_DDRAM_ADD_SET + 0x40 + col);
}



void LCD_CLS_ALL()   //����
{
	 LCD_BUSY_CHECK();
	 LCD_COMMAND_WRITE(LCD_CLR_RET); 
	 LCD_BUSY_CHECK();
}



void LCD_Print(unsigned char DISP_PAGE,unsigned char DISP_COL,unsigned char* p)
{
	unsigned char i,DisCnt;
	unsigned char col;
	col = DISP_COL;
	if(col>15)	
	  col = 15;
	
	DisCnt = 16-col;
	
	LCD_BUSY_CHECK();
	LCD_SET_ADDRESS(DISP_PAGE,DISP_COL);
	for(i = 0;i<DisCnt;i++)
	{
	 	LCD_BUSY_CHECK();
		if(p[i]!=0)					//�ַ�����0x00��β
			LCD_DATA_WRITE(p[i]);	
		else break;         //���һд�����������
	}	
}
