#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <vector>
using namespace std;

/* PARAMETRY ZADANIA */
const double A = 35.0;
const int LICZBA_PUNKTOW_SIATKI = 1000;

double funkcja(double x)
{
    return x / (1.0 + A * x * x * x * x);
}

/* ILORAZY ROZNICOWE DO POSTACI NEWTONA */
void ilorazy_roznicowe(const vector<double>& x, const vector<double>& y, vector<double>& c, int n)
{
    int i, k;
    for (i = 0; i <= n; i++) {
        c[i] = y[i];
    }

    for (k = 1; k <= n; k++) {
        for (i = n; i >= k; i--) {
            c[i] = (c[i] - c[i - 1]) / (x[i] - x[i - k]);
        }
    }
}

/* WARTOSC WIELOMIANU NEWTONA W PUNKCIE t */
double wartosc_newtona(const vector<double>& x, const vector<double>& c, int n, double t)
{
    int i;
    double wynik = c[n];
    for (i = n - 1; i >= 0; i--) {
        wynik = wynik * (t - x[i]) + c[i];
    }
    return wynik;
}

/* WEZLY CZEBYSZEWA: x_k = -cos(pi*(2k-1)/(2n)), k=1,...,n */
void wezly_czebyszewa(vector<double>& x, int n)
{
    int k;
    const double pi = acos(-1.0);
    for (k = 1; k <= n; k++) {
        x[k - 1] = -cos(pi * (2.0 * k - 1.0) / (2.0 * n));
    }
    sort(x.begin(), x.end());
}

/* WEZLY ROWNOODLEGLE DLA WIELOMIANU STOPNIA n */
void wezly_rownoodlegle(vector<double>& x, int n)
{
    int i;
    for (i = 0; i <= n; i++) {
        x[i] = -1.0 + 2.0 * i / n;
    }
}

double punkt_siatki(int i)
{
    return -1.0 + 2.0 * i / (LICZBA_PUNKTOW_SIATKI - 1);
}

/* ZAPIS WYKRESU: x, p(x), f(x); zwraca blad maksymalny */
double zapisz_interpolacje(const char* nazwa_pliku,
                           const vector<double>& wezly,
                           const vector<double>& wsp,
                           int stopien,
                           double limit_obciecia)
{
    int i;
    FILE* plik = fopen(nazwa_pliku, "w");
    if (!plik) {
        perror(nazwa_pliku);
        exit(1);
    }

    double blad_maks = 0.0;
    for (i = 0; i < LICZBA_PUNKTOW_SIATKI; i++) {
        double x = punkt_siatki(i);
        double fx = funkcja(x);
        double px = wartosc_newtona(wezly, wsp, stopien, x);
        double blad = fabs(px - fx);
        if (blad > blad_maks) blad_maks = blad;

        double y_wykres = px;
        if (fabs(y_wykres) > limit_obciecia) {
            y_wykres = (y_wykres < 0.0 ? -limit_obciecia : limit_obciecia);
        }
        fprintf(plik, "%.15e %.15e %.15e\n", x, y_wykres, fx);
    }

    fclose(plik);
    return blad_maks;
}

void zapisz_funkcje(double& max_abs_f)
{
    int i;
    FILE* plik = fopen("funkcja.dat", "w");
    if (!plik) {
        perror("funkcja.dat");
        exit(1);
    }

    max_abs_f = 0.0;
    for (i = 0; i < LICZBA_PUNKTOW_SIATKI; i++) {
        double x = punkt_siatki(i);
        double fx = funkcja(x);
        if (fabs(fx) > max_abs_f) max_abs_f = fabs(fx);
        fprintf(plik, "%.15e %.15e\n", x, fx);
    }

    fclose(plik);
}

