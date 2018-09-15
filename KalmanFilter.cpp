//
// Created by jannik on 04.09.18.
//

#include "KalmanFilter.h"
#include "Robot.h"
#include <math.h>       /* atan2 */
#include <iostream>


KalmanFilter::KalmanFilter(

    double dt,
    const Eigen::MatrixXf& A,
    const Eigen::MatrixXf& C,
    const Eigen::MatrixXf& Q,
    const Eigen::MatrixXf& R,
    const Eigen::MatrixXf& covariance)
    : A(A), C(C), Q(Q), R(R), P0(covariance),
      m(C.rows()), n(A.rows()), dt(dt), initialized(false),
      I(n, n), state(n), state_new(n)
{
    I.setIdentity();
}

void KalmanFilter::init(double t0, const Eigen::VectorXf& x0) {
    state = x0;
    covariance = P0;
    this->t0 = t0;
    t = t0;
    initialized = true;
}

void KalmanFilter::update(const Eigen::VectorXf& y) {

    if(!initialized)
        throw std::runtime_error("Filter is not initialized!");

    state_new = A * state;
    covariance = A * covariance * A.transpose() + Q;
    K = covariance * C.transpose() * (C * covariance * C.transpose() + R).inverse();

    auto y_hat = C * state_new;
    state_new += K * (y - y_hat);
    covariance = (I - K*C)*covariance;
    state = state_new;

    t += dt;
}



void KalmanFilter::localization_landmarks(const std::vector<Landmark> & observed_landmarks,
                                          const std::vector<Landmark> & true_landmarks,
                                          const Eigen::VectorXf & control)
{

    /* INFO:
     * Formel mu = 'state'
     * Formel sigma = 'P'
     * Formel R = 'Q'
     * Formel u = (not used?)
     * Formel z = 'y'
     * Formel c = ?
     *
     * x_hat(0) = x
     * x_hat(1) = y
     * x_hat(2) = phi
     *
     */

    std::cout << "KF:: state = \n" << state << std::endl;
    std::cout << "KF:: control = \n" << control << std::endl;



    float v = control(0);
    float omega = control(1);

    float phi = state(2);
    float dx;
    float dy;

    if (omega > 1E-3) // robot moves along circle with radius v/omega
    {
        dx = v / omega * (- sin(phi) + sin(phi + omega*DT));
        dy = v / omega * (  cos(phi) - cos(phi + omega*DT));
    }
    else
    {
        dx = v*DT*cos(phi);
        dy = v*DT*sin(phi);
    }


    // 1 - update state estimate
    Eigen::VectorXf state_new(3);
    state_new(0) = state(0) + dx;
    state_new(1) = state(1) + dy;
    state_new(2) = state(2) + omega * DT;

    std::cout << "KF:: state_new = \n" << state_new << std::endl;


    // Define Jacobian

    float entry_13;
    float entry_23;

    if (omega > 1E-3) // robot moves along circle with radius v/omega
    {
        entry_13 = v / omega * (- sin(phi) + sin(phi + omega*DT));
        entry_23 = v / omega * (  cos(phi) - cos(phi + omega*DT));
    }
    else
    {
        entry_13 = v*DT*sin(phi);
        entry_23 = -v*DT*cos(phi);
    }


    Eigen::MatrixXf G(3,3);

    G <<    1, 0, entry_13,
            0, 1, entry_23,
            0, 0, 1;

//    std::cout << "KF:: covariance old = \n" << covariance << std::endl;


    covariance = G * covariance * G.transpose() + Q;


    std::cout << "KF:: G = \n" << G << std::endl;
    std::cout << "KF:: Q = \n" << Q << std::endl;
    std::cout << "KF:: R = \n" << R << std::endl;


    std::cout << "KF:: covariance new = \n" << covariance << std::endl;


    // covariances of landmark measurements
    float sigma_r = 0.1f;
    float sigma_phi = 0.1f;
    float sigma_s = 0.1f;

    R <<    sigma_r, 0, 0,
            0, sigma_phi, 0,
            0, 0, sigma_s;


    Eigen::VectorXf sum1(3);
    Eigen::MatrixXf sum2(3,3);

    sum1 << 0.f, 0.f, 0.f;
    sum2 << 0.f, 0.f, 0.f,
            0.f, 0.f, 0.f,
            0.f, 0.f, 0.f;

    int nLandmarks = true_landmarks.size();


    for (int i = 0; i < nLandmarks; i++)
    {
        auto true_lm = true_landmarks[i];
        auto observed_lm = observed_landmarks[i];


        // OBSERVED LANDMARKS (fake landmark observation)
        Eigen::VectorXf delta_(2);
        delta_(0) = observed_lm.pos.x - state_new(0);
        delta_(1) = observed_lm.pos.y - state_new(1);

        auto q_ = delta_.dot(delta_);

        Eigen::VectorXf z_i(3);
        z_i(0) = sqrt(q_);
        z_i(1) = atan2(delta_(1),delta_(0)) - state_new(2);
        z_i(2) = i;

        // TRUE LANDMARKS
        Eigen::VectorXf delta(2);
        delta(0) = true_lm.pos.x - state_new(0);
        delta(1) = true_lm.pos.y - state_new(1);

        auto q = delta.dot(delta);

        Eigen::VectorXf z_hat_i(3);
        z_hat_i(0) = sqrt(q);
        z_hat_i(1) = atan2(delta(1),delta(0)) - state_new(2);
        z_hat_i(2) = i;

        // END

        Eigen::MatrixXf H_i(3,3);
        H_i(0,0) =  1/q * sqrt(q)*delta(0);
        H_i(0,1) = -1/q * sqrt(q)*delta(1);
        H_i(0,2) = 0;
        H_i(1,0) = 1/q * delta(1);
        H_i(1,1) = 1/q * delta(0);
        H_i(1,2) = -1/q;
        H_i(2,0) = 0.;
        H_i(2,1) = 0.;
        H_i(2,2) = 0.;

        auto temporal = H_i * covariance * H_i.transpose() + R;

        std::cout << "KF:: temporal = \n" << temporal << std::endl;
        std::cout << "KF:: temporal.inverse = \n" << temporal.inverse() << std::endl;


        auto K_i = covariance * H_i.transpose() * (temporal).inverse();

        std::cout << "KF:: H_i = \n" << H_i << std::endl;
        std::cout << "KF:: K_i = \n" << K_i << std::endl;
        std::cout << "KF:: z_hat_i = \n" << z_hat_i << std::endl;
        std::cout << "KF:: z_i = \n" << z_i << std::endl;


        sum1 += K_i * (z_i - z_hat_i);
        sum2 += K_i * H_i;

    }

    state_new += sum1;
    covariance = (I - sum2)*covariance;

    state = state_new;

    std::cout << "KF:: final state_new = \n" << state << std::endl;
    std::cout << "KF:: final covariance = \n" << covariance << std::endl;

    int a = 0;
}




void KalmanFilter::renderSamples(SDL_Renderer * ren)
{
    float var_x = covariance(0,0);
    float var_y = covariance(1,1);

    float mean_x = state(0);
    float mean_y = state(1);


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

