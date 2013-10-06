
#ifndef MAIN_H
#define MAIN_H

#define error_exit(...) {printf(__VA_ARGS__); return EXIT_FAILURE;}

#define boolean char
#define TRUE 1
#define FALSE 0

#define CONST_G 0.0000000000667384
#define DOUBLE_EPSILON 0.0000000001

void write_init(FILE* out1, FILE* out2, boolean verb, long double m1, vector p1, vector v1, long double m2, vector p2, vector v2, long double dt, long double t_final, long double dt_output);

void write_end(FILE* out1, FILE* out2, boolean verb);

void write_output(FILE* out1, FILE* out2, boolean verb, long double t, vector p1, vector p2);

#endif
