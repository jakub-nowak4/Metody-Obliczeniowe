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


    cout << "=== float ===" << endl;
    cout << "Epsilon maszynowy (naukowa): ";
    printf("%.20g\n", epsilon);
    printf("Epsilon maszynowy:          %.100f\n", epsilon);
    cout << "Bity mantysy (t): " << bity_mantysy << endl;
    cout << "Cyfry znaczace: 15" << endl;
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



    cout << "=== double ===" << endl;
    cout << "Epsilon maszynowy (naukowa): ";
    printf("%.20g\n", epsilon);
    printf("Epsilon maszynowy:          %.100f\n", epsilon);
    cout << "Bity mantysy (t): " << bity_mantysy << endl;
    cout << "Cyfry znaczace: 36" << endl;
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



    cout << "=== long double ===" << endl;
    cout << "Epsilon maszynowy (naukowa): ";
    printf("%.20Lg\n", epsilon);
    printf("Epsilon maszynowy:          %.100Lf\n", epsilon);
    cout << "Bity mantysy (t): " << bity_mantysy << endl;
    cout << "Cyfry znaczace: 42"<< endl;
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
