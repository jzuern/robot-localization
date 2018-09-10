//
// Created by jannik on 04.09.18.
//

#include "KalmanFilter.h"


KalmanFilter::KalmanFilter() {

    float dt = 0.1;

    // initialize state vector


    // initialize system matrix
    A << 1, 0, dt, 0,
         0, 1, 0, dt,
         0, 0, 1, 0,
         0, 0, 0, 1;



}

bool KalmanFilter::predict() {

    Eigen::MatrixXd A;
    Eigen::VectorXd mu_est = A * mu + B * u;



    // returns mu_t and covariance_t
    return true;
}


bool KalmanFilter::update() {

    return true;
}