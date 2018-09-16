Robot Localization using Extended Kalman Filtering with landmarks


[![Build Status](https://travis-ci.org/jzuern/robot-localization.svg?branch=master)](https://travis-ci.org/jzuern/robot-localization)

# Prerequesites

## Libraries

- SDL2:
Install using 
`$apt install libsdl2-dev`

- Eigen:
Eigen is a header-only library, i.e. no need to install/compile any code separately. Get it [here](eigen.tuxfamily.org).


## Build system

We use [cmake](https://cmake.org/) as the build system for this project. If you do not have it installed on your system, do so using
`$apt install cmake`

Go into the project installation directory and create a build directory:

`$mkdir build`

Then, `cd` into the directory and run cmake on the parent directory and build the project:

`cd build && cmake .. && make`






# How it works

The Extended Kalman Filter is the nonlinear extension of the standard Kalman Filter


*****

# TODO

## style
- Add tests!!
- more README.md

## bugs
- covariance explodes
