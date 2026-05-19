#include <iostream>
#include <cstdio>
#include <cmath>
#include <vector>
using namespace std;

/* WSPÓŁCZYNNIKI RÓWNANIA RÓŻNICZKOWEGO */
double wsp_A(double t) {return (100.0 * t + 10.0) / (t + 1.0);}
double f_prawa(double t, double y) {return -wsp_A(t) * (y - 1.0);}

/* GRANICE PRZEDZIAŁU */
const double TA = 0.0;
const double TB = 1.0;

/* WARUNEK POCZĄTKOWY */

/* y(TA) = ALFA */
const double ALFA = 2.0;

/* prog stabilnosci BME: dt <= 2 / A(1) = 2/55 */
const double DT_STAB = 2.0 / 55.0;


/* ROZWIAZANIE ANALITYCZNE */
double y_dokladne(double t)
{
    return 1.0 + pow(1.0 + t, 90.0) * exp(-100.0 * t);
}

/* BEZPOSREDNIA METODA EULERA (BME) */
void rozwiaz_BME(int n, vector<double>& y)
{
    double h = (TB - TA) / n;
    int i;

    y.assign(n + 1, 0.0);
    y[0] = ALFA;

    for(i = 0; i < n; i++)
    {
        double ti = TA + i * h;
        y[i + 1] = y[i] + h * f_prawa(ti, y[i]);
    }
}

/* POSREDNIA METODA EULERA (PME) */
void rozwiaz_PME(int n, vector<double>& y)
{
    double h = (TB - TA) / n;
    int i;

    y.assign(n + 1, 0.0);
    y[0] = ALFA;

    for(i = 0; i < n; i++)
    {
        double ti1 = TA + (i + 1) * h;
        double ai  = wsp_A(ti1);
        y[i + 1] = (y[i] + ai * h) / (1.0 + ai * h);
    }
}

/* POSREDNIA METODA TRAPEZOW (PMT)  rozwiazujemy algebraicznie otrzymane rownanie liniowe*/
void rozwiaz_PMT(int n, vector<double>& y)
{
    double h = (TB - TA) / n;
    int i;

    y.assign(n + 1, 0.0);
    y[0] = ALFA;

    for(i = 0; i < n; i++)
    {
        double ti  = TA + i * h;
        double ti1 = TA + (i + 1) * h;
        double a0  = wsp_A(ti);
        double a1  = wsp_A(ti1);

        y[i + 1] = (y[i] * (1.0 - 0.5 * h * a0) + 0.5 * h * (a0 + a1)) / (1.0 + 0.5 * h * a1);
    }
}

/* MAKSYMALNY BŁĄD BEZWZGLĘDNY */
double blad_maks(const vector<double>& y, int n)
{
    double h = (TB - TA) / n;
    double blad = 0.0;
    int i = 0;

    for(i = 0; i <= n; i++)
    {
        double e = fabs(y[i] - y_dokladne(TA + i * h));
        if(e > blad) blad = e;
    }

    return blad;
}

