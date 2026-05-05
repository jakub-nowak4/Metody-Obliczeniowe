set terminal pngcairo size 1200,900 enhanced font 'Arial,11'
set output 'wykres_bledow.png'
set grid
set xlabel 'log_{10}(h)'
set ylabel 'log_{10}(|blad bezwzgledny|)'
set title 'Błędy przybliżeń różniczkowych (Double vs Long Double)'
set key outside right top

plot \
  'errors_double.dat' index 0 with lines title 'Dbl: wprzod2 (x=0)', \
  'errors_double.dat' index 1 with lines title 'Dbl: wprzod3 (x=0)', \
  'errors_double.dat' index 2 with lines title 'Dbl: cen (x=π/4)', \
  'errors_double.dat' index 3 with lines title 'Dbl: wtyl2 (x=π/2)', \
  'errors_double.dat' index 4 with lines title 'Dbl: wtyl3 (x=π/2)', \
  'errors_longdouble.dat' index 0 with lines dt 2 title 'LDbl: wprzod2 (x=0)', \
  'errors_longdouble.dat' index 1 with lines dt 2 title 'LDbl: wprzod3 (x=0)', \
  'errors_longdouble.dat' index 2 with lines dt 2 title 'LDbl: cen (x=π/4)', \
  'errors_longdouble.dat' index 3 with lines dt 2 title 'LDbl: wtyl2 (x=π/2)', \
  'errors_longdouble.dat' index 4 with lines dt 2 title 'LDbl: wtyl3 (x=π/2)'

set terminal pngcairo size 800,600 enhanced font 'Arial,11'

set output 'wykres_wprzod2.png'
set title 'Błąd przybliżenia - r. w przód 2-punktowa (x=0)'
plot 'errors_double.dat' index 0 with lines title 'Double', \
     'errors_longdouble.dat' index 0 with lines dt 2 title 'Long Double'

set output 'wykres_wprzod3.png'
set title 'Błąd przybliżenia - r. w przód 3-punktowa (x=0)'
plot 'errors_double.dat' index 1 with lines title 'Double', \
     'errors_longdouble.dat' index 1 with lines dt 2 title 'Long Double'

set output 'wykres_centralna.png'
set title 'Błąd przybliżenia - r. centralna 2-punktowa (x=π/4)'
plot 'errors_double.dat' index 2 with lines title 'Double', \
     'errors_longdouble.dat' index 2 with lines dt 2 title 'Long Double'

set output 'wykres_wtyl2.png'
set title 'Błąd przybliżenia - r. w tył 2-punktowa (x=π/2)'
plot 'errors_double.dat' index 3 with lines title 'Double', \
     'errors_longdouble.dat' index 3 with lines dt 2 title 'Long Double'

set output 'wykres_wtyl3.png'
set title 'Błąd przybliżenia - r. w tył 3-punktowa (x=π/2)'
plot 'errors_double.dat' index 4 with lines title 'Double', \
     'errors_longdouble.dat' index 4 with lines dt 2 title 'Long Double'

