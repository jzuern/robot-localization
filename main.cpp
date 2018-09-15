#include <iostream>
#include <SDL.h>
#include <vector>
#include "Eigen/Dense"

#include "Robot.h"
#include "Landmark.h"
#include "KalmanFilter.h"


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


    SDL_Window *win = SDL_CreateWindow("Robot Program", XSTART, YSTART, WWIDTH, WHEIGHT, SDL_WINDOW_SHOWN);
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


    std::vector<Landmark> landmarks = createLandmarks();

    SDL_Color orange {.r=255, .g=165, .b=0, .a=255};
    SDL_Color red {.r=255, .g=0, .b=0, .a=255};
    SDL_Color gray {.r=128, .g=128, .b=128, .a=255};


    Robot robby(200, 200, 2.0, 20, orange);
    Robot robby_estimate(200, 200, 1.0, 20, red);

    // Kalman filter stuff

    int n = 5; // number of state variables
    int m = 2; // number of measurements


    Eigen::MatrixXf A(n, n); // System dynamics matrix
    Eigen::MatrixXf C(m, n); // Output matrix
    Eigen::MatrixXf Q(n, n); // Process noise covariance
    Eigen::MatrixXf R(m, m); // Measurement noise covariance
    Eigen::MatrixXf P(n, n); // Estimate error covariance


    // Best guess of initial states
    Eigen::VectorXf x0(n);
    x0 << 0.0, 0.0, 0.0, 0.0, 0.0;

    A <<    1., 0., 0., 0., 0.,
            0., 1., 0., 0., 0.,
            DT, 0., 1., 0., 0.,
            0., DT, 0., 1., 0.,
            0., 0., 0., 0., 0.;

//
    C <<    1., 0., 0., 0., 0.,
            0., 1., 0., 0., 0.;

    // Reasonable covariance matrices
    Q <<    100., .0, .0, .0, 0.,
            .0, 100., .0, .0, 0.,
            .0, .0, 10., .0, 0.,
            .0, .0, .0, 10., 0.,
            .0, .0, .0, 0., 10.;


    R <<    15.0, 0.0,
            0.0, 15.0;


    P <<    .1, .1, .1, .1, .1,
            .1, .1, .1, .1, .1,
            .1, .1, .1, .1, .1,
            .1, .1, .1, .1, .1,
            .1, .1, .1, .1, .1;


    KalmanFilter kf(DT, A, C, Q, R, P);


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

        // OPTION A: Localize directly

        Eigen::VectorXf state = robby.get_state();
        kf.update(state);
        auto x_hat = kf.state();

        robby_estimate.setPose(x_hat(0), x_hat(1), 0.0);
        robby_estimate.render(ren);




        // OPTION B: Localize via Landmarks
        kf.localization_landmarks(landmarks);

        SDL_SetRenderDrawColor(ren, gray.r, gray.g, gray.b, gray.a);
        kf.renderSamples(ren);


        SDL_RenderPresent(ren);


        //Take a quick break after all that hard work
        SDL_Delay(30);

        SDL_PumpEvents();
        const Uint8 *key_pressed = SDL_GetKeyboardState(NULL);

        robby.move(key_pressed);



        if (key_pressed[SDL_SCANCODE_RETURN])
        {
            printf("Exiting game!\n");
            break;
        }
    }


    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();



    return 0;
}