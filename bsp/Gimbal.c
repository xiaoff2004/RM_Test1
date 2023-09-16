#include "Gimbal.h"
#include "main.h"
#include "motor.h"
#include "imu.h"
#include "pid.h"
#include "remote.h"


void Gimbal_Init(void)
{
	Motor_Info[Yaw].TargetAngle=YAW_InitalAngle;
	Motor_Info[Pitch].TargetAngle=Pitch_InitalAngle;
}


void Gimbal_UpData(void)
{
	//不知道用的是哪一个角
	Motor_Info[Yaw].TargetAngle=ImuData.angle[0]*8192-1;
	Motor_Info[Yaw].TargetAngle+=RemoteData.TargetSpeedYaw;

	Motor_Info[Pitch].TargetAngle+=RemoteData.TargetSpeedPitch;

	Posi_IncrPID(&PosiPID_Info[Yaw], &IncrPID_Info[Yaw], &Motor_Info[Yaw]);
	Posi_IncrPID(&PosiPID_Info[Pitch], &IncrPID_Info[Pitch], &Motor_Info[Pitch]);
}
