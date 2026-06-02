#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

const double A = 25.0;
const int GRID = 1000;

double f(double x)
{
    return x / (1.0 + A * x * x * x * x);
}

// Ilorazy roznicowe w bazie Newtona, zgodnie ze schematem z wykladu Bieniasza.
// Po wykonaniu c[k] zawiera f[x_0,...,x_k].
void divided_diff(const double x[], const double y[], double c[], int n)
{
    for (int i = 0; i <= n; ++i) {
        c[i] = y[i];
    }

    for (int k = 1; k <= n; ++k) {
        for (int i = n; i >= k; --i) {
            c[i] = (c[i] - c[i - 1]) / (x[i] - x[i - k]);
        }
    }
}

// Ewaluacja wielomianu Newtona przez mnozenie zagniezdzone (schemat Hornera).
double newton_eval(const double x[], const double c[], int n, double t)
{
    double result = c[n];
    for (int i = n - 1; i >= 0; --i) {
        result = result * (t - x[i]) + c[i];
    }
    return result;
}

// Wezly Czebyszewa: x_k = -cos(pi*(2k-1)/(2n)), k=1,...,n.
// Sortowanie rosnace jest wygodne i stabilizuje zapis ilorazow roznicowych.
void cheb_nodes(double* x, int n)
{
    const double pi = std::acos(-1.0);
    for (int k = 1; k <= n; ++k) {
        x[k - 1] = -std::cos(pi * (2.0 * k - 1.0) / (2.0 * n));
    }
    std::sort(x, x + n);
}

// Wezly rownoodlegle: n+1 punktow i wielomian stopnia n.
void uniform_nodes(double* x, int n)
{
    for (int i = 0; i <= n; ++i) {
        x[i] = -1.0 + 2.0 * i / n;
    }
}

double grid_x(int i)
{
    return -1.0 + 2.0 * i / (GRID - 1);
}

double write_interpolation_file(const std::string& filename,
                                const std::vector<double>& nodes,
                                const std::vector<double>& coeff,
                                int degree,
                                double clip_limit)
{
    std::ofstream out(filename.c_str());
    out << std::setprecision(16);

    double max_err = 0.0;
    for (int i = 0; i < GRID; ++i) {
        const double x = grid_x(i);
        const double fx = f(x);
        const double px = newton_eval(nodes.data(), coeff.data(), degree, x);
        max_err = std::max(max_err, std::fabs(px - fx));

        double plotted = px;
        if (std::fabs(plotted) > clip_limit) {
            plotted = (plotted < 0.0 ? -clip_limit : clip_limit);
        }
        out << x << " " << plotted << " " << fx << "\n";
    }

    return max_err;
}

void write_function_file(double& max_abs_f)
{
    std::ofstream out("funkcja.dat");
    out << std::setprecision(16);

    max_abs_f = 0.0;
    for (int i = 0; i < GRID; ++i) {
        const double x = grid_x(i);
        const double fx = f(x);
        max_abs_f = std::max(max_abs_f, std::fabs(fx));
        out << x << " " << fx << "\n";
    }
}

