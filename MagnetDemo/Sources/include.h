#ifndef INCLUDE_H
#define INCLUDE_H

#include <hidef.h>      
#include "derivative.h"
#include <stdio.h>


#include "init.h"
#include "OLED.h"

#include "debug.h"

#include "Delay.h"
#include "Speed_PID.h"

//���к궨��
#define MID 1180
#define LEFT_DIE 1050
#define RIGHT_DIE 1310

#define KP1 300 //���ڵ���ռ�ձ� 
#define KD1 50 //���ڵ���ռ�ձ� 

#define KP2 230 //���ڵ���ռ�ձ� 
#define KD2 20 //���ڵ���ռ�ձ� 


#define STEER PWMDTY67
#define IO1 PWMDTY45  //��ת
#define IO2 PWMDTY23  //��ת

#define PID_Dead 5
#define Speed_Max 5000
#define SpeedMax 150


#endif

