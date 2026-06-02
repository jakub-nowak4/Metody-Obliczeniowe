#include <cstdio>
#include <cmath>
#include <vector>
using namespace std;

/* WSPÓŁCZYNNIKI RÓWNANIA RÓŻNICZKOWEGO */
double wsp_p(double x) {return 1.0;}
double wsp_q(double x) {return 2.0;}
double wsp_r(double x) {return -4.0;}
double wsp_s(double x) {return 0.5 * x * x * x;}

/* GRANICE PRZEDZIAŁU */
const double XA = 0.0;
const double XB = 1.0;

/* WARUNKI BRZEGOWE */

/* alfa y'(a) + beta y(a) + gamma = 0*/
const double ALFA = 0.0;
const double BETA = 1.0;
const double GAMMA = -2.0;

/* fi y'(b) + psi y(b) + teta = 0*/
const double FI = 0.0;
const double PSI = 1.0;
const double TETA = 2.0;


/* ROZWIAZANIE ANALITYCZNE */
double y_dokladne(double x)
{
    double sq5 = sqrt(5.0);
    double coth5 = cosh(sq5) / sinh(sq5);
    double w  = 2.0 * x * (6.0 + x * (3.0 + 2.0 * x));

    double A =   9.0
               - 95.0 * exp((-1.0 - sq5) * (-1.0 + x))
               + 55.0 * exp((-1.0 + sq5) *  x)
               + 95.0 * exp(( 1.0 + sq5) + (-1.0 + sq5) * x)
               - 55.0 * exp( 2.0 * sq5   - ( 1.0 + sq5) * x)
               + w
               - exp(2.0 * sq5) * (9.0 + w);

    return -A * (-1.0 + coth5) / 64.0;
}

/* ALGORYTM THOMASA */
void thomas(const vector<double>& dol, vector<double>& gl, vector<double>& gor, vector<double>& praw, vector<double>& y)
{
    int n = (int) gl.size();
    int i; 

    //Elimnacja w przód napisujemy gl i praw
    for(i = 1; i < n; i++)
    {
        double m =dol[i] / gl[i - 1];
        gl[i] -= m * gor[i - 1];
        praw[i] -= m * praw[i - 1];
    }

    //Podstawienie wstecz
    y[n - 1] = praw[n - 1] / gl[n - 1];
    for(i = n -2; i >= 0; i--)
    {
        y[i] = (praw[i] - gor[i] * y[i + 1]) / gl[i];
    }
}

/* METODA RÓŻNICOWA*/
void rozwiaz_MR(int n, vector<double>& y)
{
    double h = (XB - XA) / n;
    int N = n + 1;
    int i;

    vector<double> dol(N), gl(N), gor(N), praw(N);

    // Wiersz 0  -> lewy WB
    dol[0] = 0.0;
    gl[0] = BETA * h - ALFA;
    gor[0] =  ALFA;
    praw[0] = -GAMMA * h;


    /* Wiersze 1..n-1 -- rownanie roznicowe wewnetrzne*/
    for(i = 1; i <= n-1; i++)
    {
        double xi = XA + i * h;
        double pi = wsp_p(xi);
        double qi = wsp_q(xi);
        double ri = wsp_r(xi);
        double si = wsp_s(xi);

        dol[i] = pi - qi * h * 0.5;
        gl[i] = -2.0 * pi + ri * h * h;
        gor[i] = pi + qi * h * 0.5;
        praw[i] = -h * h * si;
    }

    // Wiersz n -> prawy WB
    dol[n] = -FI;
    gl[n] = FI + PSI * h;
    gor[n] = 0.0;
    praw[n] = -TETA * h;

    y.assign(N, 0.0);
    thomas(dol, gl, gor, praw, y);
}

/* METODA STRZAŁÓW -- pojedynczy strzał, zwraca rezyduum R(p) prawego WB */
double strzal(int n, double p, vector<double>& y)
{
    double h = (XB - XA) / n;
    int i;

    y.assign(n + 1, 0.0);

    //krok 2 -- y_0 i y_1 z układu warunków na lewym brzegu
    y[0] = (-GAMMA - ALFA * p) / BETA;
    y[1] = y[0] + h * p;

    // krok 3 -- marsz w przód po wzorze różnicowym
    for(i = 1; i <= n-1; i++)
    {
        double xi = XA + i * h;
        double pi = wsp_p(xi);
        double qi = wsp_q(xi);
        double ri = wsp_r(xi);
        double si = wsp_s(xi);

        double cD = pi - qi * h * 0.5;          
        double cG = -2.0 * pi + ri * h * h;     
        double cP = pi + qi * h * 0.5;          

        y[i+1] = (-h * h * si - cD * y[i-1] - cG * y[i]) / cP;
    }

    // krok 4 -- rezyduum prawego WB:  R(p) = fi*(y_n - y_{n-1})/h + psi*y_n + teta
    return FI * (y[n] - y[n-1]) / h + PSI * y[n] + TETA;
}

