#include <math.h>

unsigned char goaloop=0;
extern float ang_goal[8];
float Lh = 250;//大腿长度
float Lk = 250;//小腿长度

void SetGoal(void)
{
	//Balance();
	goaloop++;
	switch(goaloop)
	{
		case 1 :	ang_goal[0]=30;
							ang_goal[1]=30;
							ang_goal[2]=60;
							ang_goal[3]=0;
							ang_goal[4]=60;
							ang_goal[5]=0;
							ang_goal[6]=0;
							ang_goal[7]=30;
							break;
		
		case 2 :	ang_goal[0]=0;
							ang_goal[1]=0;
		
							ang_goal[2]=0;
							ang_goal[3]=30;
							ang_goal[4]=0;
							ang_goal[5]=30;
							ang_goal[6]=60;
							ang_goal[7]=0;
							break;
		
//		case 3 :	ang_goal[0]=0;break;
//		case 4 :	ang_goal[1]=0;break;
		default:	goaloop=0;break;
	}
}

float IK_AngH(float x, float y)
{//运动学逆解髋关节角度
	float AngH = atan(y/(-x)) - acos((sqrt(x*x+y*y))/2*Lh);
	return AngH;
}

float IK_AngK(float x,float y)
{//运动学逆解膝关节角度
	float AngK = 3.14 - acos((x*x+y*y-Lh*Lh-Lk*Lk)/2*Lk*Lh);
	return AngK;
}

//平衡
float blcP = 0.5;
float blcX0 = 0.0;
float blcY0 = 350.0;
float height_1 = 0;//左右腿高度差改变
float height_2 = 0;//前后腿高度差改变
float blc_y[4];
extern float AngleRoll;
extern float AnglePitch;
unsigned char blci=0;
unsigned char blcj=0;

void Balance()
{
	height_1 = blcP * AngleRoll;
	height_2 = blcP * AnglePitch;
	blc_y[0] +=  height_1 +  height_2;//左前
	blc_y[1] +=  height_1 + -height_2;//左后
	blc_y[2] += -height_1 +  height_2;//右前
	blc_y[3] += -height_1 + -height_2;//右后

	for(blci=0;blci<8;blci+=2)
	{//髋关节
		ang_goal[blci]=IK_AngH(blcX0,blc_y[blcj]);
		blcj++;
	}
	blcj=0;
	for(blci=1;blci<8;blci+=2)
	{//膝关节
		ang_goal[blci]=IK_AngK(blcX0,blc_y[blcj]);
		blcj++;
	}
}

const float	code walk[][4] =	
{
	24.0, 107.5, 62.5, 93.5,
	49.5, 63.5, 62.5, 93.5, 
	62.5, 93.5, 24.0, 107.5,
	62.5, 93.5, 49.5, 63.5,
};
