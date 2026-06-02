#include <cstdio>
#include <cmath>
#include <vector>
#include <ctime>
using namespace std;

/* PARAMETRY FIZYCZNE */
const double D     = 1.0;
const double TAU   = 0.1;
const double TMAX  = 2.0;
const double A_DOM = 12.0;     /* a >= 6*sqrt(D*(TAU+TMAX)) */

/* WSPOLCZYNNIKI SCHEMATOW */
const double LAM_KMB = 0.40;   /* stabilnosc KMB: lambda <= 0.5 */
const double LAM_ML  = 1.00;   /* Laasonen: bezwarunkowo stabilny */

/* KRYTERIA STOPU GAUSSA-SEIDELA */
const double TOL_GS_DX_BAZA = 1.0e-12;
const double TOL_GS_R_BAZA  = 1.0e-11;
const int    MAX_GS_IT      = 5000;

/* ROZWIAZANIE ANALITYCZNE */
double U_dokladne(double x, double t)
{
    return exp(-x * x / (4.0 * D * (TAU + t)))
         / (2.0 * sqrt(M_PI * D * (TAU + t)));
}

/* PARAMETRY SIATKI */
struct Siatka
{
    int    N, M, Nt;
    double h, dt, lam;
};

Siatka utworz_siatke(double h_zad, double lam_zad)
{
    Siatka s;

    s.N = (int) round(2.0 * A_DOM / h_zad);
    if(s.N < 4) s.N = 4;

    s.h  = 2.0 * A_DOM / s.N;
    s.dt = lam_zad * s.h * s.h / D;
    s.Nt = (int) ceil(TMAX / s.dt);
    if(s.Nt < 1) s.Nt = 1;

    s.dt  = TMAX / s.Nt;
    s.lam = D * s.dt / (s.h * s.h);
    s.M   = s.N - 1;

    return s;
}

/* SIATKA PRZESTRZENNA */
void siatka_x(const Siatka& s, vector<double>& x)
{
    x.resize(s.N + 1);
    for(int i = 0; i <= s.N; i++) x[i] = -A_DOM + i * s.h;
}

/* WARUNEK POCZATKOWY */
void warunki_poczatkowe(const Siatka& s, const vector<double>& x, vector<double>& U)
{
    U.assign(s.N + 1, 0.0);
    for(int i = 0; i <= s.N; i++) U[i] = U_dokladne(x[i], 0.0);
    U[0] = U[s.N] = 0.0;
}

/* MAKSYMALNY BLAD BEZWZGLEDNY */
double blad_maks(const Siatka& s, const vector<double>& x, const vector<double>& U, double t)
{
    double blad = 0.0;

    for(int i = 1; i < s.N; i++)
    {
        double e = fabs(U[i] - U_dokladne(x[i], t));
        if(e > blad) blad = e;
    }

    return blad;
}

/* ZAPIS PROFILU U(x) */
void zapisz_profil(FILE *f, const Siatka& s, const vector<double>& x, const vector<double>& U, double t)
{
    for(int i = 1; i < s.N; i++)
        fprintf(f, "%.15e\t%.15e\t%.15e\n", x[i], U[i], U_dokladne(x[i], t));

    fprintf(f, "\n\n");
}

/* ALGORYTM THOMASA -- FAKTORYZACJA */
void thomas_fak(const vector<double>& dol, const vector<double>& gl, const vector<double>& gor,
                vector<double>& gl_fak, vector<double>& mu)
{
    int M = (int) gl.size();

    gl_fak = gl;
    for(int j = 1; j < M; j++)
    {
        mu[j] = dol[j] / gl_fak[j - 1];
        gl_fak[j] -= mu[j] * gor[j - 1];
    }
}

/* ALGORYTM THOMASA -- ROZWIAZANIE */
void thomas_rozw(const vector<double>& gor, const vector<double>& gl_fak, const vector<double>& mu,
                 vector<double>& praw, vector<double>& y)
{
    int M = (int) gl_fak.size();

    for(int j = 1; j < M; j++)
        praw[j] -= mu[j] * praw[j - 1];

    y[M - 1] = praw[M - 1] / gl_fak[M - 1];
    for(int j = M - 2; j >= 0; j--)
        y[j] = (praw[j] - gor[j] * y[j + 1]) / gl_fak[j];
}

