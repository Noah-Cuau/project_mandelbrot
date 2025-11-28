#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "visualizer.h"
#include "write_movie.h"
#include "matrix.h"
#include <SDL2/SDL.h>

#define DEBUG 0

void set_center(Complex_plane *plane, int x, int y)
{
    plane->center_x = ((((double)((x - plane->win_x / 2)) / 200) - plane->center_x) * (1 / plane->zoom));
    plane->center_y = -((((double)((y - plane->win_y / 2)) / 200) - plane->center_y) * (1 / plane->zoom));
    plane->zoom += 2;
}

void window_pos_to_plane_pos(Complex_plane *plane, int x, int y, double *result_x, double *result_y)
{
    *result_x = ((((double)((x - plane->win_x / 2)) / 200) - plane->center_x) * (1 / plane->zoom));
    *result_y = ((((double)((y - plane->win_y / 2)) / 200) - plane->center_y) * (1 / plane->zoom));
}
matrix_square *render_matrix(Complex_plane *plane)
{

    double pos_x;
    double pos_y;
    Complex_sequence *u;
    Complex z;
    int count = 0;
    int bounded_count = 0;
    int i = 0;
    int j = 0;

    matrix_square *m = create_matrix(plane->win_x);
    for (double x = -2.0; i < m->d; x = x + 0.004)
    {
        for (double y = -2.0; j < m->d; y = y + 0.004)
        {

            // printf("being pos\n");
            window_pos_to_plane_pos(plane, x, y, &pos_x, &pos_y);
            // printf("end pos\n");

            set_re(&z, x);
            set_im(&z, y);
            // printf(" x = %.17f, y = %.17f\n", pos_x, pos_y);
            u = create_complex_sequence(&z, 2);
            if (is_bounded_v1(u, 30))
            {
                set_value_matrix(m, i, j, (char)(1));
                bounded_count++;
            }
            else
            {
                set_value_matrix(m, i, j, (char)(0));
            }

            // printf("end draw\n");
            // printf("\rPixel %d / %d, (%d, %d)", count, m->d * m->d, i, j);
            fflush(stdout);
            count++;
            j++;
        }
        j = 0;
        i++;
    }

    printf("\ndone, %d bounded \n", bounded_count);
    return m;
}

void render_window(SDL_Renderer *renderer, Complex_plane *plane)
{

    double pos_x;
    double pos_y;
    Complex_sequence *u;
    Complex z;
    int count = 0;
    for (int x = 0; x < plane->win_x; x++)
    {
        for (int y = 0; y < plane->win_y; y++)
        {

            // printf("being pos\n");
            window_pos_to_plane_pos(plane, x, y, &pos_x, &pos_y);
            // printf("end pos\n");

            set_re(&z, pos_x);
            set_im(&z, pos_y);
            // printf(" x = %.17f, y = %.17f\n", pos_x, pos_y);
            u = create_complex_sequence(&z, 2);
            if (is_bounded_v1(u, 30))
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            }
            // printf("being draw\n");
            SDL_RenderDrawPoint(renderer, x, y);
            // printf("end draw\n");
            // printf("\rPixel %d / %d, (%f, %f)", count, plane->win_x * plane->win_y, plane->center_x, plane->center_y);
            fflush(stdout);
            count++;
            // if (count > 10)
            // {
            //     return;
            // }
        }
    }
    SDL_RenderPresent(renderer);
    printf("done \n");
}

int runlive(int argc, char *argv[])
{
    double center_x = 0.0;
    double center_y = 0.0;
    for (int i = 0; i < argc; i++)
    {
        printf("%s\n", argv[i]);
        if (!strcmp(argv[i], "-center"))
        {
            center_x = atof(argv[i + 1]);
            center_y = atof(argv[i + 2]);
            i += 2;
        }
    }
    int win_x = 1000;
    int win_y = 1000;
    if (DEBUG)
    {
        debug_complex();
        return 0;
    }

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *fenetre = SDL_CreateWindow(
        "Visualizer",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        win_x, win_y,
        SDL_WINDOW_SHOWN);

    Complex_plane current_plane = {1.5, center_x, center_y, win_x, win_y};
    if (fenetre == NULL)

    {
        printf("cant create window\n");
        return 0;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    render_window(renderer, &current_plane);

    int close = 0;
    int test = 1;
    SDL_Event event;

    int mouse_pos_x;
    int mouse_pos_y;
    while (!close)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                close = 1;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                SDL_GetMouseState(&(mouse_pos_x), &(mouse_pos_y));
                set_center(&current_plane, mouse_pos_x, mouse_pos_y);
                render_window(renderer, &current_plane);
            }
        }

        SDL_Delay(1);
    }

    return 0;
}

Complex_plane *create_complex_plane(double zoom, double center_x, double center_y, int win_x, int win_y)
{
    Complex_plane *plane = malloc(sizeof(Complex_plane));
    plane->zoom = center_x;
    plane->center_x = center_x;
    plane->center_y = center_y;
    plane->win_x = win_x;
    plane->win_y = win_y;
    return plane;
}
int view_matrix_image(matrix_square *m)
{

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *fenetre = SDL_CreateWindow(
        "Visualizer",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        m->d, m->d,
        SDL_WINDOW_SHOWN);

    if (fenetre == NULL)

    {
        printf("cant create window\n");
        return 0;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    int close = 0;
    int test = 1;
    SDL_Event event;

    short int current_matrix_val;
    while (!close)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                close = 1;
            }
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        for (int i = 0; i < m->d; i++)
        {
            for (int j = 0; j < m->d; j++)
            {
                current_matrix_val = get_value_matrix(m, i, j);
                // printf("%hd", current_matrix_val);
                SDL_SetRenderDrawColor(renderer, color_r_list[current_matrix_val], color_g_list[current_matrix_val],
                                       color_b_list[current_matrix_val], 255);
                SDL_RenderDrawPoint(renderer, i, j);
            }
        }
        SDL_RenderPresent(renderer);
    }

    return 0;
}

void test_matrix()
{
    // double center_x = 0.0;
    // double center_y = 0.0;

    // int win_x = 1000;
    // int win_y = 1000;
    // Complex_plane *current_plane = create_complex_plane(1.5, center_x, center_y, win_x, win_y);
    // printf("center (%d, %d)\n", current_plane->win_x, current_plane->win_y);
    // matrix_square *m = render_matrix(current_plane);
    // write_matrix(m, "matrix_test");
    matrix_square *m2 = read_matrix("matrix_test");
    view_matrix_image(m2);
}

int main(int argc, char *argv[])
{
    // runlive(argc, argv);
    test_matrix();
    return EXIT_SUCCESS;
    // runlive(argc, argv);
}