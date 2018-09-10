//
// Created by jannik on 04.09.18.
//

#ifndef UNTITLED1_DEFINES_H
#define UNTITLED1_DEFINES_H

#include "cstring"
#include "Eigen/Dense"

const char * window_title = "Robot Program";
int x_start = 100;
int y_start = 100;


#define TILE_SIZE 16
#define WWIDTH 640
#define WHEIGHT 480


// convert coordinates
void from_local_to_global(float * local, float * global, float * delta)
{
    Eigen::Matrix3Xf R;


}



#endif //UNTITLED1_DEFINES_H
