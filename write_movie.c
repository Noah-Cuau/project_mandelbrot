#include <stdio.h>
#include <stdlib.h>

#include "write_movie.h"

void write_image(char *filename, Complex_sequence *u, int size_x, int size_y)
{
    FILE *p_file;
    p_file = fopen(filename, "w");

    fprintf(p_file, "<");
    char *temp;
    for (int i = 1000; size_x > 0; i = i / 10)
    {

        *temp = (size_x / i) + '0';
        printf("%c\n", *temp);
        fprintf(p_file, temp);
        size_x = size_x % i;
    }
    fprintf(p_file, "><");

    for (int i = 1000; size_y > 0; i = i / 10)
    {
        *temp = (size_y / i) + '0';
        printf("%c\n", *temp);
        fprintf(p_file, temp);
        size_y = size_y % i;
    }

    fprintf(p_file, ">");
}