//
// Created by jannik on 04.09.18.
//

#include "Robot.h"
#include "SDL.h"
#include <random>


Robot::Robot(int x_start, int y_start, float orientation, int rad)
{
    pose.x = x_start;
    pose.y = y_start;
    pose.phi = orientation;
    radius = rad;
}

Robot::~Robot()
{

}


void Robot::render(SDL_Renderer * ren)
{
    int32_t x = radius - 1;
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t err = tx - (radius << 1); // shifting bits left by 1 effectively
    // doubles the value. == tx - diameter

    int32_t int_pose_x = int(pose.x);
    int32_t int_pose_y = int(pose.y);


    while (x >= y)
    {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(ren, int_pose_x + x, int_pose_y - y);
        SDL_RenderDrawPoint(ren, int_pose_x + x, int_pose_y + y);
        SDL_RenderDrawPoint(ren, int_pose_x - x, int_pose_y - y);
        SDL_RenderDrawPoint(ren, int_pose_x - x, int_pose_y + y);
        SDL_RenderDrawPoint(ren, int_pose_x + y, int_pose_y - x);
        SDL_RenderDrawPoint(ren, int_pose_x + y, int_pose_y + x);
        SDL_RenderDrawPoint(ren, int_pose_x - y, int_pose_y - x);
        SDL_RenderDrawPoint(ren, int_pose_x - y, int_pose_y + x);

        if (err <= 0)
        {
            y++;
            err += ty;
            ty += 2;
        }
        if (err > 0)
        {
            x--;
            tx += 2;
            err += tx - (radius << 1);
        }
    }

    int x0 = int_pose_x;
    int y0 = int_pose_y;
    int x1 = x0 + int(20*(cos(pose.phi) - sin(pose.phi)));
    int y1 = y0 + int(20*(sin(pose.phi) + cos(pose.phi)));

    printf("line: x0 = %i, x1 = %i,y0 = %i,y1 = %i\n", x0,x1,y0,y1);

    SDL_RenderDrawLine(ren, x0, y0, x1, y1);


}


void Robot::move(const Uint8 * state)
{
    if (state[SDL_SCANCODE_RIGHT])
    {
        printf("rotating right\n");
        rotateRight();
    }


    if (state[SDL_SCANCODE_LEFT])
    {
        printf("rotating left\n");
        rotateLeft();
    }


    if (state[SDL_SCANCODE_UP])
    {
        printf("moving forward\n");
        moveForward();
    }


    if (state[SDL_SCANCODE_DOWN])
    {
        printf("moving backward\n");
        moveBackward();
    }

}



void Robot::moveForward() {

    float scale = 2.0;

    pose.x += scale  * (cos(pose.phi) - sin(pose.phi));
    pose.y += scale * (sin(pose.phi) + cos(pose.phi));
    printf("pose.x = %f, pose.y = %f\n", pose.x, pose.y);
}

void Robot::moveBackward(){

    float scale = 2.0;

    pose.x -= scale  * (cos(pose.phi) - sin(pose.phi));
    pose.y -= scale * (sin(pose.phi) + cos(pose.phi));
    printf("pose.x = %f, pose.y = %f\n", pose.x, pose.y);
}


void Robot::rotateLeft() {

    float scale = 4.0;

    float diff = - scale * 2*M_PI/360;
    pose.phi += diff;
    printf("pose.phi = %f\n", pose.phi);

}

void Robot::rotateRight() {

    float scale = 4.0;

    float diff = scale * 2*M_PI/360;
    pose.phi += diff;
    printf("pose.phi = %f\n", pose.phi);

}



std::vector<Landmark> Robot::measureLandmarks(std::vector<Landmark> landmarks)
{
    std::vector<Landmark> measured_landmarks;

    uint32_t time_ui = uint32_t( time(NULL) );
    srand( time_ui );

    for (auto lm = landmarks.begin(); lm != landmarks.end(); ++lm)
    {
        // Define random generator with Gaussian distribution

        float rx = 5.0*static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        float ry = 5.0*static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

        float est_pos_x = rx + lm->pos.x;
        float est_pos_y = ry + lm->pos.y;

//        printf("measured lm position: (%f, %f) \n", est_pos_x, est_pos_y);
//        printf("actual lm position: (%f, %f) \n", mean_x, mean_y);

        measured_landmarks.push_back(Landmark(est_pos_x,est_pos_y,lm->id));
    }

    return measured_landmarks;

}
