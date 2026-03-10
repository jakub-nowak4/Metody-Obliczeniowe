#include <iostream>
#include <iomanip>
#include <cstdio>

using namespace std;

void oblicz_epsilon_float() {
    float epsilon = 1.0f;
    int bity_mantysy = 0;

    while (true) {
        float polowa = epsilon / 2.0f;
        float suma = 1.0f + polowa;

        if (!(suma > 1.0f)) {
            break;
        }

        epsilon = polowa;
        bity_mantysy++;
    }

    int cyfry_znaczace = 0;
    float test = 1.0f;
    while (true) {
        float podzielone = test / 10.0f;
        float suma = 1.0f + podzielone;

        if (!(suma > 1.0f)) { 
            break;
        }

        test = podzielone;
        cyfry_znaczace++;
    }

    cout << "=== float ===" << endl;
    cout << "Epsilon maszynowy: ";
    printf("%.20g\n", epsilon);
    cout << "Bity mantysy (t): " << bity_mantysy << endl;
    cout << "Cyfry znaczace: " << cyfry_znaczace << endl;
    cout << endl;
}

void oblicz_epsilon_double() {
    double epsilon = 1.0;
    int bity_mantysy = 0;

    while (true) {
        double polowa = epsilon / 2.0;
        double suma = 1.0 + polowa;

        if (!(suma > 1.0)) {
            break;
        }

        epsilon = polowa;
        bity_mantysy++;
    }

    int cyfry_znaczace = 0;
    double test = 1.0;
    while (true) {
        double podzielone = test / 10.0;
        double suma = 1.0 + podzielone;

        if (!(suma > 1.0)) {
            break;
        }

        test = podzielone;
        cyfry_znaczace++;
    }

    cout << "=== double ===" << endl;
    cout << "Epsilon maszynowy: ";
    printf("%.20g\n", epsilon);
    cout << "Bity mantysy (t): " << bity_mantysy << endl;
    cout << "Cyfry znaczace: " << cyfry_znaczace << endl;
    cout << endl;
}

void oblicz_epsilon_long_double() {
    long double epsilon = 1.0L;
    int bity_mantysy = 0;

    while (true) {
        long double polowa = epsilon / 2.0L;
        long double suma = 1.0L + polowa;

        if (!(suma > 1.0L)) {
            break;
        }

        epsilon = polowa;
        bity_mantysy++;
    }

    int cyfry_znaczace = 0;
    long double test = 1.0L;
    while (true) {
        long double podzielone = test / 10.0L;
        long double suma = 1.0L + podzielone;

        if (!(suma > 1.0L)) {
            break;
        }

        test = podzielone;
        cyfry_znaczace++;
    }

    cout << "=== long double ===" << endl;
    cout << "Epsilon maszynowy: ";
    printf("%.20Lg\n", epsilon);
    cout << "Bity mantysy (t): " << bity_mantysy << endl;
    cout << "Cyfry znaczace: " << cyfry_znaczace << endl;
    cout << endl;
}

int main() {
    cout << "Doswiadczalne wyznaczanie epsilona maszynowego" << endl;
    cout << "fl(eps + 1) > 1, gdzie eps jest najmniejsze" << endl;
    cout << endl;

    oblicz_epsilon_float();
    oblicz_epsilon_double();
    oblicz_epsilon_long_double();

    return 0;
}
