#include <stdbool.h>

struct matrix_square
{
    char **data;
    int d;
};

typedef struct matrix_square matrix_square;

matrix_square *create_matrix(int d);
void delete_matrix(matrix_square *m);
char get_value_matrix(matrix_square *m, int i, int j);
void set_value_matrix(matrix_square *m, int i, int j, char val);
void write_matrix(matrix_square *m, char *filename);
void write_matrix_array(matrix_square **m, char *filename, int amount);
matrix_square *read_matrix(char *filename);
matrix_square **read_matrix_array(char *filename, int *size);
