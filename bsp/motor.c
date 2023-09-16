#include "motor.h"
#include "main.h"
#include "can.h"

sMotor_Info Motor_Info[10];
CAN_TxHeaderTypeDef Motor_TXHeader[3];
//CAN_RxHeaderTypeDef Motor_RXHeader[3];



void CanConfig_Init(void)
{
	CAN_FilterTypeDef can_filter;
	can_filter.FilterBank=0;
	can_filter.FilterActivation=ENABLE;
	can_filter.FilterMode=CAN_FILTERMODE_IDMASK;
	can_filter.FilterScale=CAN_FILTERSCALE_32BIT;
	can_filter.FilterIdHigh=0x0000;
	can_filter.FilterIdLow=0x0000;
	can_filter.FilterMaskIdHigh=0x0000;
	can_filter.FilterMaskIdLow=0x0000;
	can_filter.FilterFIFOAssignment=CAN_RX_FIFO0;
	HAL_CAN_ConfigFilter(&hcan1, &can_filter);
	HAL_CAN_ConfigFilter(&hcan2, &can_filter);
}

void MotorInfo_Init(void)
{
	for(uint8_t i=0; i<3; i++)
	{
		Motor_TXHeader[i].ExtId = 0x00;
		Motor_TXHeader[i].IDE = CAN_ID_STD;
		Motor_TXHeader[i].RTR = CAN_RTR_DATA;
		Motor_TXHeader[i].DLC = 8;
//		Motor_RXHeader[i].ExtId = 0x00;
//		Motor_RXHeader[i].IDE = CAN_ID_STD;
//		Motor_RXHeader[i].RTR = CAN_RTR_DATA;
//		Motor_RXHeader[i].DLC = 8;
	}
	Motor_TXHeader[Chassis].StdId=Chassiss_TXID;
	Motor_TXHeader[Gimbal].StdId=Gimbal_TXID;
	Motor_TXHeader[Frict].StdId=Frict_TXID;
}

void MotorInit(void)
{
	CanConfig_Init();
	MotorInfo_Init();
	HAL_CAN_Start(&hcan1);
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
	HAL_CAN_Start(&hcan2);
	HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO1_MSG_PENDING);
}

//-16384 ~ 16384
void Set_ChasissConf(int16_t Chasiss1,int16_t Chasiss2,int16_t Chasiss3,int16_t Chasiss4)
{
	uint8_t Data[8]={0};
	Data[0]=Chasiss1>>8;
	Data[1]=Chasiss1;
	Data[2]=Chasiss2>>8;
	Data[3]=Chasiss2;
	Data[4]=Chasiss3>>8;
	Data[5]=Chasiss3;
	Data[6]=Chasiss4>>8;
	Data[7]=Chasiss4;
	HAL_CAN_AddTxMessage(&hcan1, &Motor_TXHeader[Chassis], Data, (uint32_t *)CAN_TX_MAILBOX0);
}

//-30000 ~ 30000
void Set_GimbalConf(int16_t Yaw,int16_t Pitch)
{
	uint8_t Data[8]={0};
	Data[0]=Yaw>>8;
	Data[1]=Yaw;
	Data[2]=Pitch>>8;
	Data[3]=Pitch;
	HAL_CAN_AddTxMessage(&hcan2, &Motor_TXHeader[Gimbal], Data, (uint32_t *)CAN_TX_MAILBOX0);
}

void Set_AmmoSpeed(int16_t Frict_L,int16_t Frict_R,int16_t Ammo)
{
	uint8_t Data[8]={0};
	Data[0]=Frict_L>>8;
	Data[1]=Frict_L;
	Data[2]=Frict_R>>8;
	Data[3]=Frict_R;
	Data[4]=Ammo>>8;
	Data[5]=Ammo;
	HAL_CAN_AddTxMessage(&hcan2, &Motor_TXHeader[Frict], Data, (uint32_t *)CAN_TX_MAILBOX0);
}


void Motor_RecieveData(sMotor_Info *motor_info,uint8_t *Data)
{
	motor_info->Angle = (Data[0]<<8)|Data[1];
	motor_info->Speed = (Data[2]<<8)|Data[3];
	motor_info->Current = (Data[4]<<8)|Data[5];
	motor_info->temperature =Data[6];
}



