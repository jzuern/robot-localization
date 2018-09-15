//
// Created by jannik on 06.09.18.
//

#ifndef UNTITLED1_LANDMARK_H
#define UNTITLED1_LANDMARK_H

#include <SDL.h>


typedef struct Position
{
    float x, y;
} Position;



class Landmark {
public:
    Landmark(float x, float y, SDL_Color id);
    ~Landmark();
    Position pos;
    SDL_Color id;
    void render(SDL_Renderer * ren);
};


#endif //UNTITLED1_LANDMARK_H
