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

// Oblicza norme L2 residuum: ||b - A*x||
double normaResiduum(double rozwiazanie[ROZMIAR])
{
    double suma = 0.0;
    for (int wiersz = 0; wiersz < ROZMIAR; wiersz++)
    {
        double skladnik_residuum = prawaStrona[wiersz];
        for (int kolumna = 0; kolumna < ROZMIAR; kolumna++)
        {
            skladnik_residuum -= macierzA[wiersz][kolumna] * rozwiazanie[kolumna];
        }
        suma += skladnik_residuum * skladnik_residuum;
    }
    return sqrt(suma);
}

// Oblicza norme L2 roznicy dwoch wektorow: ||x_nowe - x_poprzednie||
double normaRoznicy(double rozwiazanie[ROZMIAR], double poprzednie[ROZMIAR])
{
    double suma = 0.0;
    for (int i = 0; i < ROZMIAR; i++)
    {
        double roznica = rozwiazanie[i] - poprzednie[i];
        suma += roznica * roznica;
    }
    return sqrt(suma);
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
// -------------------------
void jacobi()
{
    cout << "\n========== METODA JACOBIEGO ==========\n";
    cout << "Kryteria stopu: (1) ||dx|| < " << TOLERANCJA_DX
         << "  (2) ||r|| < " << TOLERANCJA_R
         << "  (3) iter > " << MAKS_ITERACJI << "\n\n";

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
// -------------------------
void gaussSeidel()
{
    cout << "\n========== METODA GAUSSA-SEIDELA ==========\n";
    cout << "Kryteria stopu: (1) ||dx|| < " << TOLERANCJA_DX
         << "  (2) ||r|| < " << TOLERANCJA_R
         << "  (3) iter > " << MAKS_ITERACJI << "\n\n";

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
void sor(double parametrOmega)
{
    cout << "\n========== METODA SOR (omega = " << parametrOmega << ") ==========\n";
    cout << "Kryteria stopu: (1) ||dx|| < " << TOLERANCJA_DX
         << "  (2) ||r|| < " << TOLERANCJA_R
         << "  (3) iter > " << MAKS_ITERACJI << "\n\n";

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
    cout << "  Uzyto 2 roznych norm docelowych stopowania algorytmu (Makra):\n";
    cout << "    (1) estymator bledu ||x^(n) - x^(n-1)|| < " << TOLERANCJA_DX << "\n";
    cout << "    (2) norma residuum  ||b - Ax||          < " << TOLERANCJA_R << "\n";
    cout << "====================================================\n";

    jacobi();
    gaussSeidel();

    sor(0.5);

    return 0;
}
