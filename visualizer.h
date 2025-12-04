#include <SDL2/SDL.h>

struct Complex_plane
{
    double zoom;
    double center_x;
    double center_y;
    int win_x;
    int win_y;
};

struct Frame
{
    double x_start;
    double y_start;
    double x_step;
    double y_step;
};
typedef struct Frame Frame;
typedef struct Complex_plane Complex_plane;
void render_window(SDL_Renderer *renderer, Complex_plane *plane);
void window_pos_to_plane_pos(Complex_plane *plane, int x, int y, double *result_x, double *result_y);
int runlive(int argc, char *argv[]);
void command_line(int argc, char *argv[]);

#define MAX_MATRIX_VAL 4

int color_r_list[] = {255, 0, 0, 0, 0, 0, 0, 0, 0};
int color_g_list[] = {255, 0, 0, 0, 0, 0, 0, 0, 0};
int color_b_list[] = {255, 200, 125, 85, 63, 31, 15, 8, 0};