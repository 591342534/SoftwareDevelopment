#include "include.h" 

unsigned int SPEED = 0;
//���      
unsigned int CENTER = 0;
unsigned int LEFT = 0;
unsigned int RIGHT = 0;


float angle = 0.0;    //���ת��ռ�ձ�






void main(void)
{
  

  DisableInterrupts;
  
  Init_All();
  
  DLY_ms(1000);
  PACNT=0;        //������������
	EnableInterrupts;

  
  while(1)
  {

    CENTER = ATD0DR0;
    RIGHT  = ATD0DR1;
    LEFT   = ATD0DR2;

  } 
  
  
 
}
