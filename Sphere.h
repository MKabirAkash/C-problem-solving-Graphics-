//
//

#ifndef RAY_TRACING_1605003_SPHERE_H
#define RAY_TRACING_1605003_SPHERE_H

#include "1605003_Objects.h"
#include "1605003_Point.h"
#include <GL/glut.h>

const int slices = 50, stacks = 50;


struct Sphere:Object {
    Sphere()= default;
    Sphere(Point c, double radius){
        reference_point = c;
        length = radius;
    }
    void draw() override{
        glPushMatrix();
        glTranslatef(reference_point.x, reference_point.y, reference_point.z);
        drawSphere(length, slices, stacks);
        glPopMatrix();
    }
    void drawSphere(double radius,int slices,int stacks)
    {
        Point points[100][100];
        int i,j;
        double h,r;
        //generate points
        for(i=0;i<=stacks;i++)
        {
            h=radius*sin(((double)i/(double)stacks)*(pi/2));
            r=radius*cos(((double)i/(double)stacks)*(pi/2));
            for(j=0;j<=slices;j++)
            {
                points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
                points[i][j].y=h;
                points[i][j].z=r*sin(((double)j/(double)slices)*2*pi);
            }
        }
        //draw quads using generated points
//        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
        glColor3f(color.x, color.y, color.z);
        for(i=0;i<stacks;i++)
        {
            for(j=0;j<slices;j++)
            {
                glBegin(GL_QUADS);{
                    //upper hemisphere
                    glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                    glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                    glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                    glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                    //lower hemisphere
                    glVertex3f(points[i][j].x,-points[i][j].y,points[i][j].z);
                    glVertex3f(points[i][j+1].x,-points[i][j+1].y,points[i][j+1].z);
                    glVertex3f(points[i+1][j+1].x,-points[i+1][j+1].y,points[i+1][j+1].z);
                    glVertex3f(points[i+1][j].x,-points[i+1][j].y,points[i+1][j].z);
                }glEnd();
            }
        }
    }
    bool isInside(Point &p) override {
        Point distVector = p - reference_point;
        double dist = dot(distVector, distVector);
        return (length * length > dist + EPS);
    }
    double getEta(Point &p) override {
        double ratio = 1.0/ this->eta;
        if (isInside(p)){
            ratio = 1/ratio;   
        }
        // cout << "sphere Eta" << "\n";
        return ratio;
    }    
    istream& input(istream &is) override{
        is >> reference_point >> length >> color;
        for (int i = 0; i < 4; i++){
            is >> coEfficients[i];
        }
        is >> shine;
        return is;
    }
    ostream& output(ostream& os) override{
        os << reference_point << endl;
        os << "radius " << length << endl;
        os << "color " << color << endl;
        os << "coeffs " ;
        for (int i = 0; i < 4; i++){
            os << coEfficients[i] << " " ;
        }
        os << endl;
        os << "Shine " << shine << endl;
        return os;
    }

    Color getColorAt(Point &p) override{
        return this->color;
    }

    double nearestTouch(Ray &r) override{
        Point r_o = r.start - reference_point;
        double a = 1;
        double b = 2 * dot(r.dir, r_o);
        double c = dot(r_o, r_o) - length*length;
        double d = b*b - 4*a*c;
        if (d < 0) return -1;
        d = sqrt(d);
        double t_p = (-b+d)/2, t_m = (-b-d)/2;
        if (t_m >= 0) return t_m;
        if (t_p >= 0) return t_p;
        return -1;
    }
    Point getNormal(Point &p) override{
        return normalize(p - this->reference_point);
    }
};


#endif //RAY_TRACING_1605003_SPHERE_H