/* METODA STRZAŁÓW */
void rozwiaz_strzalami(int n, vector<double>& y)
{
    const double tolx     = 1.0e-15;
    const double tolf     = 1.0e-15;
    const int    max_iter = 50;

    vector<double> y_temp(n + 1);

    /* dwa starty: p_0 = 0.0,  p_1 = 1.0 */
    double xn  = 0.0;
    double xn1 = 1.0;
    double xn2 = xn1;
    double fxn2 = 0.0;
    double estymator_bledu = 0.0;

    int i = 0;
    while (i < max_iter) {

        double f_xn1 = strzal(n, xn1, y_temp);
        double f_xn  = strzal(n, xn,  y_temp);
        double dF    = f_xn1 - f_xn;

        /* gdy sieczna jest pozioma */
        if (fabs(dF) < 1.0e-30) break;

        xn2  = xn1 - f_xn1 * (xn1 - xn) / dF;
        fxn2 = strzal(n, xn2, y_temp);

        estymator_bledu = xn2 - xn1;

        // Sprawdzenie K2 oraz K3 -- oba musza byc spelnione jednoczesnie
        if (fabs(estymator_bledu) <= tolx && fabs(fxn2) <= tolf) break;

        xn  = xn1;
        xn1 = xn2;
        i++;
    }

    /* ostateczny strzal z dopasowanym p */
    strzal(n, xn2, y);
}

/* MAKSYMALNY BŁĄD BEZWZGLĘDNY */
double blad_maks(const vector<double>& y, int n)
{
    double h = (XB - XA) / n;
    double blad = 0.0;
    int i = 0;

    for(i = 0; i <= n; i++)
    {
        double e = fabs(y[i] - y_dokladne(XA + i * h));
        if(e > blad) blad = e;
    }

    return blad;
}

int main()
{
    int    n, i, k;
    double h;
    FILE  *plik;


    /* (a) porównanie rozwiązań dla n = 40 (gladkie pokrycie, blad ~ 10^-4) */
    n = 40;
    h = (XB - XA) / n;
    {
        vector<double> y_MR;
        vector<double> y_strz;
        rozwiaz_MR(n, y_MR);
        rozwiaz_strzalami(n, y_strz);
        plik = fopen("rozwiazanie.dat", "w");
        fprintf(plik, "# x  y_dokladne  y_MR  y_strzaly\n");
        for (i = 0; i <= n; i++) {
            double x = XA + i * h;
            fprintf(plik, "%.15e  %.15e  %.15e  %.15e\n",
                    x, y_dokladne(x), y_MR[i], y_strz[i]);
        }
        fclose(plik);
        printf("Zapisano: rozwiazanie.dat (n = %d)\n", n);
    }

    /* (a') porównanie rozwiązań dla n = 4 (h = 0.25, blad MR rzedu O(h^2) ~ 6e-2
     *      -- juz widocznie odbiega od krzywej analitycznej) */
    n = 4;
    h = (XB - XA) / n;
    {
        vector<double> y_MR;
        vector<double> y_strz;
        rozwiaz_MR(n, y_MR);
        rozwiaz_strzalami(n, y_strz);
        plik = fopen("rozwiazanie_n4.dat", "w");
        fprintf(plik, "# x  y_dokladne  y_MR  y_strzaly\n");
        for (i = 0; i <= n; i++) {
            double x = XA + i * h;
            fprintf(plik, "%.15e  %.15e  %.15e  %.15e\n",
                    x, y_dokladne(x), y_MR[i], y_strz[i]);
        }
        fclose(plik);
        printf("Zapisano: rozwiazanie_n4.dat (n = %d)\n", n);
    }
    /* (b) zbieżność: log|błąd| vs log h
     *     dodatkowo: doświadczalny rząd p = log(e_prev/e) / log(h_prev/h) = log2(e_prev/e),  bo h_prev/h = 2
     */
    {
        double e1_prev = 0.0, e2_prev = 0.0;
        plik = fopen("bledy.dat", "w");
        fprintf(plik, "# h  blad_MR  blad_strzaly  p_MR  p_strzaly\n");
        printf("\n");
        printf("%8s %16s %18s %18s %10s %10s\n",
               "n", "h", "blad_MR", "blad_strz", "p_MR", "p_strz");
        printf("--------------------------------------------------------------------------------\n");
        for (k = 1; k <= 18; k++) {
            n = 1 << k;            /* n = 2, 4, 8, ..., 2^18 */
            h = (XB - XA) / n;
            vector<double> y1;
            vector<double> y2;
            rozwiaz_MR(n, y1);
            rozwiaz_strzalami(n, y2);
            double e1 = blad_maks(y1, n);
            double e2 = blad_maks(y2, n);

            /* eksperymentalny rząd dokładności (od drugiej iteracji) */
            double p1 = (k > 1) ? log(e1_prev / e1) / log(2.0) : 0.0;
            double p2 = (k > 1) ? log(e2_prev / e2) / log(2.0) : 0.0;

            fprintf(plik, "%.15e  %.15e  %.15e  %.6f  %.6f\n", h, e1, e2, p1, p2);
            if (k == 1) {
                printf("%8d %16.6e %18.6e %18.6e %10s %10s\n",
                       n, h, e1, e2, "---", "---");
            } else {
                printf("%8d %16.6e %18.6e %18.6e %10.4f %10.4f\n",
                       n, h, e1, e2, p1, p2);
            }
            e1_prev = e1;
            e2_prev = e2;
        }
        fclose(plik);
        printf("\nZapisano: bledy.dat\n");
        printf("\nUWAGA: p ~ 2 oznacza zgodnosc z teoretycznym rzedem O(h^2).\n");
        printf("       Spadek p (lub wartosci ujemne) -> dominacja bledow maszynowych.\n");
    }
    return 0;
}