#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include <string>

using namespace std;

#define TOLERANCJA_DX 1e-8
#define TOLERANCJA_R 1e-6
#define MAKS_ITERACJI 1000

const int ROZMIAR = 5; // liczba rownan / niewiadomych

// Macierz wspolczynnikow A i wektor prawych stron b
double macierzA[ROZMIAR][ROZMIAR] = {
    {50, 5, 4, 3, 2},
    {1, 40, 1, 2, 3},
    {4, 5, 30, -5, -4},
    {-3, -2, -1, 20, 0},
    {1, 2, 3, 4, 30}};

double prawaStrona[ROZMIAR] = {140, 67, 62, 89, 153};

double przyblizenieStartowe[ROZMIAR] = {6, 6, 6, 6, 6};

// Norma maksimum residuum: ||b - A*x||_inf  (bezwzgledna)
double normaResiduum(double rozwiazanie[ROZMIAR])
{
    double max_residuum = 0.0;
    for (int wiersz = 0; wiersz < ROZMIAR; wiersz++)
    {
        double skladnik_residuum = prawaStrona[wiersz];
        for (int kolumna = 0; kolumna < ROZMIAR; kolumna++)
        {
            skladnik_residuum -= macierzA[wiersz][kolumna] * rozwiazanie[kolumna];
        }
        if (abs(skladnik_residuum) > max_residuum) {
            max_residuum = abs(skladnik_residuum);
        }
    }
    return max_residuum;
}

// Norma maksimum roznicy dwoch przyblizen: ||x_nowe - x_poprzednie||_inf  (bezwzgledna)
double normaRoznicy(double rozwiazanie[ROZMIAR], double poprzednie[ROZMIAR])
{
    double max_roznica = 0.0;
    for (int i = 0; i < ROZMIAR; i++)
    {
        double roznica = abs(rozwiazanie[i] - poprzednie[i]);
        if (roznica > max_roznica) max_roznica = roznica;
    }
    return max_roznica;
}

// Wypisuje wyniki biezacej iteracji
void wypiszIteracje(int nrIteracji, double rozwiazanie[ROZMIAR], double estymatorBledu, double residuum)
{
    cout << "  Iter " << setw(4) << nrIteracji << " | x = [";
    for (int i = 0; i < ROZMIAR; i++)
    {
        cout << setw(10) << fixed << setprecision(6) << rozwiazanie[i] << (i < ROZMIAR - 1 ? ", " : "");
    }
    cout << "] | ||dx|| = " << scientific << setprecision(4) << estymatorBledu
         << " | ||r|| = " << scientific << setprecision(4) << residuum << "\n";
}

// -------------------------
// METODA JACOBIEGO
// x^(k+1) = -D^(-1)(L+U)x^(k) + D^(-1)b
// -------------------------
void jacobi()
{
    cout << "\n========== METODA JACOBIEGO ==========\n";
    cout << "Trzy niezalezne kryteria stopu (musza byc spelnione jednoczesnie):\n";
    cout << "  (1) bezwzgl. estymator bledu  ||x^(n) - x^(n-1)||_inf < " << TOLERANCJA_DX << "\n";
    cout << "  (2) bezwzgl. norma residuum   ||b - A*x^(n)||_inf     < " << TOLERANCJA_R << "\n";
    cout << "  (3) liczba iteracji n <= " << MAKS_ITERACJI << "\n\n";

    double rozwiazanie[ROZMIAR], noweRozwiazanie[ROZMIAR];
    for (int i = 0; i < ROZMIAR; i++)
    {
        rozwiazanie[i] = przyblizenieStartowe[i];
    }

    int nrIteracji = 0;
    double estymatorBledu = 1e12;
    double residuum = 1e12;


    while (nrIteracji < MAKS_ITERACJI)
    {
        for (int wiersz = 0; wiersz < ROZMIAR; wiersz++)
        {
            double sumaOffDiag = prawaStrona[wiersz];
            for (int kolumna = 0; kolumna < ROZMIAR; kolumna++)
            {
                if (kolumna != wiersz)
                {
                    sumaOffDiag -= macierzA[wiersz][kolumna] * rozwiazanie[kolumna];
                }
            }
            noweRozwiazanie[wiersz] = sumaOffDiag / macierzA[wiersz][wiersz];
        }

        estymatorBledu = normaRoznicy(noweRozwiazanie, rozwiazanie);
        residuum = normaResiduum(noweRozwiazanie);
        nrIteracji++;

        for (int i = 0; i < ROZMIAR; i++)
        {
            rozwiazanie[i] = noweRozwiazanie[i];
        }

        wypiszIteracje(nrIteracji, rozwiazanie, estymatorBledu, residuum);

        if (estymatorBledu < TOLERANCJA_DX && residuum < TOLERANCJA_R)
        {
            cout << "  >> Zbieznosc: ||dx|| < " << TOLERANCJA_DX << " oraz ||r|| < " << TOLERANCJA_R << " po " << nrIteracji << " iteracjach.\n";
            break;
        }
    }

    cout << "  Koncowe rozwiazanie: [";
    for (int i = 0; i < ROZMIAR; i++)
    {
        cout << fixed << setprecision(6) << rozwiazanie[i] << (i < ROZMIAR - 1 ? ", " : "");
    }
    cout << "]\n  Liczba iteracji: " << nrIteracji << "\n";
}

