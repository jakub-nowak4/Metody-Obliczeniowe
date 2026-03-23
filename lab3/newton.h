#ifndef NEWTON_H
#define NEWTON_H

void metoda_newtona(double (*f)(double), double (*df)(double), double x0, double tolx, double tolf, int max_iter);

#endif