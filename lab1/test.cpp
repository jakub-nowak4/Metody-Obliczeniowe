#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;

template<typename T>
int policz_znaczace(T epsilon) {
    ostringstream oss;
    oss << fixed << setprecision(100) << epsilon;
    string s = oss.str();
    
    int pierwszy = s.find_first_not_of("0.");
    if (pierwszy == string::npos) return 0;
    
    int ostatni = s.find_last_not_of('0');
    
    // ile cyfr pomiedzy, wlacznie
    // bez kropek!
    int count = 0;
    for (int i = pierwszy; i <= ostatni; ++i) {
        if (s[i] != '.' && s[i] != '0') count++;
    }
    return count;
}

int main() {
    float f = 1.1920928955078125e-07f;
    double d = 2.2204460492503130808e-16;
    long double ld = 1.084202172485504434e-19L;
    
    cout << "f: " << policz_znaczace(f) << endl;
    cout << "d: " << policz_znaczace(d) << endl;
    cout << "ld: " << policz_znaczace(ld) << endl;
    return 0;
}
