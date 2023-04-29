//
// Created by bishwajit on ২২/৬/২১.
//
#ifndef RAY_TRACING_1605003_POINT_H
#define RAY_TRACING_1605003_POINT_H

#include <bits/stdc++.h>
using namespace std;

#define pi (2*acos(0.0))

struct Point {
    double x{},y{},z{};
    Point(){}
    Point(double _x, double _y, double _z):x(_x), y(_y), z(_z){}

    Point& operator+=(const Point &p) {
        x += p.x;
        y += p.y;
        z += p.z;
        return *this;
    }
    Point& operator-=(const Point &p) {
        x -= p.x;
        y -= p.y;
        z -= p.z;
        return *this;
    }
    Point& operator*=(const double s) {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }
    Point& operator/=(const double s) {
        x /= s;
        y /= s;
        z /= s;
        return *this;
    }
    Point operator+(const Point& p) const {
        return Point(*this) += p;
    }

    Point operator-(const Point& p) const {
        return Point(*this) -= p;
    }

    Point operator*(const double t) const {
        return Point(*this) *= t;
    }

    Point operator/(const double t) const {
        return Point(*this) /= t;
    }

    friend Point operator*(double t, Point& p);
    friend ostream& operator<<(ostream &os, const Point& p);
    friend istream& operator>>(istream &is, Point& p);

};

ostream& operator<<(ostream &os, const Point &p) {
    return os << fixed << setprecision(7) << p.x << " " << p.y <<" " << p.z;
}

Point operator*(double t, Point &p) {
    return p * t;
}

istream &operator>>(istream &is,Point &p) {
    double x, y, z;
    if (is >> x >> y >> z){
        p.x = x, p.y = y, p.z = z;
    }
    return is;
}

double dot(Point a, Point b){
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

double norm(Point p){
    return sqrt(dot(p, p));
}

Point normalize(Point p){
    return p / norm(p);
}

Point cross(Point a, Point b) {
    return Point(a.y * b.z - a.z * b.y,
                 a.z * b.x - a.x * b.z,
                 a.x * b.y - a.y * b.x);
}

/**
 *
 * @param axis -> rotating axis
 * @param rotatingVector -> the vector that needs to rotate
 * @param angle -> in degrees
 * @return
 */
Point rotateAroundAnAxis(Point axis, Point rotatingVector, double angle){
    angle = pi / 180.0 * angle;
    return rotatingVector * cos(angle) +
           cross(axis, rotatingVector) * sin(angle) +
           axis * dot(axis, rotatingVector) * (1 - cos(angle));
}


struct Color: Point{
    Color()= default;
    Color(Point p): Point(p){
//        this->clip();
    }

    Color operator*(double t){
        Color tmp = Point(*this) * t;
        return tmp;
    }

    Color operator*(Color &rhs){
        Color c;
        c.x = x * rhs.x;
        c.y = y * rhs.y;
        c.z = z * rhs.z;
        return c;
    }

    void clip(){
        x = clamp(x, 0.0, 1.0);
        y = clamp(y, 0.0, 1.0);
        z = clamp(z, 0.0, 1.0);
    }
};



#endif //RAY_TRACING_1605003_POINT_H
