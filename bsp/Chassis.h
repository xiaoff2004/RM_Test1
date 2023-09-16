#ifndef __CHASSIS_H__
#define __CHASSIS_H__



//麦轮底盘轴距（长）
#define MECANUM_WHEEL_BASE 0.30f
//麦轮底盘轮距（宽）
#define MECANUM_WHEEL_TARCK 0.33f
//麦轮半径
#define MECANUM_WHEEL_RADIUS 0.075f
//全向轮底盘半径
#define OMNI_RADIUS 0.30f
//全向轮半径
#define OMNI_WhEEL_RADIUS 0.075f




void Chassis_Init(void);
void Chassis_UpData(void);




#endif
