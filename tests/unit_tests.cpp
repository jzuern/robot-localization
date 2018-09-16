//
// Created by jannik on 15.09.18.
//

#include <gtest/gtest.h>
#include "../src/Robot.h"


TEST(TestSuite, RobotTest)
{

    SDL_Color red {.r=255, .g=0, .b=0, .a=255};
    Robot robby(200, 200, 0.0, 20, red);

    EXPECT_EQ(1000, 1000);
}

