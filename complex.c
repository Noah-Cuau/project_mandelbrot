#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "complex.h"
#include <limits.h>

void debug_complex(void)
{
    Complex a = {1.062, 0.92};
    Complex b = {2.0, -1.0};

    Complex_sequence *test = create_complex_sequence(&b, 2);
    is_bounded_v2(test, 30, 4);

    printf("So far so good\n");
}

double get_re(Complex *z)
{
    return (z)->re;
}

double get_im(Complex *z)
{
    return (z)->im;
}

void set_re(Complex *z, double a)
{
    (z)->re = a;
}
void set_im(Complex *z, double b)
{
    (z)->im = b;
}

void complex_real_multiplication(Complex *z, double r, Complex *result)
{
    set_re(result, get_re(z) * r);
    set_im(result, get_im(z) * r);
}

void complex_addition(Complex *z1, Complex *z2, Complex *result)
{
    set_re(result, get_re(z1) + get_re(z2));
    set_im(result, get_im(z1) + get_im(z2));
}

void complex_soustraction(Complex *z1, Complex *z2, Complex *result)
{
    Complex temp;
    complex_real_multiplication(z2, -1.0, &temp);

    complex_addition(z1, &temp, result);
}

void complex_multiplication(Complex *z1, Complex *z2, Complex *result)
{
    Complex temp;
    set_re(&temp, (get_re(z1) * get_re(z2)) - (get_im(z1) * get_im(z2)));
    set_im(&temp, (get_re(z1) * get_im(z2)) + (get_re(z2) * get_im(z1)));
    set_re(result, get_re(&temp));
    set_im(result, get_im(&temp));
}

double get_modulus(Complex *z)
{
    return sqrt(get_re(z) * get_re(z) + get_im(z) * get_im(z));
}

Complex_sequence *create_complex_sequence(Complex *c, int power)
{
    Complex_sequence *p;
    Complex *values = malloc(sizeof(Complex) * 5);
    p = malloc(sizeof(Complex_sequence));
    p->nb_computed = 2;
    p->values = values;
    p->c = *c;
    p->power = power;
    Complex complex_0 = {0.0, 0.0};
    p->values[0] = complex_0;
    p->values[1] = *c;
    p->max_value = 5;
    return p;
}

void free_complex_sequence(Complex_sequence *c)
{
    free(c->values);
    free(c);
}
Complex compute_next_term(Complex_sequence *u)
{

    if (u->nb_computed == u->max_value)
    {
        u->values = realloc(u->values, (u->max_value + 5) * sizeof(Complex));
        u->max_value = u->max_value + 5;
    }

    Complex z_n;

    complex_multiplication(&u->values[u->nb_computed - 1], &u->values[u->nb_computed - 1], &z_n);
    for (int i = 2; i < u->power; i++)
    {
        complex_multiplication(&z_n, &z_n, &z_n);
    }

    complex_addition(&z_n, &u->c, &z_n);
    u->values[u->nb_computed] = z_n;
    u->nb_computed += 1;
    return z_n;
}
Complex *get_term(Complex_sequence *u, int i)
{
    return &u->values[i];
}
Complex *get_last_term(Complex_sequence *u)
{
    return &u->values[u->nb_computed - 1];
}
void compute_n_terms(Complex_sequence *u, int n)
{
    for (int i = 2; i != n + 1; i++)
    {
        compute_next_term(u);
    }
}

int is_bounded_v1(Complex_sequence *u, int precision, int max_val)
{

    // printf("begin is_bounded\n");
    int treshold_inf = 0;
    for (int i = 0; i < precision; i++)
    {
        compute_next_term(u);
        if (isfinite(get_modulus(get_last_term(u))) || treshold_inf != 0)
        {
            treshold_inf = i;
        }
    }

    double ball_radius = 0.0;
    double modulus;
    for (int i = 0; i < precision; i++)
    {
        modulus = get_modulus(get_term(u, i));

        if (modulus > ball_radius)
        {

            ball_radius = modulus;
        }
    }

    Complex current_term;
    // printf("end is_bounded\n");
    current_term = compute_next_term(u);
    //
    double radius = get_modulus(&current_term);
    if (get_modulus(&current_term) < ball_radius || isfinite(radius))
    {
        free_complex_sequence(u);
        return max_val;
    }
    free_complex_sequence(u);

    // printf("%f\n", radius);
    return 0;
}

int is_bounded_v2(Complex_sequence *u, int precision, int max_val)
{
    int inf_treshold = -1;
    for (int i = 0; i < precision; i++)
    {

        // print_complex(get_last_term(u), 0);
        compute_next_term(u);
        if (inf_treshold == -1)
        {
            if (isnan(get_im(get_last_term(u))) || isnan(get_re(get_last_term(u))))
            {
                inf_treshold = i;
            }
        }
    }
    double modulus = 0.0;
    double ball_radius = 0.0;
    for (int i = 0; i < precision; i++)
    {
        modulus = get_modulus(get_term(u, i));

        if (modulus > ball_radius)
        {

            ball_radius = modulus;
        }
    }

    Complex last_term = compute_next_term(u);
    if (get_modulus(&last_term) < ball_radius && inf_treshold == -1)
    {
        free_complex_sequence(u);
        return 0;
    }
    else
    {
        free_complex_sequence(u);
        return 1 + (inf_treshold * max_val) / precision;
    }
}
void print_complex(Complex *z, int modulus)
{
    if (modulus)
    {
        printf("re = %f, im = %f, mod = %f ", get_re(z), get_im(z), get_modulus(z));
    }
    else
    {
        printf("re = %f, im = %f", get_re(z), get_im(z));
    }
}
void print_sequence(Complex_sequence *u, int modulus)
{
    for (int i = 0; i < u->nb_computed; i++)
    {
        printf("U%d : ", i);

        print_complex(get_term(u, i), modulus);
        printf("\n");
    }
}

void print_double_array(double *arr, int size)
{
    printf("[ ");
    for (int i = 0; i < size; i++)
    {
        printf("%f, ", arr[i]);
    }
    printf(" ]");
}