// Metoda Gaussa-Seidela dla ukladu Laasonen
int gauss_seidel(const vector<double>& R, double lam, vector<double>& V, double h)
{
    int M = (int) V.size();
    double den = 1.0 + 2.0 * lam;

    vector<double> V_poprz(M);
    vector<double> tol_dx(M), tol_r(M);

    double tol_dx_b = fmax(TOL_GS_DX_BAZA, 1.0e-3 * h * h);
    double tol_r_b  = fmax(TOL_GS_R_BAZA,  1.0e-3 * h * h);

    for(int j = 0; j < M; j++)
    {
        tol_dx[j] = tol_dx_b;
        tol_r[j]  = tol_r_b;
    }

    for(int it = 0; it < MAX_GS_IT; it++)
    {
        for(int j = 0; j < M; j++) V_poprz[j] = V[j];

        for(int j = 0; j < M; j++)
        {
            double vl = (j > 0)     ? V[j - 1] : 0.0;
            double vr = (j < M - 1) ? V[j + 1] : 0.0;
            V[j] = (lam * (vl + vr) + R[j]) / den;
        }

        bool wszystkie_tolerancje = true;
        for(int j = 0; j < M; j++)
        {
            double vl = (j > 0)     ? V[j - 1] : 0.0;
            double vr = (j < M - 1) ? V[j + 1] : 0.0;
            double residuum = R[j] - (den * V[j] - lam * vl - lam * vr);

            wszystkie_tolerancje =
                wszystkie_tolerancje &&
                (fabs(V[j] - V_poprz[j]) < tol_dx[j]) &&
                (fabs(residuum)          < tol_r[j]);
        }

        if(wszystkie_tolerancje) return it + 1;
    }

    return -1;
}

/* KMB -- METODA BEZPOSREDNIA */
double rozwiaz_KMB(double h_zad, FILE *f_hist, FILE *f_snap,
                   const vector<double> *snap_t, double *czas_s)
{
    Siatka s = utworz_siatke(h_zad, LAM_KMB);

    vector<double> x;
    vector<double> U;
    vector<double> Un(s.N + 1, 0.0);

    siatka_x(s, x);
    warunki_poczatkowe(s, x, U);

    int si = 0;
    if(f_snap && snap_t && !snap_t->empty() && (*snap_t)[0] <= 1.0e-12)
    {
        zapisz_profil(f_snap, s, x, U, 0.0);
        si = 1;
    }

    if(f_hist) fprintf(f_hist, "0.0\t0.0\n");

    clock_t t0 = clock();
    for(int k = 0; k < s.Nt; k++)
    {
        double t = (k + 1) * s.dt;

        for(int i = 1; i < s.N; i++)
            Un[i] = s.lam * U[i - 1] + (1.0 - 2.0 * s.lam) * U[i] + s.lam * U[i + 1];

        Un[0] = Un[s.N] = 0.0;
        U.swap(Un);

        if(f_hist)
            fprintf(f_hist, "%.15e\t%.15e\n", t, blad_maks(s, x, U, t));

        if(f_snap && snap_t && si < (int) snap_t->size() && t >= (*snap_t)[si] - 0.5 * s.dt)
        {
            zapisz_profil(f_snap, s, x, U, t);
            si++;
        }
    }

    clock_t t1 = clock();
    if(czas_s) *czas_s = (double) (t1 - t0) / CLOCKS_PER_SEC;

    return blad_maks(s, x, U, TMAX);
}

/* LAASONEN + ALGORYTM THOMASA */
double rozwiaz_ML_thomas(double h_zad, FILE *f_hist, FILE *f_snap,
                         const vector<double> *snap_t, double *czas_s)
{
    Siatka s = utworz_siatke(h_zad, LAM_ML);

    vector<double> x;
    vector<double> U;

    siatka_x(s, x);
    warunki_poczatkowe(s, x, U);

    vector<double> dol(s.M), gl(s.M), gor(s.M);
    for(int j = 0; j < s.M; j++)
    {
        dol[j] = (j > 0)       ? s.lam : 0.0;
        gl[j]  = -(1.0 + 2.0 * s.lam);
        gor[j] = (j < s.M - 1) ? s.lam : 0.0;
    }

    vector<double> gl_fak(s.M), mu(s.M);
    thomas_fak(dol, gl, gor, gl_fak, mu);

    vector<double> praw(s.M), V(s.M);

    int si = 0;
    if(f_snap && snap_t && !snap_t->empty() && (*snap_t)[0] <= 1.0e-12)
    {
        zapisz_profil(f_snap, s, x, U, 0.0);
        si = 1;
    }

    if(f_hist) fprintf(f_hist, "0.0\t0.0\n");

    clock_t t0 = clock();
    for(int k = 0; k < s.Nt; k++)
    {
        double t = (k + 1) * s.dt;

        for(int j = 0; j < s.M; j++) praw[j] = -U[j + 1];
        thomas_rozw(gor, gl_fak, mu, praw, V);

        for(int j = 0; j < s.M; j++) U[j + 1] = V[j];
        U[0] = U[s.N] = 0.0;

        if(f_hist)
            fprintf(f_hist, "%.15e\t%.15e\n", t, blad_maks(s, x, U, t));

        if(f_snap && snap_t && si < (int) snap_t->size() && t >= (*snap_t)[si] - 0.5 * s.dt)
        {
            zapisz_profil(f_snap, s, x, U, t);
            si++;
        }
    }

    clock_t t1 = clock();
    if(czas_s) *czas_s = (double) (t1 - t0) / CLOCKS_PER_SEC;

    return blad_maks(s, x, U, TMAX);
}

