#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "main.h"

typedef enum
{
	Chassis1=0,
	Chassis2,
	Chassis3,
	Chassis4,

	frict_L,  	//摩擦轮
	frict_R,
	Ammo,		//拨弹轮

	Chassis_Posi,

	Yaw,		//为了方便解算，将yaw电机设为8，前面空的一个编号为底盘角度环控制服务
	Pitch
}sMotor_No;

typedef enum
{
	Chassis,
	Gimbal,
	Frict
}sMotorTXHeader_No;

//发送报文标识符
#define Chassiss_TXID 	0x200			//chassis1,chassis2,chsaais3,chassis4
#define Gimbal_TXID		0x2FF			//Yaw,Pitch
#define Frict_TXID		0x1FF		//Frict_L,Frict_R,Ammo


////反馈接收报文标识符
////can1
//#define Chassis1_RXID 	0x201
//#define Chassis2_RXID 	0x202
//#define Chassis3_RXID 	0x203
//#define Chassis4_RXID 	0x204
//
//
//
////can2
//#define Frict_L_RXID  	0x205			//摩擦轮
//#define Frict_R_RXID  	0x206
//#define Ammo_RXID		0x207
//#define Yaw_RXID		0x209
//#define Pitch_RXID		0x20A

typedef struct
{
	uint16_t TargetAngle;
	uint16_t Angle;
	int16_t TargetSpeed;
	int16_t Speed;
	int16_t Current;
	uint8_t temperature;
}sMotor_Info;

extern sMotor_Info Motor_Info[10];
extern CAN_TxHeaderTypeDef Motor_TXHeader[3];
//extern CAN_RxHeaderTypeDef Motor_RXHeader[3];


void MotorInit(void);
void Set_ChasissConf(int16_t Chasiss1,int16_t Chasiss,int16_t Chasiss3,int16_t Chasiss4);
void Set_GimbalConf(int16_t Yaw,int16_t Pitch);
void Set_AmmoSpeed(int16_t Frict_L,int16_t Frict_R,int16_t Ammo);
void Motor_RecieveData(sMotor_Info *Motor_Info,uint8_t *Data);


#endif
