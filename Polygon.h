//
// Created by jannik on 05.09.18.
//

#ifndef UNTITLED1_POLYGON_H
#define UNTITLED1_POLYGON_H

#include <vector>
#include "SDL.h"



class Point {
public:
    Point() {};
    Point(int x, int y) { this->x = x; this->y = y; };
    int x, y;
};





class Polygon {
public:
    Polygon(std::vector<Point> vertices);
    ~Polygon();
    Point GetCenter(void);
    Point * GetVertices(void);
    int GetNumberOfVertices(void);
    bool DrawFilledPolygon(const SDL_Color color, SDL_Renderer* renderer);
private:
    Point * vertices;
    Point center;
    int length;
};



#endif //UNTITLED1_POLYGON_H

