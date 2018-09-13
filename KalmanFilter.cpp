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

void KalmanFilter::renderSamples(SDL_Renderer * ren)
{
    float var_x = P(0,0);
    float var_y = P(1,1);

    float mean_x = x_hat(0);
    float mean_y = x_hat(1);


    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    std::default_random_engine generator(seed);
    std::normal_distribution<float> dist_x(mean_x, var_x);
    std::normal_distribution<float> dist_y(mean_y, var_y);

    int nSamples = 100;
    for (int i = 0; i < nSamples; i++)
    {
        float pos_x = dist_x(generator);
        float pos_y = dist_y(generator);


        SDL_RenderDrawPoint(ren, pos_x, pos_y);

    }


}


