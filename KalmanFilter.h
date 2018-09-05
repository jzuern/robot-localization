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
    Eigen::VectorXd mean;
    Eigen::VectorXd measurements;
    Eigen::VectorXd control;
    Eigen::MatrixXd covariance;



};


#endif //UNTITLED1_KALMANFILTER_H
