#include	<adc.h>

extern unsigned char num_Angel;

extern float ang_p[4];
extern float ang_adc[4];
extern float ang_kf[4];


void	ADC_config(void);
void Angel_Init(void);
void Angelfilt(void);
