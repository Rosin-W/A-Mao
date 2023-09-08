#include	<config.h>
#include	<delay.h>

extern float ang_kf[8];
extern float ang_goal[8];

void Cy_Init(void);
void CyCtrl(void);
void Cy_Timer_Init(void);

void Cy_Push(void);