int main()
{
    int    n, i, k;
    double h;
    FILE  *plik;


    /* (a) BME -- trzy rezimy stabilnosci
     *     n = 100 -> dt = 0.010  stabilnie
     *     n =  27 -> dt ~ 0.037   prog
     *     n =  12 -> dt ~ 0.083   niestabilnie
     */
    {
        vector<double> y_stab;
        vector<double> y_gran;
        vector<double> y_nie;

        rozwiaz_BME(100, y_stab);
        rozwiaz_BME( 27, y_gran);
        rozwiaz_BME( 12, y_nie);

        plik = fopen("bme_stabilne.dat", "w");
        fprintf(plik, "# t  y_dokladne  y_BME\n");
        h = (TB - TA) / 100;
        for (i = 0; i <= 100; i++) {
            double t = TA + i * h;
            fprintf(plik, "%.15e  %.15e  %.15e\n", t, y_dokladne(t), y_stab[i]);
        }
        fclose(plik);
        printf("Zapisano: bme_stabilne.dat (n = %d)\n", 100);

        plik = fopen("bme_graniczne.dat", "w");
        fprintf(plik, "# t  y_dokladne  y_BME\n");
        h = (TB - TA) / 27;
        for (i = 0; i <= 27; i++) {
            double t = TA + i * h;
            fprintf(plik, "%.15e  %.15e  %.15e\n", t, y_dokladne(t), y_gran[i]);
        }
        fclose(plik);
        printf("Zapisano: bme_graniczne.dat (n = %d)\n", 27);

        plik = fopen("bme_niestabilne.dat", "w");
        fprintf(plik, "# t  y_dokladne  y_BME\n");
        h = (TB - TA) / 12;
        for (i = 0; i <= 12; i++) {
            double t = TA + i * h;
            fprintf(plik, "%.15e  %.15e  %.15e\n", t, y_dokladne(t), y_nie[i]);
        }
        fclose(plik);
        printf("Zapisano: bme_niestabilne.dat (n = %d)\n", 12);
    }

    /* (b) PME */
    n = 100;
    h = (TB - TA) / n;
    {
        vector<double> y_PME;
        rozwiaz_PME(n, y_PME);
        plik = fopen("pme.dat", "w");
        fprintf(plik, "# t  y_dokladne  y_PME\n");
        for (i = 0; i <= n; i++) {
            double t = TA + i * h;
            fprintf(plik, "%.15e  %.15e  %.15e\n", t, y_dokladne(t), y_PME[i]);
        }
        fclose(plik);
        printf("Zapisano: pme.dat (n = %d)\n", n);
    }

    /* (c) PMT */
    n = 100;
    h = (TB - TA) / n;
    {
        vector<double> y_PMT;
        rozwiaz_PMT(n, y_PMT);
        plik = fopen("pmt.dat", "w");
        fprintf(plik, "# t  y_dokladne  y_PMT\n");
        for (i = 0; i <= n; i++) {
            double t = TA + i * h;
            fprintf(plik, "%.15e  %.15e  %.15e\n", t, y_dokladne(t), y_PMT[i]);
        }
        fclose(plik);
        printf("Zapisano: pmt.dat (n = %d)\n", n);
    }

    /* (d) zbieżność: log|błąd| vs log dt
     *     dodatkowo: doświadczalny rząd p = log(e_prev/e) / log(dt_prev/dt) = log2(e_prev/e),  bo dt_prev/dt = 2
     *     BME tylko w obszarze stabilnosci (dt <= DT_STAB)
     */
    {
        double eB_prev = 0.0, eP_prev = 0.0, eT_prev = 0.0;
        plik = fopen("bledy.dat", "w");
        fprintf(plik, "# dt  blad_BME  blad_PME  blad_PMT  p_BME  p_PME  p_PMT\n");
        printf("\n");
        printf("%8s %16s %18s %18s %18s %10s %10s %10s\n",
               "n", "dt", "blad_BME", "blad_PME", "blad_PMT",
               "p_BME", "p_PME", "p_PMT");
        printf("--------------------------------------------------------------------------------\n");
        for (k = 1; k <= 22; k++) {
            n = 1 << k;            /* n = 2, 4, 8, ..., 2^22 */
            h = (TB - TA) / n;

            vector<double> yB;
            vector<double> yP;
            vector<double> yT;
            double eB = 0.0, eP, eT;
            int bme_ok = 0;

            if (h <= DT_STAB) {
                rozwiaz_BME(n, yB);
                eB = blad_maks(yB, n);
                if (isfinite(eB)) bme_ok = 1;
            }
            rozwiaz_PME(n, yP);
            rozwiaz_PMT(n, yT);
            eP = blad_maks(yP, n);
            eT = blad_maks(yT, n);

            /* eksperymentalny rząd dokładności (od drugiej iteracji) */
            double pB = (k > 1 && bme_ok && eB_prev > 0.0) ? log(eB_prev / eB) / log(2.0) : 0.0;
            double pP = (k > 1) ? log(eP_prev / eP) / log(2.0) : 0.0;
            double pT = (k > 1) ? log(eT_prev / eT) / log(2.0) : 0.0;

            fprintf(plik, "%.15e  %.15e  %.15e  %.15e  %.6f  %.6f  %.6f\n",
                    h, (bme_ok ? eB : NAN), eP, eT, pB, pP, pT);

            if (k == 1) {
                if (bme_ok) {
                    printf("%8d %16.6e %18.6e %18.6e %18.6e %10s %10s %10s\n",
                           n, h, eB, eP, eT, "---", "---", "---");
                } else {
                    printf("%8d %16.6e %18s %18.6e %18.6e %10s %10s %10s\n",
                           n, h, "---", eP, eT, "---", "---", "---");
                }
            } else {
                if (bme_ok) {
                    printf("%8d %16.6e %18.6e %18.6e %18.6e", n, h, eB, eP, eT);
                    if (eB_prev > 0.0) printf(" %10.4f", pB); else printf(" %10s", "---");
                    printf(" %10.4f %10.4f\n", pP, pT);
                } else {
                    printf("%8d %16.6e %18s %18.6e %18.6e %10s %10.4f %10.4f\n",
                           n, h, "---", eP, eT, "---", pP, pT);
                }
            }

            if (bme_ok) eB_prev = eB;
            eP_prev = eP;
            eT_prev = eT;
        }
        fclose(plik);
        printf("\nZapisano: bledy.dat\n");
        printf("\nUWAGA: p ~ 1 oznacza zgodnosc z teoretycznym rzedem O(dt)  (BME, PME).\n");
        printf("       p ~ 2 oznacza zgodnosc z teoretycznym rzedem O(dt^2) (PMT).\n");
        printf("       Spadek p (lub wartosci ujemne) -> dominacja bledow maszynowych.\n");
    }
    return 0;
}
