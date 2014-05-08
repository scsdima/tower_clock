#ifndef __MOTOR_H
#define __MOTOR_H

enum {STEP_LEFT,STEP_RIGHT};
void MotorStep(int step,char dir);

void MotorCenter();
 void stepRight(char n);
 void stepLeft(char n);
#endif