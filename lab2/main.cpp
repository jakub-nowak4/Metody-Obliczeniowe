#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

void wczytaj_dane(const string& nazwa_pliku, vector<long double>& log10x_vec, vector<long double>& x_vec, vector<long double>& fx_dokl_vec);
long double oblicz_fx(long double x);
long double oblicz_fx_taylor(long double x);
long double oblicz_blad_wzgledny(long double fx_dokl, long double fx_obliczone);
void zapisz_dane(const string& nazwa_pliku, vector<long double>& log10x_vec, vector<long double>& log10_bledy_wzgledne_vec);
void metoda_podstawowa();
void metoda_ulepszona();

int main() {
    metoda_podstawowa();
    metoda_ulepszona();
    return 0;
}

void metoda_podstawowa()
{
    vector<long double> log10x_vec, x_vec, fx_dokl_vec;
    vector<long double> log10_bledy_vec;

    wczytaj_dane("dane_do_laboratorium_2.txt", log10x_vec, x_vec, fx_dokl_vec);
    cout << "=== Metoda podstawowa ===" << endl;
    cout << "Wczytano " << x_vec.size() << " punktow danych." << endl;

    for(size_t i = 0; i < x_vec.size(); i++)
    {
        long double fx_obliczone = oblicz_fx(x_vec[i]);
        long double blad = oblicz_blad_wzgledny(fx_dokl_vec[i], fx_obliczone);

        // log10(0) = -inf  psuje skale wykresu w gnuplot
        // wiec blad == 0 tak naprawde losowe trafienie w poprawne zaokraglenie
        if (blad == 0.0L)
            log10_bledy_vec.push_back(-20.0L);
        else
            log10_bledy_vec.push_back(log10l(blad));

        printf("x=%.20Lg   f_dokl=%.20Lg   f_obl=%.20Lg   blad=%.4Lg\n",
               x_vec[i], fx_dokl_vec[i], fx_obliczone, blad);
    }

    zapisz_dane("bledy_podstawowa.txt", log10x_vec, log10_bledy_vec);
}

void metoda_ulepszona()
{
    vector<long double> log10x_vec, x_vec, fx_dokl_vec;
    vector<long double> log10_bledy_vec;

    wczytaj_dane("dane_do_laboratorium_2.txt", log10x_vec, x_vec, fx_dokl_vec);
    cout << "=== Metoda ulepszona - Taylor ===" << endl;
    cout << "Wczytano " << x_vec.size() << " punktow danych." << endl;

    for(size_t i = 0; i < x_vec.size(); i++)
    {
        long double fx_obliczone;
        if (fabsl(x_vec[i]) < 0.1L)
            fx_obliczone = oblicz_fx_taylor(x_vec[i]);
        else
            fx_obliczone = oblicz_fx(x_vec[i]);

        long double blad = oblicz_blad_wzgledny(fx_dokl_vec[i], fx_obliczone);

        // log10(0) = -inf, co psuje skale wykresu w gnuplot
        // wiec blad == 0 tutaj sa to poprawne wyniki dla bardzo malych x w metodzie taylor
        if (blad == 0.0L)
            log10_bledy_vec.push_back(-20.0L);
        else
            log10_bledy_vec.push_back(log10l(blad));

        printf("x=%.20Lg   f_dokl=%.20Lg   f_obl=%.20Lg   blad=%.4Lg\n",
               x_vec[i], fx_dokl_vec[i], fx_obliczone, blad);
    }

    zapisz_dane("bledy_ulepszona.txt", log10x_vec, log10_bledy_vec);
}

void wczytaj_dane(const string& nazwa_pliku, vector<long double>& log10x_vec, vector<long double>& x_vec, vector<long double>& fx_dokl_vec)
{
    ifstream plik(nazwa_pliku);

    if (!plik.is_open()) {
        cerr << "Nie mozna otworzyc pliku: " << nazwa_pliku << endl;
        return;
    }

    string linia_pomin;
    for(int i = 0; i < 3; i++) {
        getline(plik, linia_pomin);
    }

    long double log10x, x, fx_dokl;

    while (plik >> log10x >> x >> fx_dokl) {
        log10x_vec.push_back(log10x);
        x_vec.push_back(x);
        fx_dokl_vec.push_back(fx_dokl);
    }
    plik.close();
}

long double oblicz_fx(long double x)
{
    long double licznik = x * x * x;
    long double odejmowanie = sinhl(x) - x;
    long double mianownik = 6.0L * fabsl(odejmowanie);

    return licznik / mianownik;
}

long double oblicz_fx_taylor(long double x)
{
    long double x2 = x * x;
    long double x4 = x2 * x2;
    long double x6 = x4 * x2;
    long double x8 = x6 * x2;

    long double wyraz1 = x2 / 20.0L;       // x^2  / (5!/6)
    long double wyraz2 = x4 / 840.0L;       // x^4  / (7!/6)
    long double wyraz3 = x6 / 60480.0L;     // x^6  / (9!/6)
    long double wyraz4 = x8 / 6652800.0L;   // x^8  / (11!/6)

    long double mianownik = 1.0L + wyraz1 + wyraz2 + wyraz3 + wyraz4;

    return 1.0L / mianownik;
}

long double oblicz_blad_wzgledny(long double fx_dokl, long double fx_obliczone)
{
    long double odejmowanie = fx_obliczone - fx_dokl;
    long double blad_bezwzgledny = fabsl(odejmowanie);
    long double mianownik = fabsl(fx_dokl);
    return blad_bezwzgledny / mianownik;
}

void zapisz_dane(const string& nazwa_pliku, vector<long double>& log10x_vec, vector<long double>& log10_bledy_wzgledne_vec)
{
    ofstream plik(nazwa_pliku);
    if (!plik.is_open()) {
        cerr << "Nie mozna otworzyc pliku: " << nazwa_pliku << endl;
        return;
    }

    plik << "log10(x) log10(blad wzgledny)" << endl;
    for(size_t i = 0; i < log10x_vec.size(); i++) {
        plik << log10x_vec[i] << " " << log10_bledy_wzgledne_vec[i] << endl;
    }
    plik.close();
}
