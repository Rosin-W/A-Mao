//全局变量
unsigned char num_Angel=8;

//角度传感器及其卡尔曼滤波
float ang_initial[8];
float ang_p[8];
float ang_adc[8];
float ang_kf[8];

//气缸驱动及其PID控制
float ang_goal[8];
