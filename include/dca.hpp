#ifndef _DCA_HPP_
#define _DCA_HPP_

extern double MotorPriority[5];

#define DRIVE 0
#define ARM 1
#define MOGO 2
#define ONE 3
#define INTAKE 4

void dynamic_current_task(void* param);

void setDriveCurrent(double c);
void setArmCurrent(double c);
void setMogoLiftCurrent(double c);
void setOneBarCurrent(double c);
void setIntakeCurrent(double c);

#endif