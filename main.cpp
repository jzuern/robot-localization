#include <iostream>
#include <SDL.h>
#include <vector>
#include "defines.h"
#include "Robot.h"
#include "Landmark.h"
#include "KalmanFilter.h"
#include "Eigen/Dense"


std::vector<Landmark> createLandmarks()
{
    std::vector<Landmark> lmks;

    SDL_Color color_red = {.r = 255, .g = 0, .b = 0, .a = 255 };
    SDL_Color color_green = {.r = 0, .g = 255, .b = 0, .a = 255 };
    SDL_Color color_blue = {.r = 0, .g = 0, .b = 255, .a = 255 };

    lmks.push_back( Landmark(100.,100.,color_red));
    lmks.push_back( Landmark(250.,50.,color_green));
    lmks.push_back( Landmark(350.,300.,color_blue));

    return lmks;
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



    std::vector<Landmark> landmarks = createLandmarks();

    SDL_Color orange {.r=255, .g=165, .b=0, .a=255};
    SDL_Color red {.r=255, .g=0, .b=0, .a=255};

    Robot robby(200, 200, 0.0, 20, orange);
    Robot robby_estimate(200, 200, 0.0, 20, red);

    // Kalman filter stuff

    double dt = 1.0/30; // Time step
    int n = 6; // number of state variables
    int m = 3; // number of measurements


    Eigen::MatrixXf A(n, n); // System dynamics matrix
    Eigen::MatrixXf C(m, n); // Output matrix
    Eigen::MatrixXf Q(n, n); // Process noise covariance
    Eigen::MatrixXf R(m, m); // Measurement noise covariance
    Eigen::MatrixXf P(n, n); // Estimate error covariance

    A <<    1, 0, 0, dt, 0, 0,
            0, 1, 0, 0, dt, 0,
            0, 0, 1, 0, 0, dt,
            0, 0, 0 , 1, 0, 0,
            0, 0, 0, 0, 1, 0,
            0, 0, 0, 0, 0, 1,

    C <<    1, 0, 0, 0, 0, 0,
            0, 1, 0, 0, 0, 0,
            0, 0, 1, 0, 0, 0;


    // Reasonable covariance matrices
    Q <<    .05, .05, .05, .0, .0, .0,
            .05, .05, .05, .0, .0, .0,
            .05, .05, .05, .0, .0, .0,
            .0, .0, .0, .0, .0 ,.0,
            .0, .0, .0, .0, .0, .0,
            .0, .0, .0, .0, .0, .0;


    R <<    5.0, 0.0, 0.0,
            0.0, 5.0, 0.0,
            0.0, 0.0, 5.0;


    P <<    .1, .1, .1, .1, .1, .1,
            .1, .1, 10, 10, .1, .1,
            .1, 10, 10, 10, .1, .1,
            .1, 10, 10, 10, .1, .1,
            .1, 10, 10, 10, .1, .1,
            .1, 10, 10, 10, 0.1, 0.1;


    KalmanFilter kf(dt, A, C, Q, R, P);


    // Best guess of initial states
    Eigen::VectorXf x0(n);
    x0 << 200, 200, 0.0, 0., 0., 0.0;
    float t0 = 0.0;
    kf.init(t0, x0);


    // rendering loop
    while (1) {

        //First clear the renderer
        SDL_RenderClear(ren);

        //Draw the texture
        SDL_SetRenderDrawColor(ren, 200, 200, 255, 255);

        SDL_RenderClear(ren); // fill the scene with white

        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);

        // render robot
        robby.render(ren);

        // render landmarks
        for (auto lm = landmarks.begin(); lm != landmarks.end(); ++lm)
        {
            SDL_SetRenderDrawColor(ren, lm->id.r, lm->id.g, lm->id.b, lm->id.a);
            lm->render(ren);
        }
        // update renderer
        SDL_RenderPresent(ren);

        // measure landmark positions
        auto measured_landmarks = robby.measureLandmarks(landmarks);

        // TODO: change from measured absolute position to measured landmarks
        Eigen::VectorXf y = robby.get_state();
        Eigen::VectorXf y_new(3);

        y_new << y(0), y(1), y(2);

        kf.update(y_new);
        auto x_hat = kf.state();


        printf("KF:: x = (%f, %f, %f)\n", y_new(0), y_new(1), y_new(2));
        printf("KF:: x_hat = (%f, %f, %f)\n", x_hat(0), x_hat(1), x_hat(2));

        robby_estimate.setPose(x_hat(0), x_hat(1), x_hat(2));
        robby_estimate.render(ren);


        // visualize measured landmarks
//        for (auto lm = measured_landmarks.begin(); lm != measured_landmarks.end(); ++lm)
//        {
//            SDL_SetRenderDrawColor(ren, lm->id.r, lm->id.g, lm->id.b, lm->id.a);
//            lm->render(ren);
//        }

        SDL_RenderPresent(ren);




        //Take a quick break after all that hard work
        SDL_Delay(50);

        SDL_PumpEvents();
        const Uint8 *state = SDL_GetKeyboardState(NULL);

        robby.move(state);





        if (state[SDL_SCANCODE_RETURN])
        {
            printf("Finished level creation!\n");
            break;
        }
    }


    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();



    return 0;
}