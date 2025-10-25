#include <SDL2/SDL.h>
struct Complex_plane
{
    double zoom;
    double center_x;
    double center_y;
    int win_x;
    int win_y;
};
typedef struct Complex_plane Complex_plane;
void render_window(SDL_Renderer *renderer, Complex_plane *plane);
void window_pos_to_plane_pos(Complex_plane *plane, int x, int y, double *result_x, double *result_y);