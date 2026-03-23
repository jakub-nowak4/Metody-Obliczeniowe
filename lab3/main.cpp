#include "common.h"
#include "bisekcja.h"
#include "picard.h"
#include "newton.h"
#include "sieczne.h"

#define MAX_ITER 1000
#define TOLX 1e-11
#define TOLF 1e-11

double f_a(double x);
double df_a(double x);
double f_a_picard(double x);
double df_a_picard(double x);

double f_b(double x);
double df_b(double x);
double f_b_picard(double x);
double df_b_picard(double x);

int main()
{
    cout << "== WYWOŁANIA METOD ITERACYJNYCH dla f(x) = tanh(x) + 2x - 2  ==" << endl;
    metoda_picarda(f_a, f_a_picard, df_a_picard, 0.0, TOLX, TOLF, MAX_ITER);
    metoda_bisekcji(f_a, 0.0, 1.0, TOLX, TOLF, MAX_ITER);
    metoda_newtona(f_a, df_a, 0.0, TOLX, TOLF, MAX_ITER);
    metoda_siecznych(f_a, -0.5, 0.5, TOLX, TOLF, MAX_ITER);

    cout << "\n\n== WYWOŁANIA METOD ITERACYJNYCH dla f(x) = sinh(x) + x/4 - 1 ==" << endl;
    metoda_picarda(f_b, f_b_picard, df_b_picard, 0.0, TOLX, TOLF, MAX_ITER);
    metoda_bisekcji(f_b, 0.0, 1.0, TOLX, TOLF, MAX_ITER);
    metoda_newtona(f_b, df_b, 0.0, TOLX, TOLF, MAX_ITER);
    metoda_siecznych(f_b, -0.5, 0.5, TOLX, TOLF, MAX_ITER);
    return 0;
}

double f_a(double x)
{
    return tanh(x) + 2.0 * x - 2.0;
}
double df_a(double x)
{
    return (1.0 / pow(cosh(x), 2)) + 2.0;
}

double f_a_picard(double x)
{
    return 1.0 - (tanh(x) / 2.0);
}
double df_a_picard(double x)
{
    return -0.5 * (1.0 / pow(cosh(x), 2));
}

double f_b(double x)
{
    return sinh(x) + (x / 4.0) - 1.0;
}
double df_b(double x)
{
    return cosh(x) + 0.25;
}
double f_b_picard(double x)
{
    return asinh(1.0 - 0.25 * x);
}
double df_b_picard(double x)
{
    return -0.25 / sqrt(1.0 + pow((1.0 - 0.25 * x), 2));
}