#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "complex.h"

void debug_complex(void)
{
    Complex a = {1.062, 0.92};
    Complex b = {2.0, -1.0};

    Complex_sequence *test = create_complex_sequence(&a, 2);

    printf("is bounded : %d\n", is_bounded_v1(test, 10));

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

int is_bounded_v1(Complex_sequence *u, int precision)
{

    // printf("begin is_bounded\n");
    for (int i = 0; i < precision; i++)
    {
        compute_next_term(u);
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
    if (get_modulus(&current_term) > ball_radius || isfinite(get_modulus(&current_term)))
    {
        free_complex_sequence(u);

        return 0;
    }
    free_complex_sequence(u);

    return 1;
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