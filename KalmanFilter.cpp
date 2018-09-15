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



void KalmanFilter::localization_landmarks(const std::vector<Landmark> & landmarks)
{

    /* INFO:
     * Formel mu = 'x_hat'
     * Formel sigma = 'P'
     * Formel R = 'Q'
     * Formel u = (not used?)
     * Formel z = 'y'
     * Formel c = ?
     *
     * x_hat(0) = x
     * x_hat(1) = y
     * x_hat(2) = phi
     * x_hat(3) = v
     * x_hat(4) = dphi
     *
     */

    float phi = x_hat(2);
    float v = x_hat(3);
    float omega = x_hat(4);

    float tmp1 = v / omega * (- sin(phi) + sin(phi + omega*DT));
    float tmp2 = v / omega * (  cos(phi) - cos(phi + omega*DT));

    // 1 - update state estimate
    Eigen::VectorXf x_hat_new(3);
    x_hat_new(0) = x_hat(0) + tmp1;
    x_hat_new(1) = x_hat(1) + tmp2;
    x_hat_new(2) = x_hat(2) + omega * DT;

    // Define Jacobian
    float entry_13 = v / omega * (cos(phi) - cos(phi + omega*DT));
    float entry_23 = v / omega * (sin(phi) - sin(phi + omega*DT));

    Eigen::MatrixXf G(3,3);

    G <<    1, 0, entry_13,
            0, 1, entry_23,
            0, 0, 1;

    P = G * P * G.transpose() + Q;

    // covariances of landmark measurements
    float sigma_r = 10.f;
    float sigma_phi = 10.f;
    float sigma_s = 10.f;

    R <<    sigma_r, 0, 0,
            0, sigma_phi, 0,
            0, 0, sigma_s;

    int nLandmarks = landmarks.size();

    for (int i = 0; i < nLandmarks; i++)
    {
        auto lm = landmarks[i];

        Eigen::VectorXf delta(2);
        delta(0) = lm.pos.x - x_hat_new(0);
        delta(1) = lm.pos.y - x_hat_new(1);

        auto q = delta.dot(delta);

        Eigen::VectorXf z_hat_i(3);
        z_hat_i(0) = sqrt(q);
        z_hat_i(1) = atan2(delta(1,delta(0)) - x_hat_new(2));
        z_hat_i(2) = i;

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


    }







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

