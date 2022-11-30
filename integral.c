#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <stdbool.h>

typedef double afunc(double);

extern double f1(double);
extern double f2(double);
extern double f3(double);
extern double t_f1(double);
extern double t_f2(double);
extern double t_f3(double);

struct {
    const char *name; // -- длинный параметр
    int has_arg;
    int *flag;
    int val;
} option;

static int its = 0;

static double root(afunc *f, afunc *g, double a, double b, double eps1) {
    double Fa, Fb, c;
    bool one_sign;
    while (true) {
        its++;
        Fa = (*f)(a) - (*g)(a);
        Fb = (*f)(b) - (*g)(b);
        c = (a * Fb - b * Fa) / (Fb - Fa);
        one_sign = ((Fa > 0) && ((*f)((a + b) / 2) - (*g)((a + b) / 2) > (Fa + Fb) / 2))
                   || ((Fa < 0) && ((*f)((a + b) / 2) - (*g)((a + b) / 2) < (Fa + Fb) / 2));
        if (one_sign) {
            a = c;
            if (((*f)(c) - (*g)(c)) * (f(c + eps1) - (*g)(c + eps1)) < 0)
                break;
        }
        else {
            b = c;
            if (((*f)(c) - (*g)(c)) * ((*f)(c - eps1) - (*g)(c - eps1)) < 0)
                break;
        }
    }
    return c;
}

static double integral(afunc *f, double a, double b, double eps2) {
    const double p = 1.0/3;
    bool first = true;
    double n0 = 10;
    double h, I, In = 0, I2n = 0; //i2n - curr, in - prev
    while (true) {
        In = I2n;
        h = (b - a) / n0;
        I2n = 0;
        for (int i = 0; i < n0; ++i) {
            I2n += (*f)(a + (i + 0.5) * h);
        }
        I2n *= h;
        if ((!first) && (eps2 > p * fabs(In - I2n))) {
            break;
        }
        n0 *= 2;
        first = false;
    }
    I = I2n;
    return I;
}

static afunc *map(int fn) {
    if (fn == 1) {
        return &f1;
    }
    else if (fn == 2) {
        return &f2;
    }
    else if (fn == 3) {
        return &f3;
    }
    else if (fn == 4) {
        return &t_f1;
    }
    else if (fn == 5) {
        return &t_f2;
    }
        return &t_f3;
}

int main(int argc, char *argv[]) {
    double tmp = 0;
    int f1n, f2n;
    double a, b, e, r, ans;
    int *longindex = NULL;
    const char *short_options = "hriR:I:";
    const struct option long_options[] = {
            { "help", no_argument, NULL, 'h' },
            { "root", no_argument, NULL, 'r' },
            { "iterations", no_argument, NULL, 'i' },
            { "test-root", required_argument, NULL, 'R' },
            { "test-integral", required_argument, NULL, 'I' },
            { NULL, 0, NULL, 0}
    };
    int c = getopt_long(argc, argv, short_options,
                       long_options, longindex);
    switch (c) {
        case 'h':
            printf("--help, -h               вывести этот мануал\n"
                   "--root, -r               вывести абсциссы точек пересечения кривых\n"
                   "--iterations, -r         вывести число итераций, потребовавшихся на приближенное решение уравнений при поиске точек пересечения\n"
                   "--test-root, -R          позволяют протестировать функцию root\n"
                   "--test-integral, -I      позволяют протестировать функцию integral\n");
            break;
        case 'r':
            printf("f1 x f2 : %lf\n", root(&f1, &f2, 1.2, 2.5, 0.0000001));
            printf("f2 x f3 : %lf\n", root(&f2, &f3, 0, 1.2, 0.0000001));
            printf("f1 x f3 : %lf\n", root(&f1, &f3, -0.25, 0, 0.0000001));
            break;
        case 'i':
            tmp = root(&f1, &f2, 1.2, 2.5, 0.0000001);
            printf("f1 x f2 : %d\n", its);
            its = 0;
            tmp = root(&f2, &f3, 0, 1.2, 0.0000001);
            printf("f2 x f3 : %d\n", its);
            its = 0;
            tmp = root(&f1, &f3, -0.25, 0, 0.0000001);
            printf("f1 x f3 : %d\n", its);
            its = 0;
            break;
        case 'R':
            sscanf(optarg, "%d:%d:%lf:%lf:%lf:%lf", &f1n, &f2n, &a, &b, &e, &r);
            tmp = root(map(f1n), map(f2n), a, b, e);
            printf("%lf %lf %lf\n", tmp, fabs(tmp - r), fabs(tmp - r) / r);
            break;
        case 'I':
            sscanf(optarg, "%d:%lf:%lf:%lf:%lf", &f1n, &a, &b, &e, &r);
            tmp = integral(map(f1n), a, b, e);
            printf("%lf %lf %lf\n", tmp, fabs(tmp - r), fabs(tmp - r) / r);
            break;
        case -1:
            a = root(&f1, &f3, -0.25, 0, 0.0000001);
            b = root(&f1, &f2, 1.2, 2.5, 0.0000001);
            tmp = root(&f2, &f3, 0, 1.2, 0.0000001);
            ans = integral(&f1, a, b, 0.0001);
            ans -= integral(&f3, a, tmp, 0.0001);
            ans -= integral(&f2, tmp, b, 0.0001);
            printf("%lf\n", ans);
            break;
        default:
            printf("Wrong option, use --help for help\n");
    }
    return 0;
}
