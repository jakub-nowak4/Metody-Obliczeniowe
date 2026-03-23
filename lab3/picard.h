#ifndef PICARD_H
#define PICARD_H

void metoda_picarda(double (*f)(double), double (*phi)(double), double (*dphi)(double), double x0, double tolx, double tolf, int max_iter);

#endif
