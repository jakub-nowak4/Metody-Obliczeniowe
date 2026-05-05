#!/bin/bash
sed -i '/double normaResiduum/,/^}/c\
// Oblicza norme maksimum residuum (najgorsze niespelnienie pojedynczego rownania)\
double normaResiduum(double rozwiazanie[ROZMIAR])\
{\
    double max_residuum = 0.0;\
    for (int wiersz = 0; wiersz < ROZMIAR; wiersz++)\
    {\
        double skladnik_residuum = prawaStrona[wiersz];\
        for (int kolumna = 0; kolumna < ROZMIAR; kolumna++)\
        {\
            skladnik_residuum -= macierzA[wiersz][kolumna] * rozwiazanie[kolumna];\
        }\
        if (abs(skladnik_residuum) > max_residuum) {\
            max_residuum = abs(skladnik_residuum);\
        }\
    }\
    return max_residuum;\
}' /home/jakub/Metody-Obliczeniowe/lab7/main.cpp

sed -i '/double normaRoznicy/,/^}/c\
// Oblicza norme maksimum roznicy dwoch wektorow (najwiekszy blad pojedynczej zmiennej)\
double normaRoznicy(double rozwiazanie[ROZMIAR], double poprzednie[ROZMIAR])\
{\
    double max_roznica = 0.0;\
    for (int i = 0; i < ROZMIAR; i++)\
    {\
        double roznica = abs(rozwiazanie[i] - poprzednie[i]);\
        if (roznica > max_roznica) {\
            max_roznica = roznica;\
        }\
    }\
    return max_roznica;\
}' /home/jakub/Metody-Obliczeniowe/lab7/main.cpp
