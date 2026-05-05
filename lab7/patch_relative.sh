#!/bin/bash
sed -i '/\/\/ Oblicza norme maksimum residuum/,/^}/c\
// Oblicza estymator wzglednego residuum (wedlug normy maksimum)\
double normaResiduum(double rozwiazanie[ROZMIAR])\
{\
    double max_residuum = 0.0;\
    double max_b = 0.0;\
    for (int wiersz = 0; wiersz < ROZMIAR; wiersz++)\
    {\
        if (abs(prawaStrona[wiersz]) > max_b) max_b = abs(prawaStrona[wiersz]);\
        double skladnik_residuum = prawaStrona[wiersz];\
        for (int kolumna = 0; kolumna < ROZMIAR; kolumna++)\
        {\
            skladnik_residuum -= macierzA[wiersz][kolumna] * rozwiazanie[kolumna];\
        }\
        if (abs(skladnik_residuum) > max_residuum) {\
            max_residuum = abs(skladnik_residuum);\
        }\
    }\
    if (max_b == 0.0) return max_residuum;\
    return max_residuum / max_b;\
}' /home/jakub/Metody-Obliczeniowe/lab7/main.cpp

sed -i '/\/\/ Oblicza norme maksimum roznicy/,/^}/c\
// Oblicza estymator wzgledny dla wektora poprawek (wedlug normy maksimum)\
double normaRoznicy(double rozwiazanie[ROZMIAR], double poprzednie[ROZMIAR])\
{\
    double max_roznica = 0.0;\
    double max_wartosc = 0.0;\
    for (int i = 0; i < ROZMIAR; i++)\
    {\
        double roznica = abs(rozwiazanie[i] - poprzednie[i]);\
        if (roznica > max_roznica) max_roznica = roznica;\
        if (abs(rozwiazanie[i]) > max_wartosc) max_wartosc = abs(rozwiazanie[i]);\
    }\
    if (max_wartosc == 0.0) return max_roznica;\
    return max_roznica / max_wartosc;\
}' /home/jakub/Metody-Obliczeniowe/lab7/main.cpp
