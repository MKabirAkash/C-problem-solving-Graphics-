//
// Created by bishwajit on ৩০/৬/২১.
//

#ifndef RAY_TRACING_1605003_TRIANGLE_H
#define RAY_TRACING_1605003_TRIANGLE_H
#include "1605003_Objects.h"

struct Triangle:Object {
    Point a, b, c;
    Triangle() = default;

    double determinant(Point a, Point b, Point c){
        return a.x * (b.y*c.z - c.y*b.z) + b.x * (a.z*c.y - a.y*c.z) + c.x * (a.y*b.z - b.y*a.z);
    }

    void draw() override {
        glPushMatrix();
        glColor3f(color.x, color.y, color.z);
        glBegin(GL_TRIANGLES);{
            glVertex3f(a.x, a.y, a.z);
            glVertex3f(b.x, b.y, b.z);
            glVertex3f(c.x, c.y, c.z);
        }glEnd();
        glPopMatrix();
    }

    Color getColorAt(Point &p) override {
        return this->color;
    }

    Point getNormal(Point &p) override {
        return normalize(cross(b-a, c-a));
    }

    double nearestTouch(Ray &r) override {
        double A = determinant(r.dir, a - b, a - c);
        if (fabs(A) < EPS) return -1;

        double t = determinant(a - r.start, a - b, a - c) / A;
        double beta = determinant(r.dir, a - r.start, a - c) / A;
        double gamma = determinant(r.dir, a - b, a - r.start) / A;

        if (beta + gamma < 1 + EPS && beta + EPS > 0 && gamma + EPS > 0 && t + EPS > 0) {
            return t;
        }
        return -1.0;
    }

    istream &input(istream &is) override {
        is >> a >> b >> c;
        is >> color;
        for (double & coEfficient : coEfficients) is >> coEfficient;
        is >> shine;
        return is;
    }

    ostream &output(ostream &os) override {
        os << "Printing Triangle " << endl;
        os << a << b << c;
        os << color;
        for (double & coEfficient : coEfficients) os << coEfficient;
        os << shine;
        os << endl;
        return os;
    }

};

#endif //RAY_TRACING_1605003_TRIANGLE_H
