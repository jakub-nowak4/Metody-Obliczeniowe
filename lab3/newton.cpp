#include "common.h"
#include "newton.h"

void metoda_newtona(double (*f)(double), double (*df)(double), double x0, double tolx, double tolf, int max_iter)
{
    cout << "=== METODA NEWTONA ===" << endl;

    double xn_poprzednie = x0;
    double df_xn_poprzednie;
    double fx_xn_poprzednie;

    double xn;
    double fx_xn;
    double estymator_bledu;

    cout << fixed << setprecision(16);

    int i = 0;
    while (i < max_iter)
    {
        fx_xn_poprzednie = f(xn_poprzednie);
        df_xn_poprzednie = df(xn_poprzednie);

        // Zabezpieczenie przed dzieleniem przez zero w we wzorze Newtona
        if (fabs(df_xn_poprzednie) < 1e-14)
        {
            cout << "Błąd: Pochodna w punkcie x{n-1} (" << xn_poprzednie << ") jest bliska zeru (" << df_xn_poprzednie << "). Przerywam by uniknąć dzielenia przez zero!" << endl;
            return;
        }

        xn = xn_poprzednie - (fx_xn_poprzednie / df_xn_poprzednie);
        fx_xn = f(xn);

        estymator_bledu = xn - xn_poprzednie;

        cout << "Iteracja i = " << setw(3) << i
             << " | xn = " << setw(20) << xn
             << " | f(xn) = " << setw(20) << fx_xn
             << " | x{n-1} = " << setw(20) << xn_poprzednie
             << " | f(x{n-1}) = " << setw(20) << fx_xn_poprzednie
             << " | f'(x{n-1}) = " << setw(20) << df_xn_poprzednie
             << " | en = " << setw(20) << fabs(estymator_bledu) << endl;

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