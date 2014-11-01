#include "include.h"

extern CENTER;
extern LEFT;
extern RIGHT;
extern SPEED;

extern T ;

extern status; 
                           
extern status_knob;   
                                                    
extern last_press_time;  
extern last_turn_time;   
extern value;    


unsigned char status_button=0;  //��ť����״̬����
char mystr[20]="";              //��Ļ��ʾ�ַ���������
char temp_status=1;             //���Խ׶�ĳ�������е�״̬��Ǳ���

//�������,��ʱ�ȷŵ����ļ��ڣ����Թ�����ת�Ƶ��㷨���Ƶ�c�ļ���

int pre_error = 0;                //��һ�β������ٶ�ƫ��
int d_pre_error = 0;              //��һ�β������ٶ�ƫ���΢��
unsigned short int maxspeed = 1000;//������٣������ظ�����
unsigned short int minspeed = 0;   //������٣������ظ�����
unsigned short int target = 0;    //Ŀ���ٶ�
char Kp = 10;                     //PID���ٿ��Ƶı���ϵ��
char Ki = 2;                      //PID���ٿ��ƵĻ���ϵ��
char Kd = 2;                      //PID���ٿ��Ƶ�΢��ϵ��
int error = 0;              //ʵ���ٶ���Ŀ���ٶȵ�ƫ��
int d_error = 0;            //�ٶ�ƫ���΢��
int dd_error = 0;           //�ٶ�ƫ��Ķ���΢��
int result_motor=0;         //PID�ļ�����

#define steering PWMDTY67


unsigned char check_button(void)  //�����ť�����Ƿ��µĺ�������ť1�����򷵻�1����ť2�����򷵻�2����ť3�����򷵻�3�����򷵻�0
                                  //������������ȹ�ע
{
  if(T-last_press_time>30)
  {
    if(!PTS_PTS3)
    {
      last_press_time=T;
      while(!PTS_PTS3)             //����ť1������
      {
        if(T - last_press_time > 10)//ά�ְ���״̬��ʱ�䳬��10/100=100ms������Ϊ��ť1������
        {
          LCD_CLS();
          return 1;                 //����1
        }
      }
    }
    else if(!PORTE_PE3)
    {
      last_press_time=T;
      while(!PORTE_PE3)               //����ť2������
      {
        if(T - last_press_time > 10)  //ά�ְ���״̬��ʱ�䳬��10/100=100ms������Ϊ��ť2������
        {
          LCD_CLS();
          return 2;                   //����2
        }
      }
    }
    else if(!PORTE_PE5)
    {
      last_press_time=T;
      while(!PORTE_PE5)               //����ť3������
      {
        if(T - last_press_time > 10)  //ά�ְ���״̬��ʱ�䳬��10/100=100ms������Ϊ��ť3������
        {
          LCD_CLS();
          return 3;                   //����3
        }
      }
    }
  }
  return 0;                       //����ť1��2��û�б��������³���100ms������Ϊû����ť�����£�����0
}

void wait(unsigned long time)  //��ʱ����������Ϊ��ʱʱ�䣬��λ��1/100 s
{
  unsigned long temptime = T;
  while(T - temptime < time);
}

void Debug(void)