/* LAASONEN + GAUSS-SEIDEL */
double rozwiaz_ML_gs(double h_zad, FILE *f_hist, FILE *f_snap,
                     const vector<double> *snap_t, double *czas_s)
{
    Siatka s = utworz_siatke(h_zad, LAM_ML);

    vector<double> x;
    vector<double> U;
    vector<double> praw(s.M), V(s.M, 0.0);

    siatka_x(s, x);
    warunki_poczatkowe(s, x, U);

    int si = 0;
    if(f_snap && snap_t && !snap_t->empty() && (*snap_t)[0] <= 1.0e-12)
    {
        zapisz_profil(f_snap, s, x, U, 0.0);
        si = 1;
    }

    if(f_hist) fprintf(f_hist, "0.0\t0.0\n");

    clock_t t0 = clock();
    for(int k = 0; k < s.Nt; k++)
    {
        double t = (k + 1) * s.dt;

        for(int j = 0; j < s.M; j++) praw[j] = U[j + 1];

        int iter_gs = gauss_seidel(praw, s.lam, V, s.h);
        if(iter_gs < 0)
            fprintf(stderr, "OSTRZEZENIE: GS nie spelnil wszystkich tolerancji w kroku czasu %d\n", k + 1);

        for(int j = 0; j < s.M; j++) U[j + 1] = V[j];
        U[0] = U[s.N] = 0.0;

        if(f_hist)
            fprintf(f_hist, "%.15e\t%.15e\n", t, blad_maks(s, x, U, t));

        if(f_snap && snap_t && si < (int) snap_t->size() && t >= (*snap_t)[si] - 0.5 * s.dt)
        {
            zapisz_profil(f_snap, s, x, U, t);
            si++;
        }
    }

    clock_t t1 = clock();
    if(czas_s) *czas_s = (double) (t1 - t0) / CLOCKS_PER_SEC;

    return blad_maks(s, x, U, TMAX);
}

