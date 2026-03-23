#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <iomanip>
using namespace std;

long double oblicz_fx(long double x);
long double oblicz_fx_taylor(long double x);
long double oblicz_blad_wzgledny(long double fx_dokl, long double fx_obliczone);
void metoda_podstawowa();
void metoda_ulepszona();

int main()
{
    metoda_podstawowa();
    metoda_ulepszona();
    return 0;
}

void metoda_podstawowa()
{
    ifstream plik_we("dane_do_laboratorium_2.txt");
    if (!plik_we.is_open())
    {
        cerr << "Nie mozna otworzyc pliku wejsciowego: dane_do_laboratorium_2.txt" << endl;
        return;
    }

    ofstream plik_wy("bledy_podstawowa.txt");
    if (!plik_wy.is_open())
    {
        cerr << "Nie mozna otworzyc pliku wyjsciowego: bledy_podstawowa.txt" << endl;
        return;
    }

    ofstream plik_wynik("wynik_podstawowa.txt");
    if (!plik_wynik.is_open())
    {
        cerr << "Nie mozna otworzyc pliku wyjsciowego: wynik_podstawowa.txt" << endl;
        return;
    }

    string linia_pomin;
    for (int i = 0; i < 3; i++)
    {
        getline(plik_we, linia_pomin);
    }

    plik_wy << "log10(x) log10(blad wzgledny)" << endl;
    plik_wynik << "log10(x) x f_dokl f_obl blad_wzgledny" << endl;
    cout << "=== Metoda podstawowa ===" << endl;

    long double log10x, x, fx_dokl;
    while (plik_we >> log10x >> x >> fx_dokl)
    {
        long double fx_obliczone = oblicz_fx(x);
        long double blad = oblicz_blad_wzgledny(fx_dokl, fx_obliczone);

        long double log10_blad;
        if (blad == 0.0L)
            log10_blad = -20.0L;
        else
            log10_blad = log10l(blad);

        plik_wy << log10x << " " << log10_blad << endl;
        plik_wynik << setprecision(20) << log10x << " " << x << " " << fx_dokl << " " << fx_obliczone << " " << setprecision(8) << blad << endl;

        printf("x=%.20Lg   f_dokl=%.20Lg   f_obl=%.20Lg   blad=%.4Lg\n",
               x, fx_dokl, fx_obliczone, blad);
    }

    plik_we.close();
    plik_wy.close();
    plik_wynik.close();
}

void metoda_ulepszona()
{
    ifstream plik_we("dane_do_laboratorium_2.txt");
    if (!plik_we.is_open())
    {
        cerr << "Nie mozna otworzyc pliku wejsciowego: dane_do_laboratorium_2.txt" << endl;
        return;
    }

    ofstream plik_wy("bledy_ulepszona.txt");
    if (!plik_wy.is_open())
    {
        cerr << "Nie mozna otworzyc pliku wyjsciowego: bledy_ulepszona.txt" << endl;
        return;
    }

    ofstream plik_wynik("wynik_ulepszona.txt");
    if (!plik_wynik.is_open())
    {
        cerr << "Nie mozna otworzyc pliku wyjsciowego: wynik_ulepszona.txt" << endl;
        return;
    }

    string linia_pomin;
    for (int i = 0; i < 3; i++)
    {
        getline(plik_we, linia_pomin);
    }

    plik_wy << "log10(x) log10(blad wzgledny)" << endl;
    plik_wynik << "log10(x) x f_dokl f_obl blad_wzgledny" << endl;
    cout << "=== Metoda ulepszona - Taylor ===" << endl;

    long double log10x, x, fx_dokl;
    while (plik_we >> log10x >> x >> fx_dokl)
    {
        long double fx_obliczone;
        if (fabsl(x) < 0.1L)
            fx_obliczone = oblicz_fx_taylor(x);
        else
            fx_obliczone = oblicz_fx(x);

        long double blad = oblicz_blad_wzgledny(fx_dokl, fx_obliczone);

        long double log10_blad;
        if (blad == 0.0L)
            log10_blad = -20.0L;
        else
            log10_blad = log10l(blad);

        plik_wy << log10x << " " << log10_blad << endl;
        plik_wynik << setprecision(20) << log10x << " " << x << " " << fx_dokl << " " << fx_obliczone << " " << setprecision(8) << blad << endl;

        printf("x=%.20Lg   f_dokl=%.20Lg   f_obl=%.20Lg   blad=%.4Lg\n",
               x, fx_dokl, fx_obliczone, blad);
    }

    plik_we.close();
    plik_wy.close();
    plik_wynik.close();
}

long double oblicz_fx(long double x)
{
    long double licznik = x * x * x;
    long double odejmowanie = sinhl(x) - x;
    long double mianownik = 6.0L * odejmowanie;

    return licznik / mianownik;
}

long double oblicz_fx_taylor(long double x)
{
    long double x2 = x * x;
    long double x4 = x2 * x2;
    long double x6 = x4 * x2;
    long double x8 = x6 * x2;

    long double wyraz1 = x2 / 20.0L;      // x^2  / (5!/6)
    long double wyraz2 = x4 / 840.0L;     // x^4  / (7!/6)
    long double wyraz3 = x6 / 60480.0L;   // x^6  / (9!/6)
    long double wyraz4 = x8 / 6652800.0L; // x^8  / (11!/6)

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
