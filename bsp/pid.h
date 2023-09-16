#ifndef __PID_H__
#define __PID_H__


#include "main.h"
#include "motor.h"


typedef struct
{

	float Kp,Ki,Kd;
	float Err,LastErr;
	float Sum,MaxSum;
	float Output,MaxOutput;
}sPosiPID_Info;

typedef struct
{
	float Kp,Ki,Kd;
	float Err,LastErr,LastLastErr;
	float Output,MaxOutput;
}sIncrPID_Info;

//typedef enum
//{
//	Speed=0,
//	Angle
//}PIDType;

extern sPosiPID_Info PosiPID_Info[10];
extern sIncrPID_Info IncrPID_Info[10];

void PID_Init(void);
void PosiPID(sPosiPID_Info *PosiPID,sMotor_Info *MotorInfo);
void IncrPID(sIncrPID_Info *IncrPID,sMotor_Info *MotorInfo);
void Posi_IncrPID(sPosiPID_Info *PosiPID,sIncrPID_Info *IncrPID,sMotor_Info *MotorInfo);

#endif