void zapisz_skrypty_gnuplota()
{
    FILE* gp = fopen("plot1.gp", "w");
    if (!gp) {
        perror("plot1.gp");
        exit(1);
    }
    fprintf(gp,
            "set terminal pngcairo size 1000,700\n"
            "set output 'wykres1_rownoodlegle.png'\n"
            "set title \"Zjawisko Rungego -- wezly rownoodlegle, a=25\"\n"
            "set xlabel \"x\"; set ylabel \"y\"\n"
            "set yrange [-0.6:0.6]\n"
            "set key top left\n"
            "plot 'funkcja.dat'           u 1:2 w l lw 3 lc 'black' title 'f(x)',\\\n"
            "     'interp_unif_n5.dat'    u 1:2 w l lw 1.5 dt 2 title 'n=5',\\\n"
            "     'interp_unif_n10.dat'   u 1:2 w l lw 1.5 dt 3 title 'n=10',\\\n"
            "     'interp_unif_n15.dat'   u 1:2 w l lw 1.5 dt 4 title 'n=15',\\\n"
            "     'interp_unif_n20.dat'   u 1:2 w l lw 1.5       title 'n=20'\n");
    fclose(gp);

    gp = fopen("plot2.gp", "w");
    if (!gp) {
        perror("plot2.gp");
        exit(1);
    }
    fprintf(gp,
            "set terminal pngcairo size 1000,700\n"
            "set output 'wykres2_czebyszew.png'\n"
            "set title \"Zjawisko Rungego -- wezly Czebyszewa, a=25\"\n"
            "set xlabel \"x\"; set ylabel \"y\"\n"
            "set yrange [-0.6:0.6]\n"
            "set key top left\n"
            "plot 'funkcja.dat'           u 1:2 w l lw 3 lc 'black' title 'f(x)',\\\n"
            "     'interp_cheb_n5.dat'    u 1:2 w l lw 1.5 dt 2 title 'n=5',\\\n"
            "     'interp_cheb_n10.dat'   u 1:2 w l lw 1.5 dt 3 title 'n=10',\\\n"
            "     'interp_cheb_n15.dat'   u 1:2 w l lw 1.5 dt 4 title 'n=15',\\\n"
            "     'interp_cheb_n20.dat'   u 1:2 w l lw 1.5       title 'n=20'\n");
    fclose(gp);

    gp = fopen("plot3.gp", "w");
    if (!gp) {
        perror("plot3.gp");
        exit(1);
    }
    fprintf(gp,
            "set terminal pngcairo size 1000,700\n"
            "set output 'wykres3_bledy.png'\n"
            "set logscale y\n"
            "set xlabel \"Stopien wielomianu n\"\n"
            "set ylabel \"max|p(x)-f(x)|\"\n"
            "set title \"Zbieznosc interpolacji\"\n"
            "set grid\n"
            "plot 'bledy.dat' u 1:2 w lp title 'rownoodlegle',\\\n"
            "     'bledy.dat' u 1:3 w lp title 'Czebyszew'\n");
    fclose(gp);
}

double uruchom_przypadek(const char* nazwa_pliku, bool czy_czebyszew, int n, double limit_obciecia)
{
    int i;
    int liczba_wezlow = czy_czebyszew ? n : n + 1;
    int stopien = liczba_wezlow - 1;

    vector<double> x(liczba_wezlow);
    vector<double> y(liczba_wezlow);
    vector<double> c(liczba_wezlow);

    if (czy_czebyszew) {
        wezly_czebyszewa(x, n);
    } else {
        wezly_rownoodlegle(x, n);
    }

    for (i = 0; i < liczba_wezlow; i++) {
        y[i] = funkcja(x[i]);
    }

    ilorazy_roznicowe(x, y, c, stopien);
    return zapisz_interpolacje(nazwa_pliku, x, c, stopien, limit_obciecia);
}

int main()
{
    int i;
    int stopnie[] = {5, 10, 15, 20};
    const char* pliki_rownoodlegle[] = {
        "interp_unif_n5.dat", "interp_unif_n10.dat", "interp_unif_n15.dat", "interp_unif_n20.dat"
    };
    const char* pliki_czebyszew[] = {
        "interp_cheb_n5.dat", "interp_cheb_n10.dat", "interp_cheb_n15.dat", "interp_cheb_n20.dat"
    };

    double max_abs_f = 0.0;
    zapisz_funkcje(max_abs_f);
    double limit_obciecia = 5.0 * max_abs_f;

    FILE* plik_bledow = fopen("bledy.dat", "w");
    if (!plik_bledow) {
        perror("bledy.dat");
        return 1;
    }

    printf("%6s %24s %24s\n", "n", "blad_rownoodlegle", "blad_czebyszew");
    printf("--------------------------------------------------------------------\n");

    for (i = 0; i < 4; i++) {
        int n = stopnie[i];
        double blad_rownoodlegle = uruchom_przypadek(pliki_rownoodlegle[i], false, n, limit_obciecia);
        double blad_czebyszew = uruchom_przypadek(pliki_czebyszew[i], true, n, limit_obciecia);

        fprintf(plik_bledow, "%d %.15e %.15e\n", n, blad_rownoodlegle, blad_czebyszew);
        printf("%6d %24.6e %24.6e\n", n, blad_rownoodlegle, blad_czebyszew);
    }

    fclose(plik_bledow);
    zapisz_skrypty_gnuplota();
    printf("\nZapisano pliki .dat i skrypty .gp.\n");
    return 0;
}