// -------------------------
// METODA GAUSSA-SEIDELA
// (L + D)x^(k) = -U*x^(k-1) + b
// -------------------------
void gaussSeidel()
{
    cout << defaultfloat << setprecision(6);
    cout << "\n========== METODA GAUSSA-SEIDELA ==========\n";
    cout << "Trzy niezalezne kryteria stopu (musza byc spelnione jednoczesnie):\n";
    cout << "  (1) bezwzgl. estymator bledu  ||x^(n) - x^(n-1)||_inf < " << TOLERANCJA_DX << "\n";
    cout << "  (2) bezwzgl. norma residuum   ||b - A*x^(n)||_inf     < " << TOLERANCJA_R << "\n";
    cout << "  (3) liczba iteracji n <= " << MAKS_ITERACJI << "\n\n";

    double rozwiazanie[ROZMIAR], poprzednieRozwiazanie[ROZMIAR];
    for (int i = 0; i < ROZMIAR; i++)
    {
        rozwiazanie[i] = przyblizenieStartowe[i];
    }

    int nrIteracji = 0;
    double estymatorBledu = 1e12;
    double residuum = 1e12;

    while (nrIteracji < MAKS_ITERACJI)
    {
        for (int i = 0; i < ROZMIAR; i++)
        {
            poprzednieRozwiazanie[i] = rozwiazanie[i];
        }

        for (int wiersz = 0; wiersz < ROZMIAR; wiersz++)
        {
            double sumaOffDiag = prawaStrona[wiersz];
            for (int kolumna = 0; kolumna < ROZMIAR; kolumna++)
            {
                if (kolumna != wiersz)
                {
                    sumaOffDiag -= macierzA[wiersz][kolumna] * rozwiazanie[kolumna];
                }
            }
            rozwiazanie[wiersz] = sumaOffDiag / macierzA[wiersz][wiersz];
        }

        estymatorBledu = normaRoznicy(rozwiazanie, poprzednieRozwiazanie);
        residuum = normaResiduum(rozwiazanie);
        nrIteracji++;

        wypiszIteracje(nrIteracji, rozwiazanie, estymatorBledu, residuum);

        if (estymatorBledu < TOLERANCJA_DX && residuum < TOLERANCJA_R)
        {
            cout << "  >> Zbieznosc: ||dx|| < " << TOLERANCJA_DX << " oraz ||r|| < " << TOLERANCJA_R << " po " << nrIteracji << " iteracjach.\n";
            break;
        }
    }

    cout << "  Koncowe rozwiazanie: [";
    for (int i = 0; i < ROZMIAR; i++)
    {
        cout << fixed << setprecision(6) << rozwiazanie[i] << (i < ROZMIAR - 1 ? ", " : "");
    }
    cout << "]\n  Liczba iteracji: " << nrIteracji << "\n";
}