int main()
{
    FILE *plik;

    const vector<double> snap_t = {0.0, 0.05, 0.2, 0.5, 1.0, 2.0};
    const double h_vals[] = {0.50, 0.25, 0.10, 0.05, 0.025, 0.010, 0.005, 0.0048};
    const int n_h = (int) (sizeof(h_vals) / sizeof(h_vals[0]));

    printf("Cwiczenie 11-D: rownanie dyfuzji 1D\n");
    printf("D = %.1f, tau = %.1f, Tmax = %.1f, A = %.1f\n", D, TAU, TMAX, A_DOM);
    printf("KMB: lambda = %.2f    ML: lambda = %.1f\n\n", LAM_KMB, LAM_ML);

    /* (1) ZBIEZNOSC: max|blad|(Tmax) vs h */
    {
        double e_kmb_prev = 0.0;
        double e_ml_prev  = 0.0;
        double e_gs_prev  = 0.0;
        double h_prev     = 0.0;

        plik = fopen("konwergencja.dat", "w");
        fprintf(plik, "# h  blad_KMB  t_KMB[s]  blad_ML  t_ML[s]  blad_ML_GS  t_ML_GS[s]\n");

        printf("%8s %16s %16s %16s %10s %10s %10s\n",
               "h", "blad_KMB", "blad_ML", "blad_ML_GS", "p_KMB", "p_ML", "p_GS");
        printf("--------------------------------------------------------------------------------\n");

        for(int k = 0; k < n_h; k++)
        {
            double h = h_vals[k];
            double t_kmb, t_ml, t_gs;

            double e_kmb = rozwiaz_KMB(h, NULL, NULL, NULL, &t_kmb);
            double e_ml  = rozwiaz_ML_thomas(h, NULL, NULL, NULL, &t_ml);
            double e_gs  = rozwiaz_ML_gs(h, NULL, NULL, NULL, &t_gs);

            double p_kmb = (e_kmb_prev > 0.0) ? log(e_kmb_prev / e_kmb) / log(h_prev / h) : 0.0;
            double p_ml  = (e_ml_prev  > 0.0) ? log(e_ml_prev  / e_ml ) / log(h_prev / h) : 0.0;
            double p_gs  = (e_gs_prev  > 0.0) ? log(e_gs_prev  / e_gs ) / log(h_prev / h) : 0.0;

            fprintf(plik, "%.15e  %.15e  %.15e  %.15e  %.15e  %.15e  %.15e\n",
                    h, e_kmb, t_kmb, e_ml, t_ml, e_gs, t_gs);

            if(k == 0) {
                printf("%8.4f %16.6e %16.6e %16.6e %10s %10s %10s\n",
                       h, e_kmb, e_ml, e_gs, "---", "---", "---");
            } else {
                printf("%8.4f %16.6e %16.6e %16.6e %10.4f %10.4f %10.4f\n",
                       h, e_kmb, e_ml, e_gs, p_kmb, p_ml, p_gs);
            }

            e_kmb_prev = e_kmb;
            e_ml_prev  = e_ml;
            e_gs_prev  = e_gs;
            h_prev     = h;
        }

        fclose(plik);
        printf("\nZapisano: konwergencja.dat\n");
        printf("UWAGA: p ~ 2 oznacza zgodnosc z teoretycznym rzedem O(h^2).\n\n");
    }

    /* (2) PROFILE U(x,t_i) */
    {
        double h = 0.05;

        plik = fopen("profil_kmb.dat", "w");
        fprintf(plik, "# KMB h = %.2f, lam = %.2f\n# x  U_num  U_an\n", h, LAM_KMB);
        rozwiaz_KMB(h, NULL, plik, &snap_t, NULL);
        fclose(plik);
        printf("Zapisano: profil_kmb.dat (h = %.2f)\n", h);

        plik = fopen("profil_ml.dat", "w");
        fprintf(plik, "# ML+Thomas h = %.2f, lam = %.2f\n# x  U_num  U_an\n", h, LAM_ML);
        rozwiaz_ML_thomas(h, NULL, plik, &snap_t, NULL);
        fclose(plik);
        printf("Zapisano: profil_ml.dat       (h = %.2f)\n", h);

        plik = fopen("profil_ml_gs.dat", "w");
        fprintf(plik, "# ML+Gauss-Seidel h = %.2f, lam = %.2f\n# x  U_num  U_an\n", h, LAM_ML);
        rozwiaz_ML_gs(h, NULL, plik, &snap_t, NULL);
        fclose(plik);
        printf("Zapisano: profil_ml_gs.dat    (h = %.2f)\n\n", h);
    }

    /* (3) EWOLUCJA BLEDU W CZASIE */
    {
        double h = 0.05;

        plik = fopen("blad_t_kmb.dat", "w");
        fprintf(plik, "# t  max_blad  (KMB, h = %.2f, lam = %.2f)\n", h, LAM_KMB);
        rozwiaz_KMB(h, plik, NULL, NULL, NULL);
        fclose(plik);
        printf("Zapisano: blad_t_kmb.dat (h = %.2f)\n", h);

        plik = fopen("blad_t_ml.dat", "w");
        fprintf(plik, "# t  max_blad  (ML+Thomas, h = %.2f, lam = %.2f)\n", h, LAM_ML);
        rozwiaz_ML_thomas(h, plik, NULL, NULL, NULL);
        fclose(plik);
        printf("Zapisano: blad_t_ml.dat     (h = %.2f)\n", h);

        plik = fopen("blad_t_ml_gs.dat", "w");
        fprintf(plik, "# t  max_blad  (ML+Gauss-Seidel, h = %.2f, lam = %.2f)\n", h, LAM_ML);
        rozwiaz_ML_gs(h, plik, NULL, NULL, NULL);
        fclose(plik);
        printf("Zapisano: blad_t_ml_gs.dat  (h = %.2f)\n\n", h);
    }

    /* POROWNANIE: THOMAS VS GAUSS-SEIDEL */
    {
        double h = 0.05;
        double t_thomas, t_gs;

        double e_thomas = rozwiaz_ML_thomas(h, NULL, NULL, NULL, &t_thomas);
        double e_gs     = rozwiaz_ML_gs(h, NULL, NULL, NULL, &t_gs);

        printf("Porownanie ML (h = %.2f):\n", h);
        printf("  Thomas:       blad = %.6e,  czas = %.3f s\n", e_thomas, t_thomas);
        printf("  Gauss-Seidel: blad = %.6e,  czas = %.3f s\n", e_gs, t_gs);
        printf("  roznica bledow: %.2e\n\n", fabs(e_thomas - e_gs));
    }

    printf("Generowanie wykresow: gnuplot plot1.gp plot2.gp plot3.gp\n");
    return 0;
}
