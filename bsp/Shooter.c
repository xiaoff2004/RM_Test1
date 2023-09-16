#include "Shooter.h"
#include "main.h"
#include "motor.h"
#include "imu.h"
#include "pid.h"
#include "remote.h"

void Shooter_Init(void)
{
	Motor_Info[frict_L].TargetSpeed = ShooterSpeed;
	Motor_Info[frict_R].TargetSpeed = -ShooterSpeed;
	Motor_Info[Ammo].TargetAngle =	Ammo_InitalAngle;
}

void Shooter_UpData(void)
{
	if(RemoteData.ShootStrategy== MultiShoot)
	{
		Motor_Info[frict_L].TargetSpeed = 	ShooterSpeed;
		Motor_Info[frict_R].TargetSpeed = 	-ShooterSpeed;
		Motor_Info[Ammo].TargetAngle +=8192/8;
	}
	else if(RemoteData.ShootStrategy== Reverse)
	{
		Motor_Info[frict_L].TargetSpeed =	-ReverseSpeed;
		Motor_Info[frict_R].TargetSpeed =	 ReverseSpeed;
		Motor_Info[Ammo].TargetAngle -=8192/8;
	}

	if(Motor_Info[Ammo].TargetAngle>8192)
		Motor_Info[Ammo].TargetAngle-=8192;
	else if(Motor_Info[Ammo].TargetAngle<0)
		Motor_Info[Ammo].TargetAngle+=8192;

	for(uint8_t i=frict_L;i<=frict_R;i++)
	{
		IncrPID(&IncrPID_Info[i], &Motor_Info[i]);
	}

	Posi_IncrPID(&PosiPID_Info[Ammo], &IncrPID_Info[Ammo], &Motor_Info[Ammo]);
}

