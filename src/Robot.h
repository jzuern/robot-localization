//
// Created by jannik on 04.09.18.
//

#ifndef UNTITLED1_ROBOT_H
#define UNTITLED1_ROBOT_H

#include <SDL_render.h>
#include "Landmark.h"
#include <vector>
#include <Eigen/Dense>
#include "defines.h"


typedef struct Pose
{
    float x, y;
    float phi;
} Pose;



typedef struct Velocity
{
    float v;
    float phi;
} Velocity;


class Robot {
public:
    Robot(int x_start, int y_start, float orientation, int radius, SDL_Color col);
    ~Robot();
    Pose pose;
    Velocity velocity;
    SDL_Color color;

    int radius;
    void render(SDL_Renderer * ren);

    void move(const Uint8 * , Eigen::VectorXf & control);

    void moveForward(Eigen::VectorXf & control);
    void moveBackward(Eigen::VectorXf & control);
    void rotateLeft(Eigen::VectorXf & control);
    void rotateRight(Eigen::VectorXf & control);
    void setPose(float x, float y, float phi);

    Eigen::VectorXf get_state();

    std::vector<Landmark> measureLandmarks(std::vector<Landmark> landmarks);

private:


};


#endif //UNTITLED1_ROBOT_H
