#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib>

#define M_PI 3.14159265358979323846

// Badana funkcja f(x) = cos(x)
template <typename T>
T funkcja(T x)
{
    return std::cos(x);
}

// Dokładna pochodna funkcji f'(x) = -sin(x)
template <typename T>
T dokladna_pochodna(T x)
{
    return -std::sin(x);
}

// Różnica postępująca (dwupunktowa), rzędu O(h)
template <typename T>
T wprzod2(T x, T h)
{
    return (funkcja(x + h) - funkcja(x)) / h;
}

// Różnica postępująca (trzypunktowa), rzędu O(h^2)
template <typename T>
T wprzod3(T x, T h)
{
    return (-T(3) / T(2) * funkcja(x) + T(2) * funkcja(x + h) - T(1) / T(2) * funkcja(x + 2 * h)) / h;
}

// Różnica centralna (dwupunktowa), rzędu O(h^2)
template <typename T>
T centralna2(T x, T h)
{
    return (funkcja(x + h) - funkcja(x - h)) / (T(2) * h);
}

// Różnica wsteczna (dwupunktowa), rzędu O(h)
template <typename T>
T wtyl2(T x, T h)
{
    return (funkcja(x) - funkcja(x - h)) / h;
}

// Różnica wsteczna (trzypunktowa), rzędu O(h^2)
template <typename T>
T wtyl3(T x, T h)
{
    return (T(3) / T(2) * funkcja(x) - T(2) * funkcja(x - h) + T(1) / T(2) * funkcja(x - 2 * h)) / h;
}

// Funkcja obliczająca błędy bezwzględne dla różnych kroków h
// i zapisująca je do pliku (w skali logarytmicznej).
template <typename T>
void oblicz_bledy(
    const std::string &etykieta,
    T punkt_x,
    T (*przyblizenie)(T, T),
    std::ofstream &plik_wyjsciowy,
    int liczba_krokow = 200)
{
    T dokladna = dokladna_pochodna(punkt_x);
    for (int i = 0; i <= liczba_krokow; ++i)
    {
        // Generujemy logarytm z kroku h od -20 do -1 (zmniejszanie kroku)
        double log_h = -20.0 + 19.0 * i / liczba_krokow;
        T h = static_cast<T>(std::pow(10.0, log_h));

        T wartosc_przyblizona = przyblizenie(punkt_x, h);
        T blad = std::abs(wartosc_przyblizona - dokladna);

        // Zapisujemy tylko dodatnie wartości błędów
        if (blad > T(0))
        {
            plik_wyjsciowy << log_h << " " << std::log10(static_cast<double>(blad)) << " " << etykieta << "\n";
        }
    }
    plik_wyjsciowy << "\n\n";
}

// Funkcja wypisująca i oceniająca eksperymentalny rząd dokładności
template <typename T>
void wypisz_rzad(const std::string &nazwa, T punkt_x, T (*przyblizenie)(T, T), int teoretyczny_p)
{
    // Dobieramy kroki h, dla których dominuje błąd metody (błąd obcięcia),
    // a nie błąd reprezentacji zmiennoprzecinkowej (maszynowy).
    T h1 = 1e-1;
    T h2 = 1e-3;

    T dokladna = dokladna_pochodna(punkt_x);
    T blad1 = std::abs(przyblizenie(punkt_x, h1) - dokladna);
    T blad2 = std::abs(przyblizenie(punkt_x, h2) - dokladna);

    double p_eksperymentalne = 0.0;

    // Obliczanie współczynnika zgodności logarytmicznej
    if (blad1 > 0 && blad2 > 0)
    {
        p_eksperymentalne = (std::log10(static_cast<double>(blad1)) - std::log10(static_cast<double>(blad2))) /
                            (std::log10(static_cast<double>(h1)) - std::log10(static_cast<double>(h2)));
    }

    std::cout << std::left << std::setw(25) << nazwa
              << " | Teoretyczny p: " << teoretyczny_p
              << " | Eksperymentalny p: " << std::fixed << std::setprecision(4) << p_eksperymentalne << "\n";
}

