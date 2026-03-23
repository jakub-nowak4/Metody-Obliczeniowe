#include "common.h"
#include "bisekcja.h"

void metoda_bisekcji(double (*f)(double), double a, double b, double tolx, double tolf, int max_iter)
{
    cout << "=== METODA BISEKCJI ===" << endl;

    // FUNKCJA MUSI BYĆ CIĄGŁA!
    // Sprawdzamy czy założenia początkowe są spełnione
    double fx_a = f(a);
    double fx_b = f(b);

    if (fx_a * fx_b > 0.0)
    {
        cout << "Warunek początkowy nie spełniony fx(a) * fx(b) > 0 => krańce przedziału mają ten sam znak!" << endl;
        cout << endl;
        return;
    }

    double an = a;
    double fx_an;

    double bn = b;
    double fx_bn;

    double xn;
    double fx_xn;

    double estymator_bledu;

    cout << fixed << setprecision(16);

    int i = 0;
    while (i < max_iter)
    {
        xn = (an + bn) / 2;
        fx_xn = f(xn);

        fx_an = f(an);
        fx_bn = f(bn);

        estymator_bledu = (bn - an) / 2.0;

        cout << "Iteracja i = " << setw(3) << i
             << " | an = " << setw(20) << an
             << " | f(an) = " << setw(20) << fx_an
             << " | bn = " << setw(20) << bn
             << " | f(bn) = " << setw(20) << fx_bn
             << " | xn = " << setw(20) << xn
             << " | f(xn) = " << setw(20) << fx_xn
             << " | en = " << setw(20) << fabs(estymator_bledu) << endl;

        // Sprawdzenie K2 oraz K3
        if (fabs(estymator_bledu) <= tolx && fabs(fx_xn) <= tolf)
        {
            cout << "\nZnaleziono pierwiastek! xn = " << xn << endl;
            cout << endl;
            return;
        }

        // Wyznaczenie krancy nowego pod przedzialu
        if (fx_an * fx_xn < 0)
        {
            bn = xn;
        }
        else
        {
            an = xn;
        }

        i++;
    }

    if (i >= max_iter)
    {
        cout << "Kryterium 1 nie spełnione: przekroczono liczbę max iteracji" << endl;
        cout << "\nNajlepsze wyznaczone przybliżenie: xn = " << xn << endl;
        cout << endl;
    }
}
