#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

matrix_square *create_matrix(int d)
{
    matrix_square *m = malloc(sizeof(matrix_square));
    m->d = d;
    m->data = malloc(sizeof(char *) * d);
    for (int i = 0; i < d; i++)
    {
        m->data[i] = malloc(sizeof(char) * d);
    }
    return m;
}
void delete_matrix(matrix_square *m)
{
    for (int i = 0; i < m->d; i++)
    {
        free(m->data[i]);
    }
    free(m->data);
    free(m);
}

char get_value_matrix(matrix_square *m, int i, int j)
{
    if (i >= m->d || j >= m->d)
    {
        fprintf(stderr, "Error, invalid matrix use\n");
    }
    return m->data[i][j];
}
void set_value_matrix(matrix_square *m, int i, int j, char val)
{
    if (i >= m->d || j >= m->d)
    {
        fprintf(stderr, "Error, invalid matrix use\n");
    }
    m->data[i][j] = val;
}

void write_matrix(matrix_square *m, char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        fprintf(stderr, "Failled to open file %s for writing.\n", filename);
    }

    fprintf(file, "%d\n", m->d);
    for (int i = 0; i < m->d; i++)
    {
        for (int j = 0; j < m->d; j++)
        {
            fprintf(file, "%c", get_value_matrix(m, i, j));
        }
    }
    fclose(file);
}

matrix_square *read_matrix(char *filename)
{

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Failled to open file %s for reading.\n", filename);
    }
    int d;
    fscanf(file, "%d%*[\n]", &d);
    matrix_square *m = create_matrix(d);
    char val;
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            fscanf(file, "%c", &val);
            set_value_matrix(m, i, j, val);
        }
    }
    fclose(file);
    return m;
}