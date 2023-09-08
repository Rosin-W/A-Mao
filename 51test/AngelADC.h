#include	<adc.h>

extern unsigned char num_Angle;

extern float ang_initial[8];
extern float ang_p[8];
extern float ang_adc[8];
extern float ang_kf[8];


void	ADC_config(void);
void Angle_Init(void);
void Anglefilt(void);
void AngleRead(void);
