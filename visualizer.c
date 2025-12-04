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
matrix_square *render_matrix(Frame *frame, int precision)
{

    double pos_x;
    double pos_y;
    Complex_sequence *u;
    Complex z;
    int count = 0;
    int bounded_count = 0;
    int i = 0;
    int j = 0;

    matrix_square *m = create_matrix(1000);
    for (double x = frame->x_start; i < m->d; x = x + frame->x_step)
    {
        for (double y = frame->y_start; j < m->d; y = y + frame->y_step)
        {

            // printf("being pos\n");
            // printf("end pos\n");

            set_re(&z, x);
            set_im(&z, y);
            // printf(" x = %.17f, y = %.17f\n", pos_x, pos_y);
            u = create_complex_sequence(&z, 2);

            set_value_matrix(m, i, j, (char)(is_bounded_v2(u, precision, MAX_MATRIX_VAL)));
            // printf("end draw\n");
            // printf("\rPixel %d / %d, (%d, %d)", count, m->d * m->d, i, j);
            fflush(stdout);
            count++;
            j++;
        }
        j = 0;
        i++;
    }

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
            if (is_bounded_v1(u, 5, MAX_MATRIX_VAL))
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

int view_matrix_array(matrix_square **m, int size)
{

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *fenetre = SDL_CreateWindow(
        "Visualizer",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1000, 1000,
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
    matrix_square *current_matrix = m[0];
    int current_i = 0;
    while (!close)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                close = 1;
            }
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.scancode == SDL_SCANCODE_LEFT && current_i > 0)
                {
                    current_i--;
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT && current_i < size - 1)
                {
                    current_i++;
                }
            }
        }
        current_matrix = m[current_i];
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        for (int i = 0; i < current_matrix->d; i++)
        {
            for (int j = 0; j < current_matrix->d; j++)
            {
                current_matrix_val = get_value_matrix(current_matrix, i, j);
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

matrix_square **test_zoom(int size)
{
    Frame *f = malloc(sizeof(Frame));
    f->x_start = -2.5;
    f->y_start = -2.0;
    f->x_step = 0.004;
    f->y_step = 0.004;
    matrix_square **array = malloc(sizeof(matrix_square *) * size);
    for (int i = 0; i < size; i++)
    {
        array[i] = render_matrix(f, 50);
        f->x_start += 0.5;
        f->y_start += 0.5;
        f->x_step += 0.001;
        f->y_step += 0.001;
        printf("\rMatrix %d / %d", i + 1, size);
        fflush(stdout);
    }
    write_matrix_array(array, "test", size);
    return array;
}

matrix_square **render_zoom_to_point(int nb_frames, int precision, Complex *center)
{
    Complex *up_left = malloc(sizeof(Complex));
    set_re(up_left, -2.5);
    set_im(up_left, -2.0);
    Frame *f = malloc(sizeof(Frame));
    double width = 4;
    double width_div = 1;
    f->x_start = get_re(up_left);
    f->y_start = get_im(up_left);
    f->x_step = width / 1000;
    f->y_step = width / 1000;
    Complex *center_to_upleft = malloc(sizeof(Complex));

    matrix_square **array = malloc(sizeof(matrix_square *) * nb_frames);
    for (int i = 0; i < nb_frames; i++)
    {
        array[i] = render_matrix(f, precision);
        set_re(center_to_upleft, get_re(up_left) - get_re(center));
        set_im(center_to_upleft, get_im(up_left) - get_im(center));

        set_re(up_left, get_re(up_left) + (get_re(center_to_upleft) * -0.2));
        set_im(up_left, get_im(up_left) + (get_im(center_to_upleft) * -0.2));
        f->x_start = get_re(up_left);
        f->y_start = get_im(up_left);
        width *= 0.8;
        f->x_step = width / 1000;
        f->y_step = width / 1000;

        printf("\rImage : %d / %d upleft : (%lf, %lf)", i + 1, nb_frames, get_re(up_left), get_im(up_left));
        fflush(stdout);
    }
    printf("\n");
    return array;
}

int main(int argc, char *argv[])
{
    command_line(argc, argv);
    return EXIT_SUCCESS;
    // runlive(argc, argv);
}

void command_line(int argc, char *argv[])
{
    if (argc > 1)
    {

        if (!strcmp(argv[1], "open"))
        {
            if (argc < 3)
            {
                printf("Too few arguments\n");
            }
            else
            {
                matrix_square *m = read_matrix(argv[2]);
                view_matrix_image(m);
            }
        }
        if (!strcmp(argv[1], "render-image"))
        {
            if (argc < 3)
            {
                printf("Too few arguments\n");
            }
            else
            {
                double width = 4;
                Frame *f = malloc(sizeof(Frame));
                f->x_start = -2.5;
                f->y_start = -2;
                f->x_step = width / 1000;
                f->y_step = width / 1000;

                matrix_square *m = render_matrix(f, 50);
                write_matrix(m, argv[2]);
            }
        }
        if (!strcmp(argv[1], "render-zoom"))
        {
            if (argc < 3)
            {
                printf("Too few arguments\n");
            }
            else
            {
                int frame_amount = 30;
                int precision = 50;
                Complex c = {0.005, 0.75};
                matrix_square **m = render_zoom_to_point(frame_amount, precision, &c);
                write_matrix_array(m, argv[2], frame_amount);
            }
        }
        if (!strcmp(argv[1], "open-movie"))
        {
            if (argc < 3)
            {
                printf("Too few arguments\n");
            }
            else
            {
                int size;
                matrix_square **m = read_matrix_array(argv[2], &size);
                view_matrix_array(m, size);
            }
        }
    }
}