//
// Created by jannik on 04.09.18.
//

#ifndef UNTITLED1_KALMANFILTER_H
#define UNTITLED1_KALMANFILTER_H

#include <Eigen/Dense>
#include <SDL_render.h>
#include <random>
#include "Landmark.h"
#include <math.h>       /* atan2 */

#include <chrono>
#include <random>

class KalmanFilter {
public:


    /**
    * Create a Kalman filter with the specified matrices.
    *   A - System dynamics matrix
    *   C - Output matrix
    *   Q - Process noise covariance
    *   R - Measurement noise covariance
    *   P - Estimate error covariance
    */


    KalmanFilter(
            double dt,
            const Eigen::MatrixXf& A,
            const Eigen::MatrixXf& C,
            const Eigen::MatrixXf& Q,
            const Eigen::MatrixXf& R,
            const Eigen::MatrixXf& P
    );

    /**
    * Initialize the filter with a guess for initial states.
    */
    void init(double t0, const Eigen::VectorXf& x0);

    /**
    * Update the estimated state based on measured values. The
    * time step is assumed to remain constant.
    */
    void update(const Eigen::VectorXf& y);

    /**
    * Return the current state and time.
    */
    Eigen::VectorXf state() { return x_hat; };

    double time() { return t; };


    void renderSamples(SDL_Renderer * ren);

    void localization_landmarks(const std::vector<Landmark> & landmarks);

private:

    // Matrices for computation
    Eigen::MatrixXf A, C, Q, R, P, K, P0;

    // System dimensions
    int m, n;

    // Initial and current time
    double t0, t;

    // Discrete time step
    double dt;

    // Is the filter initialized?
    bool initialized;

    // n-size identity
    Eigen::MatrixXf I;

    // Estimated states
    Eigen::VectorXf x_hat, x_hat_new;
};


#endif //UNTITLED1_KALMANFILTER_H
