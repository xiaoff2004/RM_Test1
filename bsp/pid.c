/*
 * @file		pid.c/h
 * @brief		pid相关程序
 * @brief		GM6020   	MaxOutput: 	30000
 * 							MaxSpeed:	320rpm(No load)
 * 										132rpm(Loading)
 *
 *				M3508(19:1)		MaxOutput: 	16384
 *							MaxSpeed:	482rpm(No load)
 * 										469rpm(Loading)
 *
 *
 * @history
 *
 *
 *
 */


#include "pid.h"
#include "motor.h"
#include "main.h"
#include "math.h"

sPosiPID_Info PosiPID_Info[10];
sIncrPID_Info IncrPID_Info[10];

//Chassis1=0,Chassis2,Chassis3,Chassis4,frict_L,frict_R,Ammo,Chassis_Posi,Yaw,Pitch

float PidInfo[2][10][5]={	//PosiPID{Kp,Ki,Kd,MaxSum,MaxOutput}
		{	{0},			//chasiss
			{0},
			{0},
			{0},
			{0},
			{0},
			{0},
			{2.0f,0,80.0f,5000,28000},	//yaw
			{0},
			{0}},
							//IncrPID{Kp,Ki,Kd,MaxOutput,0}
		{	{0,0.01f,0,5000,0},			//chasiss
			{0},
			{0},
			{0},
			{0},
			{0},
			{0},
			{0},
			{0.15f,1.5f,0,28000,0},	//yaw
			{0}}
};

float PIDInfo_Limit(float Value,float MaxValue)
{
	if(fabs(Value) > MaxValue)
	{
		if(Value > 0)
			Value = MaxValue;
		else
			Value = -MaxValue;
	}

	return Value;
}

void PID_Init(void)
{
	for(uint8_t i=0;i<8;i++)
	{
		PosiPID_Info[i].Kp		=PidInfo[0][i][0];
		PosiPID_Info[i].Ki		=PidInfo[0][i][1];
		PosiPID_Info[i].Kd		=PidInfo[0][i][2];
		PosiPID_Info[i].MaxSum	=PidInfo[0][i][3];
		PosiPID_Info[i].MaxOutput=PidInfo[0][i][4];
		PosiPID_Info[i].Err		=0;
		PosiPID_Info[i].Sum  	=0;
		PosiPID_Info[i].Output	=0;

		IncrPID_Info[i].Kp		=PidInfo[1][i][0];
		IncrPID_Info[i].Ki		=PidInfo[1][i][1];
		IncrPID_Info[i].Kd		=PidInfo[1][i][2];
		IncrPID_Info[i].MaxOutput=PidInfo[1][i][3];
		IncrPID_Info[i].Err		=0;
		IncrPID_Info[i].LastErr	=0;
		IncrPID_Info[i].Output	=0;
	}
}

void PosiPID(sPosiPID_Info *PosiPID,sMotor_Info *MotorInfo)
{
	PosiPID->LastErr=PosiPID->Err;
	PosiPID->Err=MotorInfo->TargetAngle -MotorInfo->Angle;
	if(PosiPID->Err > 4096)
		PosiPID->Err -= 8192;
	else if(PosiPID->Err < 4096)
		PosiPID->Err += 8192;
	PosiPID->Sum +=PosiPID->Err;
	PosiPID->Sum =PIDInfo_Limit(PosiPID->Sum, PosiPID->MaxSum);
	PosiPID->Output =PosiPID->Kp *	PosiPID->Err
					+PosiPID->Ki *	PosiPID->Sum
					+PosiPID->Kd *	(PosiPID->Err -PosiPID->LastErr);
	PosiPID->Output =PIDInfo_Limit(PosiPID->Output, PosiPID->MaxOutput);
}

void IncrPID(sIncrPID_Info *IncrPID,sMotor_Info *MotorInfo)
{
	IncrPID->LastLastErr =IncrPID->LastErr ;
	IncrPID->LastErr =IncrPID ->Err;
	IncrPID->Err =MotorInfo->TargetSpeed -MotorInfo ->Speed;
	IncrPID->Output +=IncrPID->Kp *IncrPID->Err -IncrPID ->LastErr
					 +IncrPID->Ki *IncrPID->Err
					 +IncrPID->Kd *(IncrPID->Err -2 * IncrPID->LastErr +IncrPID->LastLastErr);
	IncrPID->Output =PIDInfo_Limit(IncrPID->Output, IncrPID->MaxOutput);
}

void Posi_IncrPID(sPosiPID_Info *PosiPID,sIncrPID_Info *IncrPID,sMotor_Info *MotorInfo)
{
	PosiPID->LastErr=PosiPID->Err;
	PosiPID->Err=MotorInfo->TargetAngle -MotorInfo->Angle;
	if(PosiPID->Err > 4096)
		PosiPID->Err -= 8192;
	else if(PosiPID->Err < -4096)
		PosiPID->Err += 8192;
	PosiPID->Sum +=PosiPID->Err;
	PosiPID->Sum =PIDInfo_Limit(PosiPID->Sum, PosiPID->MaxSum);
	PosiPID->Output =PosiPID->Kp *	PosiPID->Err
					+PosiPID->Ki *	PosiPID->Sum
					+PosiPID->Kd *	(PosiPID->Err -PosiPID->LastErr);
	PosiPID->Output =PIDInfo_Limit(PosiPID->Output, PosiPID->MaxOutput);

	IncrPID->LastLastErr =IncrPID->LastErr ;
	IncrPID->LastErr =IncrPID ->Err;
	IncrPID->Err =PosiPID->Output -MotorInfo ->Speed;
	IncrPID->Output +=IncrPID->Kp *IncrPID->Err -IncrPID ->LastErr
					 +IncrPID->Ki *IncrPID->Err
					 +IncrPID->Kd *(IncrPID->Err -2.0f * IncrPID->LastErr +IncrPID->LastLastErr);
	IncrPID->Output =PIDInfo_Limit(IncrPID->Output, IncrPID->MaxOutput);
}

void PID_Clear(void)
{
	for(uint8_t i =0;i<10;i++)
	{
		IncrPID_Info[i].Kp		=0;
		IncrPID_Info[i].Ki		=0;
		IncrPID_Info[i].Kd		=0;
		IncrPID_Info[i].MaxOutput=0;
		IncrPID_Info[i].Err		=0;
		IncrPID_Info[i].LastErr	=0;
		IncrPID_Info[i].Output	=0;
	}
}