void write_plot_scripts()
{
    FILE* gp = std::fopen("plot1.gp", "w");
    if (!gp) {
        std::perror("plot1.gp");
        std::exit(1);
    }
    std::fprintf(gp,
        "set terminal pngcairo size 1000,700\n"
        "set output 'wykres1_rownoodlegle.png'\n"
        "set title \"Zjawisko Rungego -- wezly rownoodlegle, a=25\"\n"
        "set xlabel \"x\"; set ylabel \"y\"\n"
        "set yrange [-0.6:0.6]\n"
        "set key top left\n"
        "plot 'funkcja.dat'           u 1:2 w l lw 3 lc 'black' title 'f(x)',\\\n"
        "     'interp_unif_n5.dat'    u 1:2 w l lw 1.5 dt 2 title 'n=5',\\\n"
        "     'interp_unif_n10.dat'   u 1:2 w l lw 1.5 dt 3 title 'n=10',\\\n"
        "     'interp_unif_n15.dat'   u 1:2 w l lw 1.5 dt 4 title 'n=15',\\\n"
        "     'interp_unif_n20.dat'   u 1:2 w l lw 1.5       title 'n=20'\n");
    std::fclose(gp);

    gp = std::fopen("plot2.gp", "w");
    if (!gp) {
        std::perror("plot2.gp");
        std::exit(1);
    }
    std::fprintf(gp,
        "set terminal pngcairo size 1000,700\n"
        "set output 'wykres2_czebyszew.png'\n"
        "set title \"Zjawisko Rungego -- wezly Czebyszewa, a=25\"\n"
        "set xlabel \"x\"; set ylabel \"y\"\n"
        "set yrange [-0.6:0.6]\n"
        "set key top left\n"
        "plot 'funkcja.dat'           u 1:2 w l lw 3 lc 'black' title 'f(x)',\\\n"
        "     'interp_cheb_n5.dat'    u 1:2 w l lw 1.5 dt 2 title 'n=5',\\\n"
        "     'interp_cheb_n10.dat'   u 1:2 w l lw 1.5 dt 3 title 'n=10',\\\n"
        "     'interp_cheb_n15.dat'   u 1:2 w l lw 1.5 dt 4 title 'n=15',\\\n"
        "     'interp_cheb_n20.dat'   u 1:2 w l lw 1.5       title 'n=20'\n");
    std::fclose(gp);

    gp = std::fopen("plot3.gp", "w");
    if (!gp) {
        std::perror("plot3.gp");
        std::exit(1);
    }
    std::fprintf(gp,
        "set terminal pngcairo size 1000,700\n"
        "set output 'wykres3_bledy.png'\n"
        "set logscale y\n"
        "set xlabel \"Stopien wielomianu n\"\n"
        "set ylabel \"max|p(x)-f(x)|\"\n"
        "set title \"Zbieznosc interpolacji\"\n"
        "set grid\n"
        "plot 'bledy.dat' u 1:2 w lp title 'rownoodlegle',\\\n"
        "     'bledy.dat' u 1:3 w lp title 'Czebyszew'\n");
    std::fclose(gp);
}

double run_case(const std::string& filename, bool chebyshev, int n, double clip_limit)
{
    const int node_count = chebyshev ? n : n + 1;
    const int degree = node_count - 1;

    std::vector<double> x(node_count);
    std::vector<double> y(node_count);
    std::vector<double> c(node_count);

    if (chebyshev) {
        cheb_nodes(x.data(), n);
    } else {
        uniform_nodes(x.data(), n);
    }

    for (int i = 0; i < node_count; ++i) {
        y[i] = f(x[i]);
    }

    divided_diff(x.data(), y.data(), c.data(), degree);
    return write_interpolation_file(filename, x, c, degree, clip_limit);
}

int main()
{
    const int degrees[] = {5, 10, 15, 20};

    double max_abs_f = 0.0;
    write_function_file(max_abs_f);
    const double clip_limit = 5.0 * max_abs_f;

    std::ofstream errors("bledy.dat");
    errors << std::setprecision(16);

    std::cout << std::scientific << std::setprecision(3);
    for (int i = 0; i < 4; ++i) {
        const int n = degrees[i];
        const double err_unif = run_case("interp_unif_n" + std::to_string(n) + ".dat",
                                         false, n, clip_limit);
        const double err_cheb = run_case("interp_cheb_n" + std::to_string(n) + ".dat",
                                         true, n, clip_limit);

        errors << n << " " << err_unif << " " << err_cheb << "\n";
        std::cout << "n=" << std::setw(2) << n
                  << "  rownoodl: max_err = " << err_unif
                  << "    czebyszew: max_err = " << err_cheb << "\n";
    }

    write_plot_scripts();
    return 0;
}
