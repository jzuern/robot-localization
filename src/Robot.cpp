//
// Created by jannik on 04.09.18.
//

#include "Robot.h"
#include "SDL.h"
#include <random>


Robot::Robot(int x_start, int y_start, float orientation, int rad, SDL_Color col)
{
    // state properties
    pose.x = x_start;
    pose.y = y_start;
    pose.phi = orientation;

    velocity.v = 0.;
    velocity.phi = 0.;

    // static properties
    radius = rad;
    color = col;
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

    SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, color.a);


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
    int x1 = x0 + int(15*(cos(pose.phi)));
    int y1 = y0 + int(15*(sin(pose.phi)));


    SDL_RenderDrawLine(ren, x0, y0, x1, y1);


}


void Robot::move(const Uint8 * state, Eigen::VectorXf & control)
{
    // preallocate control vector
    control(0) = 0.f;
    control(1) = 0.f;


    if (state[SDL_SCANCODE_RIGHT])
    {

        printf("rotating right\n");
        rotateRight(control);
    }


    if (state[SDL_SCANCODE_LEFT])
    {
        printf("rotating left\n");
        rotateLeft(control);
    }


    if (state[SDL_SCANCODE_UP])
    {
        printf("moving forward\n");
        moveForward(control);
    }


    if (state[SDL_SCANCODE_DOWN])
    {
        printf("moving backward\n");
        moveBackward(control);
    }

}



void Robot::moveForward(Eigen::VectorXf & control) {

    velocity.v = 1*DT;

    control(0) = velocity.v;

    pose.x += velocity.v * cos(pose.phi);
    pose.y += velocity.v * sin(pose.phi);
}

void Robot::moveBackward(Eigen::VectorXf & control){

    velocity.v = - 1*DT;

    control(0) = velocity.v;

    pose.x += velocity.v * cos(pose.phi);
    pose.y += velocity.v * sin(pose.phi);
}


void Robot::rotateLeft(Eigen::VectorXf & control) {


    velocity.phi = - DT * 2*M_PI/360;

    control(1) = velocity.phi;

    pose.phi += velocity.phi;


}

void Robot::rotateRight(Eigen::VectorXf & control) {

    velocity.phi = DT * 2*M_PI/360;

    control(1) = velocity.phi;

    pose.phi += velocity.phi;
}


void Robot::setPose(float x, float y, float phi) {

    pose.x = x;
    pose.y = y;
    pose.phi = phi;
}



std::vector<Landmark> Robot::measureLandmarks(std::vector<Landmark> landmarks)
{
    std::vector<Landmark> measured_landmarks;

    uint32_t time_ui = uint32_t( time(NULL) );
    srand( time_ui );

    for (auto lm = landmarks.begin(); lm != landmarks.end(); ++lm)
    {
        // Define random generator with Gaussian distribution

        float rx = .001*static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        float ry = .001*static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

//        rx = 0.f;
//        ry = 0.f;

        float est_pos_x = rx + lm->pos.x;
        float est_pos_y = ry + lm->pos.y;

        measured_landmarks.push_back(Landmark(est_pos_x,est_pos_y,lm->id));
    }

    return measured_landmarks;

}



Eigen::VectorXf Robot::get_state()
{
    Eigen::VectorXf state(3);

    // without PHI
    state(0) = this->pose.x;
    state(1) = this->pose.y;
    state(2) = this->pose.phi;


    return state;
}
