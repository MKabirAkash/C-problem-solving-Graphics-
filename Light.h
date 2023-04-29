//
// Created by bishwajit on ২৩/৬/২১.
//

#ifndef RAY_TRACING_1605003_LIGHT_H
#define RAY_TRACING_1605003_LIGHT_H

#include "1605003_Objects.h"
#include <GL/glut.h>

struct Light {
    Point light_pos;
    Color color;
    Light()= default;
    Light(Point pos, Color col):light_pos(pos),color(col){}

    void draw(){
        glPushMatrix();

        glPointSize(5);
        glColor3f(color.x, color.y, color.z);
        glBegin(GL_POINTS);
        glVertex3f(light_pos.x, light_pos.y, light_pos.z);
        glEnd();

        glPopMatrix();
    }
    friend istream& operator>>(istream &is, Light& l);
    friend ostream& operator<<(ostream &os, Light& l);
};


istream &operator>>(istream& is, Light &l){
    is >> l.light_pos >> l.color;
    return is;
}

ostream &operator<<(ostream & os, Light &l){
    os << "light eye " << l.light_pos << endl;
    os << "color " << l.color << endl;
    return os;
}

#endif //RAY_TRACING_1605003_LIGHT_H
