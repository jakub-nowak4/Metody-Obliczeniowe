#include "common.h"
#include "sieczne.h"

void metoda_siecznych(double (*f)(double), double x0, double x1, double tolx, double tolf, int max_iter)
{
    cout << "=== METODA SIECZNYCH" << endl;

    double xn = x0;
    double xn1 = x1;
    double xn2;
    double fxn2;

    double estymator_bledu;

    cout << fixed << setprecision(16);

    int i = 0;
    while (i < max_iter)
    {
        xn2 = xn1 - f(xn1) / ((f(xn1) - f(xn)) / (xn1 - xn));
        fxn2 = f(xn2);

        estymator_bledu = xn2 - xn1;

        cout << "Iteracja i = " << setw(3) << i
             << " | x{n + 2} = " << setw(20) << xn2
             << " | f(x{n + 2}) = " << setw(20) << fxn2
             << " | x{n + 1} = " << setw(20) << xn1
             << " | xn = " << setw(20) << xn
             << " | en = " << setw(20) << fabs(estymator_bledu) << endl;

        // Sprawdzenie K2 oraz K3
        if (fabs(estymator_bledu) <= tolx && fabs(fxn2) <= tolf)
        {
            cout << "\nZnaleziono pierwiastek! xn = " << xn2 << endl;
            cout << endl;
            return;
        }

        xn = xn1;
        xn1 = xn2;

        i++;
    }

    if (i >= max_iter)
    {
        cout << "Kryterium 1 nie spełnione: przekroczono liczbę max iteracji" << endl;
        cout << "\nNajlepsze wyznaczone przybliżenie: xn = " << xn2 << endl;
        cout << endl;
    }
}