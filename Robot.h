//
// Created by jannik on 04.09.18.
//

#ifndef UNTITLED1_ROBOT_H
#define UNTITLED1_ROBOT_H

typedef struct Pose
{
    int x, y;
    float phi;
} Pose;



class Robot {
public:
    Robot();
    ~Robot();
    Pose pose;


private:

};


#endif //UNTITLED1_ROBOT_H
