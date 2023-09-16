#include "remote.h"
#include "main.h"
#include "usart.h"
#include <stdio.h>




uint8_t RecieveDate[18]={0};
RemoteData_t RemoteData={0};

void RemoteInit(void)
{
	RemoteData.RemoteOrBoard=Remote;

}

void RemoteData_Unpack(void)
{
	Remote_t Temp={0};
	Temp. ch0 = ((RecieveDate[1]<<8)|RecieveDate[0])&0x7ff;
	Temp. ch0 -=1024;
	Temp. ch1 = ((RecieveDate[2]<<5)|(RecieveDate[1]>>3))&0x7ff;
	Temp. ch1 -=1024;
	Temp. ch2 = ((RecieveDate[4]<<10)|(RecieveDate[3]<<2)|(RecieveDate[2]>>6))&0x7ff;
	Temp. ch2 -=1024;
	Temp. ch3 = ((RecieveDate[5]<<7)|(RecieveDate[4]>>1))&0x7ff;
	Temp. ch3 -=1024;
	Temp. wheel =(RecieveDate[17]<<8)|(RecieveDate[16]);
	Temp. wheel -=1024;

	Temp. s1 =(RecieveDate[5]>>4)&0x0003;
	Temp. s2 =((RecieveDate[5]>>4)&0x000c)>>2;

	Temp. X = (RecieveDate[7]<<8)|RecieveDate[6];
	Temp. Y = (RecieveDate[9]<<8)|RecieveDate[8];
	Temp. Z = (RecieveDate[11]<<8)|RecieveDate[10];
	Temp. L = RecieveDate[12];
	Temp. R = RecieveDate[13];

	uint16_t button = (RecieveDate[15]<<8)|RecieveDate[14];
	Temp. W = button & 0x80;
	Temp. S = button & 0x40;
	Temp. A = button & 0x20;
	Temp. D = button & 0x10;
	Temp. Q = button & 0x08;
	Temp. E = button & 0x84;
	Temp. Shirt = button & 0x02;
	Temp. Ctrl = button & 0x01;


	if(RemoteData.RemoteOrBoard== Remote)
	{
		RemoteData.TargetSpeedX =Temp.ch3 * ChassisSpeed_RemoteChange;
		RemoteData.TargetSpeedY =Temp.ch2 * ChassisSpeed_RemoteChange;
		RemoteData.TargetSpeedYaw=Temp.ch0 * YawSpeed_RemoteChange;
		RemoteData.TargetSpeedPitch=Temp.ch1 *PitchSpeed_RemoteChange;
	}
	else if(RemoteData.RemoteOrBoard==Board)
	{
		RemoteData.TargetSpeedX =(Temp.W -Temp.S) * ChassisSpeed_BoardChange;
		RemoteData.TargetSpeedY =(Temp.A -Temp.D) * ChassisSpeed_BoardChange;
		if(Temp.L)
		{
			RemoteData.TargetSpeedYaw=Temp.X * -1 * YawSpeed_BoardChange;
			RemoteData.TargetSpeedPitch=Temp.Y * -1 * PitchSpeed_BoardChange;
		}
		else
		{
			RemoteData.TargetSpeedYaw=0;
			RemoteData.TargetSpeedPitch=0;
		}
	}
	RemoteData.ChassisStatus=Temp.s1;

	RemoteData.FireType=Temp.s2;

	if(Temp.wheel>660)
		RemoteData.ShootStrategy=MultiShoot;
	else
		RemoteData.ShootStrategy=SingleShoot;

}