// Generuje skrypt i uruchamia Gnuplota do wykreślenia wyników
void wygeneruj_i_uruchom_gnuplot()
{
    std::ofstream skrypt("plot_errors.plt");

    skrypt << "set terminal pngcairo size 1200,900 enhanced font 'Arial,11'\n";
    skrypt << "set output 'wykres_bledow.png'\n";
    skrypt << "set grid\n";
    skrypt << "set xlabel 'log_{10}(h)'\n";
    skrypt << "set ylabel 'log_{10}(|blad bezwzgledny|)'\n";
    skrypt << "set title 'Błędy przybliżeń różniczkowych (Double vs Long Double)'\n";
    skrypt << "set key outside right top\n\n";

    skrypt << "plot \\\n";
    skrypt << "  'errors_double.dat' index 0 with lines title 'Dbl: wprzod2 (x=0)', \\\n";
    skrypt << "  'errors_double.dat' index 1 with lines title 'Dbl: wprzod3 (x=0)', \\\n";
    skrypt << "  'errors_double.dat' index 2 with lines title 'Dbl: cen (x=π/4)', \\\n";
    skrypt << "  'errors_double.dat' index 3 with lines title 'Dbl: wtyl2 (x=π/2)', \\\n";
    skrypt << "  'errors_double.dat' index 4 with lines title 'Dbl: wtyl3 (x=π/2)', \\\n";
    skrypt << "  'errors_longdouble.dat' index 0 with lines dt 2 title 'LDbl: wprzod2 (x=0)', \\\n";
    skrypt << "  'errors_longdouble.dat' index 1 with lines dt 2 title 'LDbl: wprzod3 (x=0)', \\\n";
    skrypt << "  'errors_longdouble.dat' index 2 with lines dt 2 title 'LDbl: cen (x=π/4)', \\\n";
    skrypt << "  'errors_longdouble.dat' index 3 with lines dt 2 title 'LDbl: wtyl2 (x=π/2)', \\\n";
    skrypt << "  'errors_longdouble.dat' index 4 with lines dt 2 title 'LDbl: wtyl3 (x=π/2)'\n\n";

    skrypt << "set terminal pngcairo size 800,600 enhanced font 'Arial,11'\n\n";

    skrypt << "set output 'wykres_wprzod2.png'\n";
    skrypt << "set title 'Błąd przybliżenia - r. w przód 2-punktowa (x=0)'\n";
    skrypt << "plot 'errors_double.dat' index 0 with lines title 'Double', \\\n";
    skrypt << "     'errors_longdouble.dat' index 0 with lines dt 2 title 'Long Double'\n\n";

    skrypt << "set output 'wykres_wprzod3.png'\n";
    skrypt << "set title 'Błąd przybliżenia - r. w przód 3-punktowa (x=0)'\n";
    skrypt << "plot 'errors_double.dat' index 1 with lines title 'Double', \\\n";
    skrypt << "     'errors_longdouble.dat' index 1 with lines dt 2 title 'Long Double'\n\n";

    skrypt << "set output 'wykres_centralna.png'\n";
    skrypt << "set title 'Błąd przybliżenia - r. centralna 2-punktowa (x=π/4)'\n";
    skrypt << "plot 'errors_double.dat' index 2 with lines title 'Double', \\\n";
    skrypt << "     'errors_longdouble.dat' index 2 with lines dt 2 title 'Long Double'\n\n";

    skrypt << "set output 'wykres_wtyl2.png'\n";
    skrypt << "set title 'Błąd przybliżenia - r. w tył 2-punktowa (x=π/2)'\n";
    skrypt << "plot 'errors_double.dat' index 3 with lines title 'Double', \\\n";
    skrypt << "     'errors_longdouble.dat' index 3 with lines dt 2 title 'Long Double'\n\n";

    skrypt << "set output 'wykres_wtyl3.png'\n";
    skrypt << "set title 'Błąd przybliżenia - r. w tył 3-punktowa (x=π/2)'\n";
    skrypt << "plot 'errors_double.dat' index 4 with lines title 'Double', \\\n";
    skrypt << "     'errors_longdouble.dat' index 4 with lines dt 2 title 'Long Double'\n\n";

    skrypt.close();
    int ret = std::system("gnuplot plot_errors.plt");
    if (ret != 0)
    {
        std::cerr << "Ostrzezenie: wystapil problem z uruchomieniem Gnuplota." << std::endl;
    }
}

