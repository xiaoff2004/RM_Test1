#include "imu.h"
#include "main.h"
#include "spi.h"
#include "gpio.h"
#include "math.h"
#include "MahonyAHRS.h"

ImuData_t ImuData;


void Gyro_WriteByte(uint8_t addr,uint8_t data)
{
	HAL_GPIO_WritePin(CS1_Gyro_GPIO_Port, CS1_Gyro_Pin, GPIO_PIN_RESET);
	uint8_t TxAddr= (addr & 0x7f);
	HAL_SPI_Transmit(&hspi1, &TxAddr, 1, 1000);
	while(HAL_SPI_GetState(&hspi1)==HAL_SPI_STATE_BUSY_TX);
	HAL_SPI_Transmit(&hspi1, &data, 1, 1000);
	while(HAL_SPI_GetState(&hspi1)==HAL_SPI_STATE_BUSY_TX);
	HAL_Delay(1);
	HAL_GPIO_WritePin(CS1_Gyro_GPIO_Port, CS1_Gyro_Pin, GPIO_PIN_SET);
}

void Gyro_ReadBytes(uint8_t addr,uint8_t *pbuff,uint8_t len)
{
	HAL_GPIO_WritePin(CS1_Gyro_GPIO_Port, CS1_Gyro_Pin, GPIO_PIN_RESET);
	uint8_t temp = (addr | 0x80);
	HAL_SPI_Transmit(&hspi1, &temp, 1, 1000);
	while(HAL_SPI_GetState(&hspi1)== HAL_SPI_STATE_BUSY_RX);
	for(uint8_t i=0;i<len;i++)
	{
		HAL_SPI_Receive(&hspi1, &temp, 1, 1000);
		while(HAL_SPI_GetState(&hspi1)== HAL_SPI_STATE_BUSY_RX);
		pbuff[i]=temp;
	}
	HAL_GPIO_WritePin(CS1_Gyro_GPIO_Port, CS1_Gyro_Pin, GPIO_PIN_SET);
}

void Accel_WriteByte(uint8_t addr,uint8_t data)
{
	HAL_GPIO_WritePin(CS1_Accel_GPIO_Port, CS1_Accel_Pin, GPIO_PIN_RESET);
	uint8_t TxData = (addr & 0x7f) ;
	HAL_SPI_Transmit(&hspi1, &TxData, 1, 1000);
	while(HAL_SPI_GetState(&hspi1)==HAL_SPI_STATE_BUSY_TX);
	HAL_SPI_Transmit(&hspi1, &data, 1, 1000);
	while(HAL_SPI_GetState(&hspi1)==HAL_SPI_STATE_BUSY_TX);
	HAL_Delay(1);
	HAL_GPIO_WritePin(CS1_Accel_GPIO_Port, CS1_Accel_Pin, GPIO_PIN_SET);
}

void Accel_ReadBytes(uint8_t addr,uint8_t *pbuff,uint8_t len)
{
	HAL_GPIO_WritePin(CS1_Accel_GPIO_Port, CS1_Accel_Pin, GPIO_PIN_RESET);
	uint8_t temp= (addr | 0x80);
	HAL_SPI_Transmit(&hspi1, &temp, 1, 1000);
	while(HAL_SPI_GetState(&hspi1)== HAL_SPI_STATE_BUSY_RX);
	HAL_SPI_Receive(&hspi1, &temp, 1, 1000);
	while(HAL_SPI_GetState(&hspi1)== HAL_SPI_STATE_BUSY_RX);
	for(uint8_t i=0;i<len;i++)
	{
		HAL_SPI_Receive(&hspi1, &temp, 1, 1000);
		while(HAL_SPI_GetState(&hspi1)== HAL_SPI_STATE_BUSY_RX);
		pbuff[i]=temp;
	}
	HAL_GPIO_WritePin(CS1_Accel_GPIO_Port, CS1_Accel_Pin, GPIO_PIN_SET);
}
void BMI088_Init(void)
{
	Accel_WriteByte(ACC_SOFTRESET_ADDR, ACC_SOFTRESET_VAL);//软重启
	HAL_Delay(50);
	Accel_WriteByte(ACC_PWR_CTRL_ADDR, ACC_PWR_CTRL_ON); //打开加速度计电源
	Accel_WriteByte(ACC_RANGE_ADDR, ACC_RANGE_3G);
	Accel_WriteByte(ACC_CONF_ADDR,ACC_CONF_RESERVED<<7 |ACC_CONF_BWP_OSR4<<6 |ACC_CONF_ODR_200_Hz );

	Gyro_WriteByte(GYRO_SOFTRESET_ADDR, GYRO_SOFTRESET_VAL);//软重启
	HAL_Delay(50);
	Gyro_WriteByte(GYRO_LPM1_ADDR, GYRO_LPM1_NOR);//陀螺仪变成正常模式
	Gyro_WriteByte(GYRO_RANGE_ADDR, GYRO_RANGE_500_DEG_S);
	Gyro_WriteByte(GYRO_BANDWIDTH_ADDR, GYRO_ODR_200Hz_BANDWIDTH_64Hz);

	ImuData.angle_q[0]=1;
}

