#include <cstdio>
#include <cmath>

using namespace std;

const double PI = acos(-1.0);
const double EPS_STOP = 1.0e-14;
const int ILE_N = 12;
const int N_WARTOSCI[ILE_N] = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096};

/* CALKOWANA FUNKCJA */
double f(double y)
{
    return 2.0 / sqrt(PI) * exp(-y * y);
}

/* ZLOZONA KWADRATURA PROSTOKATOW -- LEWA */
double lewa_prostokat(double x_gorny, int n)
{
    double h = x_gorny / n;
    double suma = 0.0;
    int i;

    for(i = 0; i <= n - 1; i++)
    {
        double y = i * h;
        suma += f(y);
    }

    return h * suma;
}

/* ZLOZONA KWADRATURA PROSTOKATOW -- PRAWA */
double prawa_prostokat(double x_gorny, int n)
{
    double h = x_gorny / n;
    double suma = 0.0;
    int i;

    for(i = 1; i <= n; i++)
    {
        double y = i * h;
        suma += f(y);
    }

    return h * suma;
}

/* ZLOZONA KWADRATURA PROSTOKATOW -- SRODEK */
double srodek_prostokat(double x_gorny, int n)
{
    double h = x_gorny / n;
    double suma = 0.0;
    int i;

    for(i = 0; i <= n - 1; i++)
    {
        double y = i * h + 0.5 * h;
        suma += f(y);
    }

    return h * suma;
}

/* ZLOZONA KWADRATURA TRAPEZOW */
double trapezy(double x_gorny, int n)
{
    double h = x_gorny / n;
    double suma = 0.5 * f(0.0) + 0.5 * f(x_gorny);
    int i;

    for(i = 1; i <= n - 1; i++)
    {
        double y = i * h;
        suma += f(y);
    }

    return h * suma;
}

/* ZLOZONA KWADRATURA PARABOL (SIMPSONA) */
double simpson(double x_gorny, int n)
{
    if(n % 2 == 1) n++;

    double h = x_gorny / n;
    double suma = f(0.0) + f(x_gorny);
    int i;

    for(i = 1; i <= n - 1; i++)
    {
        double y = i * h;
        if(i % 2 == 1) suma += 4.0 * f(y);
        else           suma += 2.0 * f(y);
    }

    return h * suma / 3.0;
}

double rzad_zbieznosci(double blad_poprzedni, double blad)
{
    if(blad_poprzedni <= 0.0 || blad <= 0.0) return 0.0;
    return log(blad_poprzedni / blad) / log(2.0);
}

void drukuj_naglowek(double x, const char *nazwa_metody, double dokladna)
{
    printf("\n=== erf(%.1f) -- %s ===\n", x, nazwa_metody);
    printf("Wartosc dokladna: %.15f\n", dokladna);
    printf("%8s  %19s  %13s  %7s\n", "n", "wynik", "blad abs", "rzad");
    printf("--------  -------------------  -------------  -------\n");
}

void drukuj_wiersz(int n, double wynik, double blad, double rzad, bool pierwszy)
{
    if(pierwszy)
    {
        printf("%8d  %19.15f  %13.6e  %7s\n", n, wynik, blad, "---");
    }
    else
    {
        printf("%8d  %19.15f  %13.6e  %7.3f\n", n, wynik, blad, rzad);
    }
}

void tabela_metody(double x, const char *nazwa_metody, double (*metoda)(double, int))
{
    double dokladna = erf(x);
    double blad_poprzedni = 0.0;
    int k;

    drukuj_naglowek(x, nazwa_metody, dokladna);

    for(k = 0; k < ILE_N; k++)
    {
        int n = N_WARTOSCI[k];
        double wynik = metoda(x, n);
        double blad = fabs(wynik - dokladna);
        double p = rzad_zbieznosci(blad_poprzedni, blad);

        drukuj_wiersz(n, wynik, blad, p, k == 0);

        if(blad < EPS_STOP) break;
        blad_poprzedni = blad;
    }
}