{
  LCD_P6x8Str(0,0,"TJU Intelligent Car"); //��Ļ��1��3��5��7����ʾTJU Intelligent Car
  LCD_P6x8Str(0,2,"TJU Intelligent Car");
  LCD_P6x8Str(0,4,"TJU Intelligent Car");
  LCD_P6x8Str(0,6,"TJU Intelligent Car");
  
  wait(100);         //��ʱ1s
  
  LCD_CLS();         //�����Ļ

  LCD_P6x8Str(0,0,"Left=");
  LCD_P6x8Str(0,1,"Center=");
  LCD_P6x8Str(0,2,"Right=");
  
  for(;;)  //�����������Խ׶Σ������ǵ��Խ׶ε���ѭ���������ʵ�ʱ��break����֮��С��������
  {
    //����ǰС���ĵ���״̬status��0Ϊ��Ļ��ʾ3����вɼ������źţ�����ADת�������ֵ
    //1Ϊ���������ֵ��2Ϊ����PID������3Ϊ�趨�����ٶȣ�4����
    
    if(status==0)       //��Ļ��ʾ����ź�ֵ
    {
      sprintf(mystr,"%04d",LEFT);
      LCD_P6x8Str(30,0,mystr);
      sprintf(mystr,"%04d",CENTER);
      LCD_P6x8Str(42,1,mystr);
      sprintf(mystr,"%04d",RIGHT);
      LCD_P6x8Str(36,2,mystr);
      
      status_button=check_button();
      if(status_button==1)      //������ť1������������׶Σ��Ƚ����������׶�
      {
        value=steering;
        LCD_P6x8Str(0,7,"STEER=");
        status++;
        TFLG1=0xFF;
        TIE = 0b111;          //ʹ����ת�������жϣ���ֹ�г��ж�
      }
      else if(status_button==2)//������ť2�����ٶ��趨�׶Σ�׼������
      {
        LCD_P6x8Str(0,6,"MAX=");
				LCD_P6x8Str(0,7,"MIN=");
        status=3;
        TFLG1=0xFF;
        TIE = 0b111;          //ʹ����ת�������жϣ���ֹ�г��ж�
      }
    }
    else if(status==1)        //���������ֵ����ת��ť1�����������ת��ť2�ֵ����
    {
      switch(status_knob)
      {
        case 1:
        {
          value += 10;
          status_knob=0;
          break;
        }
        case 2:
        {
          value -= 10;
          status_knob=0;
          break;
        }
        case 3:
        {
          value += 100;
          status_knob=0;
          break;
        }
        case 4:
        {
          value -= 100;
          status_knob=0;
          break;
        }
      }
      steering=value;
      
      sprintf(mystr,"%4d",value);
			LCD_P6x8Str(36,7,mystr);
			
      status_button=check_button();//������ť1����PID���Խ׶�
      if(status_button==1)
      {
        value=target;
        status++;
        LCD_P6x8Str(0,2,"*Kp=");
				LCD_P6x8Str(0,3," Ki=");
				LCD_P6x8Str(0,4," Kd=");
				LCD_P6x8Str(0,5," target=");
				LCD_P6x8Str(0,6," PWM=");
				LCD_P6x8Str(0,7," speed=");        
      }
      else if(status_button==2)//������ť2�ص���������ֵ��ʾ�׶�
      {
        status=0;
        LCD_P6x8Str(0,0,"Left=");
        LCD_P6x8Str(0,1,"Center=");
        LCD_P6x8Str(0,2,"Right=");        
        TFLG1=0xFF;
        TIE=0x00;
      }
    }
    
    else if(status==2)  //����PID��������ת��ť1���ڵ���ѡ���Ĳ�������ť2����ѡ�������ĸ�����
    {
      switch(status_knob)
      {
        case 1:
        {
          switch(temp_status)
          {
            case 1:
            {
              Kp++;
              if(Kp>30)
                Kp=30;
      				break;
            }
            case 2:
            {
              Ki++;
              if(Ki>30)
                Ki=30;
      				break;
            }
            case 3:
            {
              Kd++;
              if(Kd>30)
                Kd=30;
      				break;
            }
            case 4:
            {
              value+=100;
              if(value>1000)
                value=1000;
              target=value;
      				break;
            }
          }
          status_knob=0;
          break;
        }
        case 2:
        {
          switch(temp_status)
          {
            case 1:
            {
              Kp--;
              if(Kp<0)
                Kp=0;
      				break;
            }
            case 2:
            {
              Ki--;
              if(Ki<0)
                Ki=0;
      				break;
            }
            case 3:
            {
              Kd--;
              if(Kd<0)
                Kd=0;
      				break;
            }
            case 4:
            {
              value-=100;
              if(value<0)
                value=0;
              target=value;
      				break;
            }
          }
          status_knob=0;
          break;
        }
        case 3:
        {
          temp_status++;
          if(temp_status>4)
            temp_status=1;
          switch(temp_status)
          {
            case 1:
            {
              LCD_P6x8Str(0,2,"*Kp=");
      				LCD_P6x8Str(0,3," Ki=");
      				LCD_P6x8Str(0,4," Kd=");
      				LCD_P6x8Str(0,5," target=");
      				LCD_P6x8Str(0,6," PWM=");
      				LCD_P6x8Str(0,7," speed=");
      				break;
            }
            case 2:
            {
              LCD_P6x8Str(0,2," Kp=");
      				LCD_P6x8Str(0,3,"*Ki=");
      				LCD_P6x8Str(0,4," Kd=");
      				LCD_P6x8Str(0,5," target=");
      				LCD_P6x8Str(0,6," PWM=");
      				LCD_P6x8Str(0,7," speed=");
      				break;
            }
            case 3:
            {
              LCD_P6x8Str(0,2," Kp=");
      				LCD_P6x8Str(0,3," Ki=");
      				LCD_P6x8Str(0,4,"*Kd=");
      				LCD_P6x8Str(0,5," target=");
      				LCD_P6x8Str(0,6," PWM=");
      				LCD_P6x8Str(0,7," speed=");
      				break;
            }
            case 4:
            {
              LCD_P6x8Str(0,2," Kp=");
      				LCD_P6x8Str(0,3," Ki=");
      				LCD_P6x8Str(0,4," Kd=");
      				LCD_P6x8Str(0,5,"*target=");
      				LCD_P6x8Str(0,6," PWM=");
      				LCD_P6x8Str(0,7," speed=");
      				break;
            }
          }
          status_knob=0;
          break;
        }
        case 4:
        {
          temp_status--;
          if(temp_status<1)
            temp_status=4;
          
          switch(temp_status)
          {
            case 1:
            {
              LCD_P6x8Str(0,2,"*Kp=");
      				LCD_P6x8Str(0,3," Ki=");
      				LCD_P6x8Str(0,4," Kd=");
      				LCD_P6x8Str(0,5," target=");
      				LCD_P6x8Str(0,6," PWM=");
      				LCD_P6x8Str(0,7," speed=");
      				break;
            }
            case 2:
            {
              LCD_P6x8Str(0,2," Kp=");
      				LCD_P6x8Str(0,3,"*Ki=");
      				LCD_P6x8Str(0,4," Kd=");
      				LCD_P6x8Str(0,5," target=");
      				LCD_P6x8Str(0,6," PWM=");
      				LCD_P6x8Str(0,7," speed=");
      				break;
            }
            case 3:
            {
              LCD_P6x8Str(0,2," Kp=");
      				LCD_P6x8Str(0,3," Ki=");
      				LCD_P6x8Str(0,4,"*Kd=");
      				LCD_P6x8Str(0,5," target=");
      				LCD_P6x8Str(0,6," PWM=");
      				LCD_P6x8Str(0,7," speed=");
      				break;
            }
            case 4:
            {
              LCD_P6x8Str(0,2," Kp=");
      				LCD_P6x8Str(0,3," Ki=");
      				LCD_P6x8Str(0,4," Kd=");
      				LCD_P6x8Str(0,5,"*target=");
      				LCD_P6x8Str(0,6," PWM=");
      				LCD_P6x8Str(0,7," speed=");
      				break;
            }
          }
          status_knob=0;
          break;          
        }
      }
      
      sprintf(mystr,"%3d",Kp);
      LCD_P6x8Str(24,2,mystr);
      sprintf(mystr,"%3d",Ki);
      LCD_P6x8Str(24,3,mystr);
      sprintf(mystr,"%3d",Kd);
      LCD_P6x8Str(24,4,mystr);
      sprintf(mystr,"%4d",target);
      LCD_P6x8Str(48,5,mystr);
      sprintf(mystr,"%4d",PWMDTY45);
      LCD_P6x8Str(30,6,mystr);
      sprintf(mystr,"%4d",SPEED);
      LCD_P6x8Str(42,7,mystr);
      
      status_button=check_button();
      if(status_button==1)
      {
        LCD_P6x8Str(0,2,"*Kp=");
				LCD_P6x8Str(0,3," Ki=");
				LCD_P6x8Str(0,4," Kd=");
				LCD_P6x8Str(0,5," target=");
				LCD_P6x8Str(0,6," PWM=");
				LCD_P6x8Str(0,7," speed=");        
      }
      else if(status_button==2)//������ť1�޷�Ӧ��������ť2�ص���������ֵ��ʾ�׶�
      {
        target=0;
        status=0;
        LCD_P6x8Str(0,0,"Left=");
        LCD_P6x8Str(0,1,"Center=");
        LCD_P6x8Str(0,2,"Right=");
        status_knob=0;
        TFLG1=0xFF;
        TIE=0x00;
      }
    }
    
    else if(status==3)  //�趨�����ٶ�
    {
      switch(status_knob)
      {
        case 1:
        {
          maxspeed += 50;
          status_knob=0;
          break;
        }
        case 2:
        {
          maxspeed -= 50;
          status_knob=0;
          break;
        }
        case 3:
        {
          minspeed += 50;
          status_knob=0;
          break;
        }
        case 4:
        {
          minspeed -= 50;
          status_knob=0;
          break;
        }
      }
      
      sprintf(mystr,"%4d",maxspeed);
      LCD_P6x8Str(24,6,mystr);
      sprintf(mystr,"%4d",minspeed);
      LCD_P6x8Str(24,7,mystr);
      
      status_button=check_button();
      if(status_button==1)//���°�ť1�ص���������ֵ��ʾ�׶�
      {
        status=0;
        LCD_P6x8Str(0,0,"Left=");
        LCD_P6x8Str(0,1,"Center=");
        LCD_P6x8Str(0,2,"Right=");
        TFLG1=0xFF;
        TIE=0x00;
      }
      else if(status_button==2)//���°�ť2׼������
      {
        LCD_P6x8Str(0,0,"GO GO GO!!!");
        wait(120);
        LCD_CLS();
        status++;
      }
    }
    
    else if(status==4)  //С�����������������������ǰ���ĳ�ʼ�������������
    {
      TFLG1=0xFF;
      TIE=0x00;
      /**************************************************************
      ������ܻ���Ҫ����һЩ�����ĳ�ʼ�������⻹����ӵ�������Ĵ���
      **************************************************************/
      
      break;//�������Խ׶���ѭ��������С�����н׶�
    }
  }

}