// -------------------------
// METODA SOR (sukcesywna nadrelaksacja)
// -------------------------
// (D + omega*L)x^(k+1) = omega*b - [omega*U + (omega - 1)D]x^(k)
void sor(double parametrOmega)
{
    cout << defaultfloat << setprecision(6);
    cout << "\n========== METODA SOR (omega = " << parametrOmega << ") ==========\n";
    cout << "Trzy niezalezne kryteria stopu (musza byc spelnione jednoczesnie):\n";
    cout << "  (1) bezwzgl. estymator bledu  ||x^(n) - x^(n-1)||_inf < " << TOLERANCJA_DX << "\n";
    cout << "  (2) bezwzgl. norma residuum   ||b - A*x^(n)||_inf     < " << TOLERANCJA_R << "\n";
    cout << "  (3) liczba iteracji n <= " << MAKS_ITERACJI << "\n\n";

    double rozwiazanie[ROZMIAR], poprzednieRozwiazanie[ROZMIAR];
    for (int i = 0; i < ROZMIAR; i++)
    {
        rozwiazanie[i] = przyblizenieStartowe[i];
    }

    int nrIteracji = 0;
    double estymatorBledu = 1e12;
    double residuum = 1e12;

    while (nrIteracji < MAKS_ITERACJI)
    {
        for (int i = 0; i < ROZMIAR; i++)
        {
            poprzednieRozwiazanie[i] = rozwiazanie[i];
        }

        for (int wiersz = 0; wiersz < ROZMIAR; wiersz++)
        {
            double sumaOffDiag = prawaStrona[wiersz];
            for (int kolumna = 0; kolumna < ROZMIAR; kolumna++)
            {
                if (kolumna != wiersz)
                {
                    sumaOffDiag -= macierzA[wiersz][kolumna] * rozwiazanie[kolumna];
                }
            }
            double krokGS = sumaOffDiag / macierzA[wiersz][wiersz];
            rozwiazanie[wiersz] = (1.0 - parametrOmega) * poprzednieRozwiazanie[wiersz] + parametrOmega * krokGS;
        }

        estymatorBledu = normaRoznicy(rozwiazanie, poprzednieRozwiazanie);
        residuum = normaResiduum(rozwiazanie);
        nrIteracji++;

        wypiszIteracje(nrIteracji, rozwiazanie, estymatorBledu, residuum);

        if (estymatorBledu < TOLERANCJA_DX && residuum < TOLERANCJA_R)
        {
            cout << "  >> Zbieznosc: ||dx|| < " << TOLERANCJA_DX << " oraz ||r|| < " << TOLERANCJA_R << " po " << nrIteracji << " iteracjach.\n";
            break;
        }
    }

    cout << "  Koncowe rozwiazanie: [";
    for (int i = 0; i < ROZMIAR; i++)
    {
        cout << fixed << setprecision(6) << rozwiazanie[i] << (i < ROZMIAR - 1 ? ", " : "");
    }
    cout << "]\n  Liczba iteracji: " << nrIteracji << "\n";
}

int main()
{
    cout << "====================================================\n";
    cout << "  Uklad rownan liniowych Ax = b\n";
    cout << "  Rozmiar ukladu: " << ROZMIAR << "x" << ROZMIAR << "\n";
    cout << "  Przyblizenie startowe x0 = [6, 6, 6, 6, 6]\n";
    cout << "  Trzy niezalezne kryteria zakonczenia iteracji (laczone AND):\n";
    cout << "    (1) bezwzgl. estymator bledu  ||x^(n) - x^(n-1)||_inf < " << TOLERANCJA_DX << "\n";
    cout << "    (2) bezwzgl. norma residuum   ||b - A*x^(n)||_inf     < " << TOLERANCJA_R << "\n";
    cout << "    (3) liczba iteracji n <= " << MAKS_ITERACJI << "\n";
    cout << "====================================================\n";

    jacobi();
    gaussSeidel();

    sor(0.5);

    return 0;
}
