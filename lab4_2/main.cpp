#include <iostream>
#include <cmath>
#include <iomanip>
#include <algorithm>
using namespace std;

#define TOLX 1e-11
#define TOLY 1e-11

#define MAX_ITER 1000

void metoda_newtona_uogolniona(double x0, double y0, double z0, double tolx, double tolf, int max_iter);

int main()
{
    metoda_newtona_uogolniona(1.0, 1.0, 1.0, TOLX, TOLY, MAX_ITER);
    return 0;
}

void metoda_newtona_uogolniona(double x0, double y0, double z0, double tolx, double tolf, int max_iter)
{
    cout << "=== UOGOLNIONA METODA NEWTONA ===" << endl;
    cout << fixed << setprecision(16);

    double xn = x0;
    double yn = y0;
    double zn = z0;

    int iter = 0;
    while (iter < max_iter)
    {
        // Macierz funkcji
        double f[3] = {
            pow(xn, 2) + pow(yn, 2) + pow(zn, 2) - 4.0,
            pow(xn, 2) + pow(yn, 2) / 2.0 - 1.0,
            xn * yn - 0.5};

        double J[3][3] = {
            {2.0 * xn, 2.0 * yn, 2.0 * zn},
            {2.0 * xn, yn, 0.0},
            {yn, xn, 0.0}};

        // Macierz rozszerzona [J | f] — 3 wiersze i 4 kolumny
        double A[3][4] = {
            {J[0][0], J[0][1], J[0][2], f[0]},
            {J[1][0], J[1][1], J[1][2], f[1]},
            {J[2][0], J[2][1], J[2][2], f[2]}};

        // Eliminacja Gaussa do macierz trójkątnej górnej
        for (int col = 0; col < 3; col++)
        {
            for (int row = col + 1; row < 3; row++)
            {
                // Mnoznik do odjecia wiersza
                double ratio = A[row][col] / A[col][col];
                for (int k = col; k < 4; k++)
                    A[row][k] -= ratio * A[col][k];
            }
        }

        // Wyliczamy poszczegolne delta zaczynac od delta_z
        double delta[3];
        for (int i = 2; i >= 0; i--)
        {
            delta[i] = A[i][3]; // poczatkowa delta  = wynik funkcji
            for (int j = i + 1; j < 3; j++)
                delta[i] -= A[i][j] * delta[j]; // od delty odejmujemy juz wczesniej wyliczone delty * wspolcznnik
            delta[i] /= A[i][i];                // delte ktora liczymy dzielimy przez wspolcznnik
        }

        xn -= delta[0];
        yn -= delta[1];
        zn -= delta[2];

        double fn_0 = pow(xn, 2) + pow(yn, 2) + pow(zn, 2) - 4.0;
        double fn_1 = pow(xn, 2) + pow(yn, 2) / 2.0 - 1.0;
        double fn_2 = xn * yn - 0.5;

        cout << "Iteracja i = " << setw(3) << iter
             << " | xn = " << setw(18) << xn
             << " | yn = " << setw(18) << yn
             << " | zn = " << setw(18) << zn << endl;

        // Sprawdzenie zatrzymania pętli
        if (abs(delta[0]) < tolx && abs(delta[1]) < tolx && abs(delta[2]) < tolx &&
            abs(fn_0) < tolf && abs(fn_1) < tolf && abs(fn_2) < tolf)
        {
            cout << "\nZnaleziono pierwiastek układu!" << endl;
            cout << "x = " << xn << "\ny = " << yn << "\nz = " << zn << "\n"
                 << endl;
            return;
        }

        iter++;
    }

    if (iter >= max_iter)
    {
        cout << "Kryterium 1 nie spełnione: przekroczono liczbę max iteracji" << endl;
        cout << "\nNajlepsze wyznaczone przybliżenie:" << endl;
        cout << "x = " << xn << "\ny = " << yn << "\nz = " << zn << "\n"
             << endl;
    }
}
