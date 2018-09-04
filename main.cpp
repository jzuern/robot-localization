#include <iostream>
#include <SDL.h>
#include <vector>
#include "defines.h"




void initializeGrid()
{




}
void update_position(SDL_Rect * rect, const Uint8 * state)
{

    if (state[SDL_SCANCODE_RIGHT])
    {
        rect->x += 1;
    }

    if (rect->x > width)
    {
        rect->x = width;
    }

    if (state[SDL_SCANCODE_LEFT])
    {
        rect->x -= 1;
    }

    if (rect->x < 0)
    {
        rect->x = 0;
    }


    if (state[SDL_SCANCODE_UP])
    {
        rect->y -= 1;
    }

    if (rect->y > height)
    {
        rect->y = height;
    }

    if (state[SDL_SCANCODE_DOWN])
    {
        rect->y += 1;
    }

    if (rect->y < 0)
    {
        rect->y = 0;
    }
}


SDL_Point * generateShape(const int points_count)
{


}




int main() {

    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }


    SDL_Window *win = SDL_CreateWindow(window_title, x_start, y_start, width, height, SDL_WINDOW_SHOWN);
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

    int points_count = 10000;
    SDL_Point * points = generateShape(points_count);



    int mouse_x;
    int mouse_y;

    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.h = 100;
    rect.w = 100;

    initializeGrid();

    // first rendering loop
    while (1) {

        printf("Move mouse to create level!\n");

        //First clear the renderer
        SDL_RenderClear(ren);

        //Draw the texture
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);

        SDL_RenderClear(ren); // fill the scene with white

        SDL_SetRenderDrawColor(ren, 255, 0, 0, 255); // the rect color (solid red)
        SDL_RenderFillRect(ren, &rect);


//        SDL_RenderDrawPoints(ren, points, points_count);

        //Update the screen
        SDL_RenderPresent(ren);

        //Take a quick break after all that hard work
        SDL_Delay(20);

        SDL_PumpEvents();
        if (SDL_GetMouseState(&mouse_x, &mouse_y) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
            SDL_Log("Mouse Button 1 (left) is pressed.");
            printf("Coordinates: x = %i, y = %i\n", mouse_x, mouse_y);
        }

        const Uint8 *state = SDL_GetKeyboardState(NULL);

        if (state[SDL_SCANCODE_RETURN])
        {
            printf("finished level creation!\n");
            break;
        }
    }

    // second rendering loop
    while (1) {

        //First clear the renderer
        SDL_RenderClear(ren);

        //Draw the texture
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);

        SDL_RenderClear(ren); // fill the scene with white

        SDL_SetRenderDrawColor(ren, 255, 0, 0, 255); // the rect color (solid red)
        SDL_RenderFillRect(ren, &rect);

        //Update the screen
        SDL_RenderPresent(ren);

        //Take a quick break after all that hard work
        SDL_Delay(20);

        SDL_PumpEvents();
        if (SDL_GetMouseState(&mouse_x, &mouse_y) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
            SDL_Log("Mouse Button 1 (left) is pressed.");
            printf("Coordinates: x = %i, y = %i\n", mouse_x, mouse_y);
        }

        const Uint8 *state = SDL_GetKeyboardState(NULL);
        update_position(&rect, state);
    }





    return 0;
}