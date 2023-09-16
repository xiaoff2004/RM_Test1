#ifndef __REMOTE_H__
#define __REMOTE_H__

#include "main.h"

extern uint8_t RecieveDate[18];

typedef struct
{
	int16_t ch0;		//遥控器通道
	int16_t ch1;
	int16_t ch2;
	int16_t ch3;
	uint8_t s1;			//开关位（1~3）
	uint8_t s2;
	int16_t wheel;		//左上方拨轮

	int16_t X;
	int16_t Y;
	int16_t Z;
	uint8_t L;
	uint8_t R;

	uint8_t W;
	uint8_t S;
	uint8_t A;
	uint8_t D;
	uint8_t Q;
	uint8_t E;
	uint8_t Shirt;
	uint8_t Ctrl;
}Remote_t;

typedef enum
{
	ChassisNoSpin=1,
	ChassisFollow,
	ChassisSpin
}ChassisStatus_t;

typedef enum
{
	FireNone=1,
	FreeFire,
	AutoFire
}FireType_t;

typedef enum
{
	Reverse,//反转（倒弹）
	SingleShoot,
	MultiShoot
}ShootStrategy_t;

typedef enum
{
	Remote,
	Board
}RemoteOrBoard_t;

typedef enum
{
	Mecanum,
	Omni_4
}ChassisType_t;

typedef struct
{
	int16_t TargetSpeedX;
	int16_t TargetSpeedY;
	int16_t TargetSpeedW;
	int16_t TargetSpeedYaw;
	int16_t TargetSpeedPitch;
	int16_t TargetAngle;

//	int16_t SpeedX;
//	int16_t SpeedY;
//	int16_t SpeedW;
//	int16_t SpeedYaw;
//	int16_t SpeedPitch;
//	int16_t Angle;

	ChassisStatus_t ChassisStatus;
	FireType_t FireType;
	ShootStrategy_t ShootStrategy;

	RemoteOrBoard_t RemoteOrBoard;

	ChassisType_t ChassisType;


}RemoteData_t;




//通道值映射到底盘或Yaw轴、Pitch轴速度的比值
//遥控器映射值
#define ChassisSpeed_RemoteChange 0.6f
#define YawSpeed_RemoteChange		0.6f
#define PitchSpeed_RemoteChange		0.6f
//键盘映射值
#define ChassisSpeed_BoardChange 0.6f
#define YawSpeed_BoardChange		0.6f
#define PitchSpeed_BoardChange		0.6f


extern RemoteData_t RemoteData;
void RemoteInit(void);
void RemoteData_Unpack(void);
#endif