void zapisz_gnuplot(double x, const char *plik_danych, const char *plik_gp, const char *plik_png)
{
    FILE *gp = fopen(plik_gp, "w");
    if(gp == NULL)
    {
        printf("Nie mozna utworzyc pliku: %s\n", plik_gp);
        return;
    }

    fprintf(gp, "set terminal pngcairo size 900,650\n");
    fprintf(gp, "set output '%s'\n", plik_png);
    fprintf(gp, "set logscale xy\n");
    fprintf(gp, "set grid\n");
    fprintf(gp, "set xlabel \"n (liczba podprzedzialow)\"\n");
    fprintf(gp, "set ylabel \"|I_num - erf(x)|\"\n");
    fprintf(gp, "set title \"Zbieznosc kwadratury: erf(%.1f)\"\n", x);
    fprintf(gp, "set key top right\n");
    fprintf(gp, "C1 = 0.07; C2 = 0.003; C4 = 0.001\n");
    fprintf(gp, "plot '%s' u 1:2 w lp lw 2 pt 7  title 'prosto. lewa  O(h^1)',  \\\n", plik_danych);
    fprintf(gp, "     '%s' u 1:3 w lp lw 2 pt 9  title 'prosto. prawa O(h^1)',  \\\n", plik_danych);
    fprintf(gp, "     '%s' u 1:4 w lp lw 2 pt 5  title 'prosto. srodek O(h^2)', \\\n", plik_danych);
    fprintf(gp, "     '%s' u 1:5 w lp lw 2 pt 13 title 'trapezy O(h^2)',        \\\n", plik_danych);
    fprintf(gp, "     '%s' u 1:6 w lp lw 2 pt 11 title 'Simpson O(h^4)',        \\\n", plik_danych);
    fprintf(gp, "     C1/x      w l lw 1 dt 2 lc 'gray' title 'ref O(1/n)',   \\\n");
    fprintf(gp, "     C2/x**2   w l lw 1 dt 3 lc 'gray' title 'ref O(1/n^2)', \\\n");
    fprintf(gp, "     C4/x**4   w l lw 1 dt 4 lc 'gray' title 'ref O(1/n^4)'\n");

    fclose(gp);
}

void tabela(double x, const char *nazwa_pliku)
{
    FILE *plik = fopen(nazwa_pliku, "w");
    if(plik == NULL)
    {
        printf("Nie mozna utworzyc pliku: %s\n", nazwa_pliku);
        return;
    }

    double dokladna = erf(x);
    int k;

    fprintf(plik, "# x = %.1f\n", x);
    fprintf(plik, "# n  blad_lewa  blad_prawa  blad_srodek  blad_trapezy  blad_simpson\n");

    tabela_metody(x, "Kwadratura prostokatow lewa", lewa_prostokat);
    tabela_metody(x, "Kwadratura prostokatow prawa", prawa_prostokat);
    tabela_metody(x, "Kwadratura prostokatow srodek", srodek_prostokat);
    tabela_metody(x, "Kwadratura trapezow", trapezy);
    tabela_metody(x, "Kwadratura parabol (Simpsona)", simpson);

    for(k = 0; k < ILE_N; k++)
    {
        int n = N_WARTOSCI[k];
        double blad_lewa = fabs(lewa_prostokat(x, n) - dokladna);
        double blad_prawa = fabs(prawa_prostokat(x, n) - dokladna);
        double blad_srodek = fabs(srodek_prostokat(x, n) - dokladna);
        double blad_trapezy = fabs(trapezy(x, n) - dokladna);
        double blad_simpson = fabs(simpson(x, n) - dokladna);

        fprintf(plik, "%8d  %.15e  %.15e  %.15e  %.15e  %.15e\n",
                n, blad_lewa, blad_prawa, blad_srodek, blad_trapezy, blad_simpson);
    }

    fclose(plik);
    printf("\nZapisano: %s\n", nazwa_pliku);
}

int main()
{
    tabela(1.0, "bledy.dat");
    tabela(2.0, "bledy_x2.dat");
    tabela(3.0, "bledy_x3.dat");

    zapisz_gnuplot(1.0, "bledy.dat",    "plot_x1.gp", "wykres_erf_x1.png");
    zapisz_gnuplot(2.0, "bledy_x2.dat", "plot_x2.gp", "wykres_erf_x2.png");
    zapisz_gnuplot(3.0, "bledy_x3.dat", "plot_x3.gp", "wykres_erf_x3.png");

    printf("\nZapisano skrypty: plot_x1.gp, plot_x2.gp, plot_x3.gp\n");
    return 0;
}
