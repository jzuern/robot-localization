//
// Created by jannik on 04.09.18.
//

#ifndef UNTITLED1_KALMANFILTER_H
#define UNTITLED1_KALMANFILTER_H

#include <Eigen/Dense>

class KalmanFilter {
public:
    KalmanFilter();
    ~KalmanFilter();

    bool update();
    bool predict();

private:
//    Eigen::VectorXf mean;
//    Eigen::VectorXf measurements;
//    Eigen::VectorXf control;
//    Eigen::MatrixXf covariance;


    Eigen::MatrixXf A; // system matrix
    Eigen::VectorXf x; // state vector
    Eigen::VectorXf w; // measurement vector

    // x_(k+1) = A*x + w

    Eigen::MatrixXf Y; // observation matrix
    Eigen::MatrixXf C; // ??
    Eigen::MatrixXf 



};


#endif //UNTITLED1_KALMANFILTER_H
