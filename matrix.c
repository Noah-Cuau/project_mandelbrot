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
    fprintf(file, "%d\n", 1);
    for (int i = 0; i < m->d; i++)
    {
        for (int j = 0; j < m->d; j++)
        {
            fprintf(file, "%c", get_value_matrix(m, i, j));
        }
    }
    fclose(file);
}

void write_matrix_array(matrix_square **m, char *filename, int amount)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        fprintf(stderr, "Failled to open file %s for writing.\n", filename);
    }

    fprintf(file, "%d\n", 1000);
    fprintf(file, "%d\n", amount);
    for (int k = 0; k < amount; k++)
    {

        for (int i = 0; i < 1000; i++)
        {
            for (int j = 0; j < 1000; j++)
            {
                fprintf(file, "%c", get_value_matrix(m[k], i, j));
            }
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
    int a;
    fscanf(file, "%d%*[\n]", &a);
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

matrix_square **read_matrix_array(char *filename, int *size)
{

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Failled to open file %s for reading.\n", filename);
    }
    int d;
    fscanf(file, "%d%*[\n]", &d);
    char val;
    int nb_matrix;
    fscanf(file, "%d%*[\n]", &nb_matrix);
    matrix_square **m_array = malloc(sizeof(matrix_square) * nb_matrix);
    for (int k = 0; k < nb_matrix; k++)
    {
        m_array[k] = create_matrix(d);
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fscanf(file, "%c", &val);
                set_value_matrix(m_array[k], i, j, val);
            }
        }
    }
    fclose(file);
    *size = nb_matrix;
    return m_array;
}