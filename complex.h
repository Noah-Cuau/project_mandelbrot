struct Complex
{
    double re;
    double im;
};

struct Complex_sequence
{
    int power;
    struct Complex c;
    int nb_computed;
    struct Complex *values;
    int max_value;
};

typedef struct Complex Complex;
typedef struct Complex_sequence Complex_sequence;

double get_re(Complex *z);
double get_im(Complex *z);
void set_re(Complex *z, double a);
void set_im(Complex *z, double a);
void complex_real_multiplication(Complex *z, double r, Complex *result);
void complex_addition(Complex *z1, Complex *z2, Complex *result);
void complex_soustraction(Complex *z1, Complex *z2, Complex *result);
void complex_multiplication(Complex *z1, Complex *z2, Complex *result);
void complex_power(Complex *z1, int power, Complex *result);
double get_modulus(Complex *z);
void print_complex(Complex *z, int modulus);
Complex_sequence *create_complex_sequence(Complex *c, int power);
void free_complex_sequence(Complex_sequence *c);
Complex compute_next_term(Complex_sequence *u);
Complex *get_term(Complex_sequence *u, int i);
Complex *get_last_term(Complex_sequence *u);
void compute_n_terms(Complex_sequence *u, int n);
int is_bounded_v1(Complex_sequence *u, int precision);
void print_sequence(Complex_sequence *u, int modulus);

void debug_complex(void);
void print_double_array(double *arr, int size);