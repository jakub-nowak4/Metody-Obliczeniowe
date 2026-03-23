#include "common.h"
#include "picard.h"

void metoda_picarda(double (*f)(double), double (*phi)(double), double (*dphi)(double), double x0, double tolx, double tolf, int max_iter)
{
    cout << "=== METODA PICARDA ===" << endl;

    // Phi(x) musi być różniczkowalne
    double xn_poprzednie = x0;

    double xn;
    double fx_xn;
    double zbieznosc;
    double estymator_bledu;

    cout << fixed << setprecision(16);

    int i = 0;
    while (i < max_iter)
    {
        // Sprawdzamy zbieżność przed iteracją

        xn = phi(xn_poprzednie);
        fx_xn = f(xn);
        estymator_bledu = xn - xn_poprzednie;
        zbieznosc = fabs(dphi(xn));

        cout << "Iteracja i = " << setw(3) << i
             << " | xn = " << setw(20) << xn
             << " | f(xn) = " << setw(20) << fx_xn
             << " | |phi'(xn)| = " << setw(20) << zbieznosc
             << " | en = " << setw(20) << fabs(estymator_bledu) << endl;

        if (zbieznosc >= 1.0)
        {
            cout << "UWAGA: |phi'(x)| wynosi " << zbieznosc << " (>= 1.0) - Iteracja rozbieżna!" << endl;
            cout << endl;
            return;
        }

        // Sprawdzenie K2 oraz K3
        if (fabs(estymator_bledu) <= tolx && fabs(fx_xn) <= tolf)
        {
            cout << "\nZnaleziono pierwiastek! xn = " << xn << endl;
            cout << endl;
            return;
        }

        xn_poprzednie = xn;

        i++;
    }

    if (i >= max_iter)
    {
        cout << "Kryterium 1 nie spełnione: przekroczono liczbę max iteracji" << endl;
        cout << "\nNajlepsze wyznaczone przybliżenie: xn = " << xn << endl;
        cout << endl;
    }
}
