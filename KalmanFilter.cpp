//
// Created by jannik on 04.09.18.
//

#include "KalmanFilter.h"
#include "Robot.h"


KalmanFilter::KalmanFilter(

    double dt,
    const Eigen::MatrixXf& A,
    const Eigen::MatrixXf& C,
    const Eigen::MatrixXf& Q,
    const Eigen::MatrixXf& R,
    const Eigen::MatrixXf& P)
    : A(A), C(C), Q(Q), R(R), P0(P),
      m(C.rows()), n(A.rows()), dt(dt), initialized(false),
      I(n, n), x_hat(n), x_hat_new(n)
{
    I.setIdentity();
}

void KalmanFilter::init(double t0, const Eigen::VectorXf& x0) {
    x_hat = x0;
    P = P0;
    this->t0 = t0;
    t = t0;
    initialized = true;
}

void KalmanFilter::update(const Eigen::VectorXf& y) {

    if(!initialized)
        throw std::runtime_error("Filter is not initialized!");

    x_hat_new = A * x_hat;
    P = A * P * A.transpose() + Q;
    K = P * C.transpose() * (C * P * C.transpose() + R).inverse();

    auto y_hat = C * x_hat_new;
    x_hat_new += K * (y - y_hat);
    P = (I - K*C)*P;
    x_hat = x_hat_new;

    t += dt;
}