void Get_ImuData(ImuData_t *data)
{
	uint8_t buff_acc[ACC_XYZ_LEN],buff_gyro[GYRO_XYZ_LEN],buff_temp[TEMP_LEN];


	int16_t Accel[3],Gyro[3];

	Accel_ReadBytes(ACC_X_LSB_ADDR,buff_acc ,ACC_XYZ_LEN );
	Accel[0]=((int16_t)buff_acc[1] << 8) + (int16_t)buff_acc[0];
	Accel[1]=((int16_t)buff_acc[3] << 8) + (int16_t)buff_acc[2];
	Accel[2]=((int16_t)buff_acc[5] << 8) + (int16_t)buff_acc[4];
	data->Accel[0]=(float)Accel[0]*BMI088_ACCEL_3G_SEN;
	data->Accel[1]=(float)Accel[1]*BMI088_ACCEL_3G_SEN;
	data->Accel[2]=(float)Accel[2]*BMI088_ACCEL_3G_SEN;


	Gyro_ReadBytes(GYRO_RATE_X_LSB_ADDR, buff_gyro, GYRO_XYZ_LEN);
	Gyro[0]=((int16_t)buff_gyro[1] << 8) + (int16_t)buff_gyro[0];
	Gyro[1]=((int16_t)buff_gyro[3] << 8) + (int16_t)buff_gyro[2];
	Gyro[2]=((int16_t)buff_gyro[5] << 8) + (int16_t)buff_gyro[4];
	data->Gyro[0]=(float)Gyro[0] / 65.536f * DEG2SEC;
	data->Gyro[1]=(float)Gyro[1] / 65.536f * DEG2SEC;
	data->Gyro[2]=(float)Gyro[2] / 65.536f * DEG2SEC;


	Accel_ReadBytes(TEMP_MSB_ADDR, buff_temp, TEMP_LEN);
	uint16_t Temp=(buff_temp[0]<<3) +(buff_temp[1]>>5);
	int16_t Tempr=0;
	if(Temp >1023)
		Tempr = (int16_t)Temp -2048;
	else
		Tempr = (int16_t)Temp;
	data->temperature = Tempr * TEMP_UNIT +TEMP_BIAS;



	MahonyAHRSupdateIMU(data->angle_q, data->Gyro[0], data->Gyro[1], data->Gyro[2], data->Accel[0], data->Accel[1], data->Accel[2]);
	data->angle[0] = atan2f(2.0f*(data->angle_q[0]*data->angle_q[3]+data->angle_q[1]*data->angle_q[2]), 2.0f*(data->angle_q[0]*data->angle_q[0]+data->angle_q[1]*data->angle_q[1])-1.0f);
	data->angle[1] = asinf(-2.0f*(data->angle_q[1]*data->angle_q[3]-data->angle_q[0]*data->angle_q[2]));
	data->angle[2] = atan2f(2.0f*(data->angle_q[0]*data->angle_q[1]+data->angle_q[2]*data->angle_q[3]),2.0f*(data->angle_q[0]*data->angle_q[0]+data->angle_q[3]*data->angle_q[3])-1.0f);
}


