#include <math.h>

unsigned char goaloop=0;
extern float ang_goal[8];
float Lh = 250;//���ȳ���
float Lk = 250;//С�ȳ���

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
{//�˶�ѧ����ŹؽڽǶ�
	float AngH = atan(y/(-x)) - acos((sqrt(x*x+y*y))/2*Lh);
	return AngH;
}

float IK_AngK(float x,float y)
{//�˶�ѧ���ϥ�ؽڽǶ�
	float AngK = 3.14 - acos((x*x+y*y-Lh*Lh-Lk*Lk)/2*Lk*Lh);
	return AngK;
}

//ƽ��
float blcP = 0.5;
float blcX0 = 0.0;
float blcY0 = 350.0;
float height_1 = 0;//�����ȸ߶Ȳ�ı�
float height_2 = 0;//ǰ���ȸ߶Ȳ�ı�
float blc_y[4];
extern float AngleRoll;
extern float AnglePitch;
unsigned char blci=0;
unsigned char blcj=0;

void Balance()
{
	height_1 = blcP * AngleRoll;
	height_2 = blcP * AnglePitch;
	blc_y[0] +=  height_1 +  height_2;//��ǰ
	blc_y[1] +=  height_1 + -height_2;//���
	blc_y[2] += -height_1 +  height_2;//��ǰ
	blc_y[3] += -height_1 + -height_2;//�Һ�

	for(blci=0;blci<8;blci+=2)
	{//�Źؽ�
		ang_goal[blci]=IK_AngH(blcX0,blc_y[blcj]);
		blcj++;
	}
	blcj=0;
	for(blci=1;blci<8;blci+=2)
	{//ϥ�ؽ�
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