int main()
{
    // Punkty wyznaczone do badania: lewy kraniec, środek i prawy kraniec przedziału [0, pi/2]
    const double x_lewe = 0.0;
    const double x_srodek = M_PI / 4.0;
    const double x_prawe = M_PI / 2.0;

    // --- Wypisywanie rzędów dokładności ---
    std::cout << "--- RZEDY DOKLADNOSCI (DOUBLE) ---\n";
    wypisz_rzad<double>("x0_wprzod2 (2-punktowe)", x_lewe, wprzod2<double>, 1);
    wypisz_rzad<double>("x0_wprzod3 (3-punktowe)", x_lewe, wprzod3<double>, 2);
    wypisz_rzad<double>("xm_cen2 (centralne)", x_srodek, centralna2<double>, 2);
    wypisz_rzad<double>("xn_wtyl2 (2-punktowe)", x_prawe, wtyl2<double>, 1);
    wypisz_rzad<double>("xn_wtyl3 (3-punktowe)", x_prawe, wtyl3<double>, 2);

    std::cout << "\n--- RZEDY DOKLADNOSCI (LONG DOUBLE) ---\n";
    wypisz_rzad<long double>("x0_wprzod2 (2-punktowe)", (long double)x_lewe, wprzod2<long double>, 1);
    wypisz_rzad<long double>("x0_wprzod3 (3-punktowe)", (long double)x_lewe, wprzod3<long double>, 2);
    wypisz_rzad<long double>("xm_cen2 (centralne)", (long double)x_srodek, centralna2<long double>, 2);
    wypisz_rzad<long double>("xn_wtyl2 (2-punktowe)", (long double)x_prawe, wtyl2<long double>, 1);
    wypisz_rzad<long double>("xn_wtyl3 (3-punktowe)", (long double)x_prawe, wtyl3<long double>, 2);
    std::cout << "\n";

    // --- Generowanie plików z danymi ---
    {
        std::ofstream plik_z_wynikami("errors_double.dat");
        plik_z_wynikami << std::scientific << std::setprecision(15);
        oblicz_bledy<double>("x0_wprzod2", x_lewe, wprzod2<double>, plik_z_wynikami);
        oblicz_bledy<double>("x0_wprzod3", x_lewe, wprzod3<double>, plik_z_wynikami);
        oblicz_bledy<double>("xm_cen2", x_srodek, centralna2<double>, plik_z_wynikami);
        oblicz_bledy<double>("xn_wtyl2", x_prawe, wtyl2<double>, plik_z_wynikami);
        oblicz_bledy<double>("xn_wtyl3", x_prawe, wtyl3<double>, plik_z_wynikami);
    }

    {
        std::ofstream plik_z_wynikami("errors_longdouble.dat");
        plik_z_wynikami << std::scientific << std::setprecision(18);
        oblicz_bledy<long double>("x0_wprzod2", (long double)x_lewe, wprzod2<long double>, plik_z_wynikami);
        oblicz_bledy<long double>("x0_wprzod3", (long double)x_lewe, wprzod3<long double>, plik_z_wynikami);
        oblicz_bledy<long double>("xm_cen2", (long double)x_srodek, centralna2<long double>, plik_z_wynikami);
        oblicz_bledy<long double>("xn_wtyl2", (long double)x_prawe, wtyl2<long double>, plik_z_wynikami);
        oblicz_bledy<long double>("xn_wtyl3", (long double)x_prawe, wtyl3<long double>, plik_z_wynikami);
    }

    wygeneruj_i_uruchom_gnuplot();
    return 0;
}