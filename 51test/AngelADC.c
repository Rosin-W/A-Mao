#include 	"AngelADC.h"

void	ADC_config(void)
{
	ADC_InitTypeDef		ADC_InitStructure;				//�ṹ����
	ADC_InitStructure.ADC_Px        = ADC_P1_All ;	//����Ҫ��ADC��IO,	ADC_P10 ~ ADC_P17(�����),ADC_P1_All
	ADC_InitStructure.ADC_Speed     = ADC_360T;			//ADC�ٶ�			ADC_90T,ADC_180T,ADC_360T,ADC_540T
	ADC_InitStructure.ADC_Power     = ENABLE;			//ADC��������/�ر�	ENABLE,DISABLE
	ADC_InitStructure.ADC_AdjResult = ADC_RES_H8L2;		//ADC�������,	ADC_RES_H2L8,ADC_RES_H8L2
	ADC_InitStructure.ADC_Polity    = PolityHigh;		//���ȼ�����	PolityHigh,PolityLow
	ADC_InitStructure.ADC_Interrupt = DISABLE;			//�ж�����		ENABLE,DISABLE
	ADC_Inilize(&ADC_InitStructure);					//��ʼ��
	ADC_PowerControl(ENABLE);							//������ADC��Դ��������, ENABLE��DISABLE
}

unsigned char i = 0;

void Angle_Init(void)
{
	ADC_config();
	for(i=0;i<8;i++)//�Ƕȴ�������س�ֵ�趨
	{
		ang_initial[i]=0;
		ang_adc[i] = (float)Get_ADC10bitResult(i)/2.86;
		ang_kf[i] = ang_adc[i];
		ang_p[i]=0.05;
	}
}

unsigned char F = 1;//״̬ת�ƾ���
unsigned char F_ = 1;
unsigned char H = 1;//״̬�۲����
float Q = 0.8;//������������
unsigned int R = 0.2;//�۲���������
float X_,P_,K;

void AngleRead(void)
{//�Ƕȴ�������ֵ��ȡ
	for(i=0;i<8;i++)
	{ang_adc[i] = (float)Get_ADC10bitResult(i)/2.86;}//�Ƕ���ֵADC��ȡ
	ang_adc[1]=-ang_adc[1];//���ڰ�װλ�ò�ͬ�����𴫸�����ֵ��Ҫ����
	ang_adc[2]=-ang_adc[2];
	ang_adc[5]=-ang_adc[5];
	ang_adc[6]=-ang_adc[6];
}

void Anglefilt(void)
{//�Ƕȴ�������ֵ�������˲�
	AngleRead();
	for(i=0;i<8;i++)
	{
		ang_adc[i]=ang_adc[i]-ang_initial[i];//ȥ����ʼֵ���ԽǶȱ仯����ֵ����
		X_ = F*ang_kf[i];//������һʱ��Ԥ�Ƶ�ǰ�۲�ֵ������׼ȷֵ
		P_ = F*ang_p[i]*F_+Q;//������һʱ��Э����Ԥ�⵱ǰЭ����
		K = ang_p[i]*(H*ang_p[i]*H+R);//������ϵ��K��Ȩ��Ԥ��P�͹۲�R
		ang_kf[i] = X_+K*(ang_adc[i]-H*X_);//ʵ�ʹ۲�ֵ��Ԥ�ƹ۲�ֵ���ϲв�����
		ang_p[i] = (1-K*H)*P_;//������ѹ���ֵ�������ֲ������ֵ������һ���˲�
	}
}
