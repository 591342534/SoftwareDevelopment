#include "include.h"

 


void SetBusCLK_80M(void)//����Ƭ����Ƶ��Ϊ80MHz
{   
  CLKSEL = 0x00;				//disengage PLL to system
  PLLCTL_PLLON = 1;
  SYNR = 0b11001001;    //VCOFRQ=11, SYNDIV=0b001001=9                    
  REFDV = 0b10000001;   //REFFRQ=10, REFDIV=0b000001=1
                        //���İ��Ͼ���fOSC=16M 
                        //fVCO=2*fOSC*(SYNDIV+1)/(REFDIV+1)=2*16M*10/2=160M
  POSTDIV = 0x00;       //POSTDIV=0, fPLL=fVCO=160M
                        //fBUS=fPLL/2=80M
                        //fREF=fOSC/(REFDIV+1)=8M  
  _asm(nop);
  _asm(nop);
  while(!(CRGFLG_LOCK == 1));	  //when pll is steady ,then use it;
  CLKSEL_PLLSEL = 1;		        //engage PLL to system;
}

void initRTI(void)    //Real Time Interrupt��ʵʱ�жϣ�ÿ10ms�ж�һ��
{
  RTICTL= 0b11000111;    //ʮ���ƣ�16M/(16*10*10^3)=100;
  CRGINT|=0b10000000;   //ʹ��RTI    
}


void initPIT(void)      //�����жϳ�ʼ��
{   
   PITCFLMT_PITE=0;     //disable PIT
   
   PITCE_PCE0=1;        //enable timer channel 0
   PITMUX_PMUX0=0;      //ch0 connected to micro timer 0
   //����΢��ʱ���Ͷ�ʱ������
   PITMTLD0=250-1;      
   PITLD0=3200-1;      //250 * 3200 / 80000000 = 10ms. --> ��Ϊʵ��ֵ���д����ڵ���
   
   PITINTE_PINTE0=1;    //enable interupt channel 0
   
   PITCFLMT_PITE=1;     //enable PIT
}

void initPWM(void)     //PWM��ʼ��
{
  PWME = 0x00;         //��ֹPWM
  PWMPRCLK = 0x00;     //clockA 1��Ƶ:80MHz   clockB 1��Ƶ:80MHz
  
  //*****ת���� 6��7����********
  PWMCTL_CON67 = 1;    //67����
  PWMSCLB = 40;        //��clock SB ����2*PWMSCLB=80��Ƶ��pwm clock=clockB/80=1MHz;  
  PWMPOL_PPOL7 = 1;    //�ź��Ըߵ�ƽ��ʼ
  PWMCLK_PCLK7 = 1;    //ʱ��ΪSB
  PWMCAE_CAE7 = 0;     //�����
  PWMPER67 = 20000;    //��������ֵΪ20000��PWMƵ�� = SB/20000 = 50Hz
  PWMDTY67 = MID;     //ռ�ձ�Ϊת������ֵλ��
  PWME_PWME7 = 1;      //ʹ��PWM67
  //******************************
  
  //*****�������1 4��5����*******
  PWMCTL_CON45=1;      //45����
  PWMPOL_PPOL5 = 1;    //�ź��Ըߵ�ƽ��ʼ
  PWMCLK_PCLK5 = 0;    //ʱ��ΪA
  PWMCAE_CAE5 = 0;     //�����
  PWMPER45 = 5000;     //PWMƵ�� = clockA/5000 = 16kHz
  PWMDTY45 = 0;        //ռ�ձ�0�����������
  PWME_PWME5 = 1;      //ʹ��PWM45
  //******************************
  
  //*****�������2 2��3����*******
  PWMCTL_CON23=1;      //23����
  PWMPOL_PPOL3 = 1;    //�ź��Ըߵ�ƽ��ʼ
  PWMCLK_PCLK3 = 0;    //ʱ��ΪB
  PWMCAE_CAE3 = 0;     //�����
  PWMPER23 = 5000;     //PWMƵ�� = clockB/5000 = 16kHz
  PWMDTY23 = 0;        //ռ�ձ�0�����������
  PWME_PWME3 = 1;      //ʹ��PWM23
  //******************************
}

void initPORT(void)     //IO�ڳ�ʼ��
{
  DDRB=0xFF;            //B����Ϊ���������Ļ
  DDRS=0x00;            //S����Ϊ���룬����ť1
  PORTB=0xFF;
  DDRE=0x00;            //E����Ϊ���룬����ť2��3
}

void initTIM(void)     //���벶׽�жϳ�ʼ���������ۼ�����ʼ��
{
 
  TIOS =0x00;          //��ʱ��ͨ��Ϊ���벶׽
  TSCR1=0x80;          //��ʱ��ʹ��
  TCTL4=0b00010101;    //T0��׽��ť3������, T1��׽��ť1������, T2��׽��ť2������
  TFLG1=0xFF;          //���жϱ�־λ
  TIE=0x00;            
}

void initPAC(void)
{
  PACTL=0x40;          //ʹ��PT7�����ۼ���
  PACNT=0;        //������������
}


void initATD(void)
{
  ATD0CTL0=0x02;       //ת����AD2��ص�AD0��������AD0��AD1��AD2��AD0��AD1......˳��ת��
  ATD0CTL1=0x30;       //�����ⲿ����ADת����ת������10λ������ǰ�ŵ磨�ŵ��õ�ת��ֵ����ȷ������Ҫ�����ʱ�����ڣ������費��Ҫ�ŵ����λ���е��ԣ�
  ATD0CTL2=0x40;       //�������� �޵ȴ�ģʽ, �жϽ�ֹ
  ATD0CTL3=0x98;       //ATDת������Ҷ��룬ת�����г�����3����ת��AD0��AD1��AD2����������FIFOģʽ
  ATD0CTL4=0x01;       //4����,ATDCLK=fBUS/(2*(PRS+1))=80M/(2*��1+1))=20M
  ATD0CTL5=0x30;       //������ͨ��ת��
}

void Init_All(void)

{
  SetBusCLK_80M();
  initPORT();
  initLCD();
//  initRTI();  
  initPIT();
  initPWM();
//  initTIM();
  initPAC();
  initATD();
}