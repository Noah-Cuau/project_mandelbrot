#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "visualizer.h"
#include "write_movie.h"
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
void render_window(SDL_Renderer *renderer, Complex_plane *plane)
{

    double pos_x;
    double pos_y;
    Complex_sequence *u;
    Complex z;
    for (int x = 0; x < plane->win_x; x++)
    {
        for (int y = 0; y < plane->win_y; y++)
        {

            window_pos_to_plane_pos(plane, x, y, &pos_x, &pos_y);
            set_re(&z, pos_x);
            set_im(&z, pos_y);

            u = create_complex_sequence(&z, 5);
            if (is_bounded_v1(u, 30))

            {

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            }
            SDL_RenderDrawPoint(renderer, x, y);
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

int main(int argc, char *argv[])
{
    runlive(argc, argv);
    // write_image("filetest", NULL, 1280, 780);
}