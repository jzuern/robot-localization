#include <iostream>
#include <SDL.h>
#include <vector>
#include "defines.h"
#include "Polygon.h"



void initializeGrid(bool ** grid, int size_x, int size_y)
{

    for (int x = 0; x < size_x; x++)
    {
        for (int y = 0; y < size_y; y++)
        {
            grid[x][y] = false;
            if ((x > 4./5 * size_x) and (y > 4./5 * size_y))
            {
                grid[x][y] = true;
            }
        }
    }
}

void update_position(Polygon * poly, const Uint8 * state)
{
    if (state[SDL_SCANCODE_RIGHT])
    {
        printf("rotating right\n");
        poly->rotateRight();
    }


    if (state[SDL_SCANCODE_LEFT])
    {
        printf("rotating left\n");
        poly->rotateLeft();
    }


    if (state[SDL_SCANCODE_UP])
    {
        printf("moving forward\n");
        poly->moveForward();
    }


    if (state[SDL_SCANCODE_DOWN])
    {
        printf("moving backward\n");
        poly->moveBackward();
    }

}




int main() {

    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }


    SDL_Window *win = SDL_CreateWindow(window_title, x_start, y_start, WWIDTH, WHEIGHT, SDL_WINDOW_SHOWN);
    if (win == nullptr){
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr){
        SDL_DestroyWindow(win);
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }


    int size_x = WWIDTH / TILE_SIZE;
    int size_y = WHEIGHT / TILE_SIZE;

    bool ** grid = new bool*[size_x];
    for(int i = 0; i < size_x; ++i)
        grid[i] = new bool[size_y];

    initializeGrid(grid, size_x, size_y);


    int mouse_x;
    int mouse_y;

    // NEW

    SDL_Color color = {.r = 255, .g = 0, .b = 0, .a = 255 };
    std::vector<Point> vertices;

    vertices.push_back(Point(100, 100));
    vertices.push_back(Point(100, 200));
    vertices.push_back(Point(200, 200));
    vertices.push_back(Point(200, 100));

    Polygon poly = Polygon(vertices);


    printf("Move mouse to create level. Press return to exit Creation Mode and enter Play Mode!\n");

    // first rendering loop
    while (1) {

        //First clear the renderer
        SDL_RenderClear(ren);

        //Draw the texture
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);

        SDL_RenderClear(ren); // fill the scene with white

        poly.DrawFilledPolygon(color, ren);

        SDL_RenderPresent(ren);

        //Take a quick break after all that hard work
        SDL_Delay(50);

        SDL_PumpEvents();
        const Uint8 *state = SDL_GetKeyboardState(NULL);

        update_position(&poly, state);

        if (state[SDL_SCANCODE_RETURN])
        {
            printf("finished level creation!\n");
            break;
        }
    }


    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();



    return 0;
}