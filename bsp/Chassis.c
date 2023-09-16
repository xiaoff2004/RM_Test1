#include "Chassis.h"
#include "main.h"
#include "motor.h"
#include "imu.h"
#include "pid.h"
#include "remote.h"
#include "math.h"
#include "Gimbal.h"

void Chassis_Init(void)
{
	RemoteData.ChassisType = Mecanum;
}


void ChassisAngle_Solve(void)
{
	//经过处理后可认为枪口位置编码器返回值为0
	Motor_Info[Chassis_Posi].Angle = Motor_Info[Yaw].Angle-YAW_InitalAngle +8192;
	float wheel_speed_sum = -((float)Motor_Info[Chassis1].Speed + (float)Motor_Info[Chassis2].Speed + (float)Motor_Info[Chassis3].Speed + (float)Motor_Info[Chassis4].Speed);
	Motor_Info[Chassis_Posi].Speed = wheel_speed_sum / (2*(MECANUM_WHEEL_BASE + MECANUM_WHEEL_TARCK)*60
						/(2*MECANUM_WHEEL_RADIUS * PI)*3591/187);
//	RemoteData.SpeedX = (-Motor_Info[Chassis1].Speed + Motor_Info[Chassis2].Speed + Motor_Info[Chassis3].Speed + Motor_Info[Chassis4].Speed)/4;
//	RemoteData.SpeedY = (Motor_Info[Chassis1].Speed + Motor_Info[Chassis2].Speed - Motor_Info[Chassis3].Speed - Motor_Info[Chassis4].Speed)/4;
}

void Chassis_UpData(void)
{
	//判断底盘的状态（不转、转、底盘跟随）
	switch((uint8_t)RemoteData.ChassisStatus)
	{
	case ChassisNoSpin:
		RemoteData.TargetSpeedW=0;
	case ChassisSpin:
		RemoteData.TargetSpeedW=5.0f;

	case ChassisFollow:
		{
			Motor_Info[Chassis_Posi].TargetAngle=YAW_InitalAngle;
			Posi_IncrPID(&PosiPID_Info[Chassis_Posi], &IncrPID_Info[Chassis_Posi], &Motor_Info[Chassis_Posi]);
			RemoteData.TargetSpeedW=PosiPID_Info[Chassis_Posi].Output;
		}
	}

	//将世界坐标系下的速度转换成底盘坐标系下的速度
	ChassisAngle_Solve();
	float angle =(float)Motor_Info[Chassis_Posi].Angle/8192.0f * 2.0f *PI;
	RemoteData.TargetSpeedX=RemoteData.TargetSpeedX*cosf(angle) +RemoteData.TargetSpeedY*sinf(angle);
	RemoteData.TargetSpeedY=RemoteData.TargetSpeedY*cosf(angle) -RemoteData.TargetSpeedX*sinf(angle);


	//麦轮底盘解算
	if(RemoteData.ChassisType == Mecanum)
	{
		Motor_Info[Chassis1].TargetSpeed = -(RemoteData.TargetSpeedX - RemoteData.TargetSpeedY + RemoteData.TargetSpeedW *0.5*(MECANUM_WHEEL_BASE + MECANUM_WHEEL_TARCK))*60/(2*MECANUM_WHEEL_RADIUS *PI)*3591/187;
		Motor_Info[Chassis2].TargetSpeed = (RemoteData.TargetSpeedX + RemoteData.TargetSpeedY - RemoteData.TargetSpeedW *0.5*(MECANUM_WHEEL_BASE + MECANUM_WHEEL_TARCK))*60/(2*MECANUM_WHEEL_RADIUS *PI)*3591/187;
		Motor_Info[Chassis3].TargetSpeed = (RemoteData.TargetSpeedX - RemoteData.TargetSpeedY - RemoteData.TargetSpeedW *0.5*(MECANUM_WHEEL_BASE + MECANUM_WHEEL_TARCK))*60/(2*MECANUM_WHEEL_RADIUS *PI)*3591/187;
		Motor_Info[Chassis4].TargetSpeed = -(RemoteData.TargetSpeedX + RemoteData.TargetSpeedY + RemoteData.TargetSpeedW *0.5*(MECANUM_WHEEL_BASE + MECANUM_WHEEL_TARCK))*60/(2*MECANUM_WHEEL_RADIUS *PI)*3591/187;
	}

	else if(RemoteData.ChassisType  == Omni_4)
	{
		Motor_Info[Chassis1].TargetSpeed = -(RemoteData.TargetSpeedX - RemoteData.TargetSpeedY + RemoteData.TargetSpeedW *0.5*(MECANUM_WHEEL_BASE + MECANUM_WHEEL_TARCK))*60/(2*MECANUM_WHEEL_RADIUS *PI)*3591/187;
		Motor_Info[Chassis2].TargetSpeed = (RemoteData.TargetSpeedX + RemoteData.TargetSpeedY - RemoteData.TargetSpeedW *0.5*(MECANUM_WHEEL_BASE + MECANUM_WHEEL_TARCK))*60/(2*MECANUM_WHEEL_RADIUS *PI)*3591/187;
		Motor_Info[Chassis3].TargetSpeed = (RemoteData.TargetSpeedX - RemoteData.TargetSpeedY - RemoteData.TargetSpeedW *0.5*(MECANUM_WHEEL_BASE + MECANUM_WHEEL_TARCK))*60/(2*MECANUM_WHEEL_RADIUS *PI)*3591/187;
		Motor_Info[Chassis4].TargetSpeed = -(RemoteData.TargetSpeedX + RemoteData.TargetSpeedY + RemoteData.TargetSpeedW *0.5*(MECANUM_WHEEL_BASE + MECANUM_WHEEL_TARCK))*60/(2*MECANUM_WHEEL_RADIUS *PI)*3591/187;
	}

	for(uint8_t i =Chassis1;i<=Chassis4;i++)
		{
			IncrPID(&IncrPID_Info[i], &Motor_Info[i]);
		}
}

