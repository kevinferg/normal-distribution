#ifndef FASTTRIG_H
#define FASTTRIG_H

/**********************************************
 *  Bhaskara I's sine approximation formula
 * 
 * See: 
 * https://en.wikipedia.org/wiki/Bh%C4%81skara_I%27s_sine_approximation_formula
 * 
 **********************************************/

#define FASTPI 3.14159265358979323846

double fastsin(double x);
double fastcos(double x);

int test_fasttrig(void);

#endif