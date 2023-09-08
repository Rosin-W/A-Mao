#include 	"AngelADC.h"

void	ADC_config(void)
{
	ADC_InitTypeDef		ADC_InitStructure;				//结构定义
	ADC_InitStructure.ADC_Px        = ADC_P1_All ;	//设置要做ADC的IO,	ADC_P10 ~ ADC_P17(或操作),ADC_P1_All
	ADC_InitStructure.ADC_Speed     = ADC_360T;			//ADC速度			ADC_90T,ADC_180T,ADC_360T,ADC_540T
	ADC_InitStructure.ADC_Power     = ENABLE;			//ADC功率允许/关闭	ENABLE,DISABLE
	ADC_InitStructure.ADC_AdjResult = ADC_RES_H8L2;		//ADC结果调整,	ADC_RES_H2L8,ADC_RES_H8L2
	ADC_InitStructure.ADC_Polity    = PolityHigh;		//优先级设置	PolityHigh,PolityLow
	ADC_InitStructure.ADC_Interrupt = DISABLE;			//中断允许		ENABLE,DISABLE
	ADC_Inilize(&ADC_InitStructure);					//初始化
	ADC_PowerControl(ENABLE);							//单独的ADC电源操作函数, ENABLE或DISABLE
}

unsigned char i = 0;

void Angle_Init(void)
{
	ADC_config();
	for(i=0;i<8;i++)//角度传感器相关初值设定
	{
		ang_initial[i]=0;
		ang_adc[i] = (float)Get_ADC10bitResult(i)/2.86;
		ang_kf[i] = ang_adc[i];
		ang_p[i]=0.05;
	}
}

unsigned char F = 1;//状态转移矩阵
unsigned char F_ = 1;
unsigned char H = 1;//状态观测矩阵
float Q = 0.8;//测量噪声方差
unsigned int R = 0.2;//观测噪声方差
float X_,P_,K;

void AngleRead(void)
{//角度传感器数值读取
	for(i=0;i<8;i++)
	{ang_adc[i] = (float)Get_ADC10bitResult(i)/2.86;}//角度数值ADC读取
	ang_adc[1]=-ang_adc[1];//由于安装位置不同，个别传感器数值需要修正
	ang_adc[2]=-ang_adc[2];
	ang_adc[5]=-ang_adc[5];
	ang_adc[6]=-ang_adc[6];
}

void Anglefilt(void)
{//角度传感器数值卡尔曼滤波
	AngleRead();
	for(i=0;i<8;i++)
	{
		ang_adc[i]=ang_adc[i]-ang_initial[i];//去除初始值，以角度变化绝对值计算
		X_ = F*ang_kf[i];//根据上一时刻预计当前观测值，不是准确值
		P_ = F*ang_p[i]*F_+Q;//根据上一时刻协方差预测当前协方差
		K = ang_p[i]*(H*ang_p[i]*H+R);//卡尔曼系数K，权衡预测P和观察R
		ang_kf[i] = X_+K*(ang_adc[i]-H*X_);//实际观测值由预计观测值加上残差修正
		ang_p[i] = (1-K*H)*P_;//更新最佳估计值的噪声分布，输出值用于下一次滤波
	}
}
