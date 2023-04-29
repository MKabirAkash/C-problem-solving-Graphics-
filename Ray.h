//
// Created by bishwajit on ২৩/৬/২১.
//

#ifndef RAY_TRACING_1605003_RAY_H
#include "1605003_Point.h"

struct Ray{
    Point start;
    Point dir;
    Ray() = default;
    Ray(Point st, Point dir){
        this->start = st;
        this->dir = normalize(dir);
    }

};
#define RAY_TRACING_1605003_RAY_H

#endif //RAY_TRACING_1605003_RAY_H
