#include "init.h"

//��Ƶ��24MHz
void MCU_Init(void)
{
  MCGC2 = 0x36;
  while(!(MCGSC & 0x02));        // wait for the OSC stable
  MCGC1 = 0x98;
  while((MCGSC & 0x1C ) != 0x08); // external clock is selected
  MCGC3 = 0x48;
  while ((MCGSC & 0x48) != 0x48); // wait for the PLL is locked
  MCGC1 = 0x18;
  while((MCGSC & 0x6C) != 0x6C);
}


void LED_Init(void)
{
  PTBDD |= 0x03; //PB0,PB1 output
  PTBD_PTBD0 = 1; //led off
  PTBD_PTBD1 = 1;
}


void KBI_Init(void)
{
  KBISC = 0x0E; //only edges
  KBIPE = 0x30; //enable key4 ,key5
}


void RTC_Init(void)
{
   MCGC2 |= 0x26; //ʹ���ⲿ����
   RTCSC = 0x38; //�ⲿ4MHz����, 16��Ƶ
   RTCMOD = 40; // (1/4000)*40 = 1/100 ��ʱ10ms
}

void PTED_Init(void)
{
  PTEDD |= 0xE0;
  PTED |= 0xE0;
}

void SPI_Init(void)
{
   PTED_Init();
   SPI1C1 = 0x5D; //ǰ��������ز�������λ��Ч��ʼ
   SPI1BR = 0x11; //8��Ƶ, baud = BusClock/(2*4) = 1MHz
}

void ATD_Init(void)
{
  APCTL1 |= 0x04; //disable PB2 IO and set to AD 
  //ADCCFG = 0x04; //busclock, 12bit
  ADCCFG = 0x00; //busclock, 8bit
  ADCSC1_AIEN = 1; //enable interrup
  ADCSC1_ADCO = 0; //one conversion
  ADCSC1_ADCH = 2; //select AD ch2
  ADCSC2_ADTRG = 1; //hardware 
}

void SCI_Init(void)
{
   SCI1BD = BusClock/16/9600;
   SCI1C2 = 0x2C; 
}


void ALL_Init(void)
{
  SOPT1 &= 0x3F; //disable cop
  LED_Init();
  KBI_Init();
  RTC_Init();
  SPI_Init();
  ATD_Init();
  SCI_Init();
}