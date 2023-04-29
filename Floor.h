//
// Created by bishwajit on ২৩/৬/২১.
//

#ifndef RAY_TRACING_1605003_FLOOR_H
#define RAY_TRACING_1605003_FLOOR_H

#include "1605003_Objects.h"
#include "bitmap_image.hpp"
#include <GL/glut.h>
const bool isTextureOn = false;

void drawSquare(double a, int col)
{
    glColor3f(col,col,col);
    glBegin(GL_QUADS);{
        glVertex3f( 0, 0,0);
        glVertex3f( 0,a,0);
        glVertex3f(a,a,0);
        glVertex3f(a, 0,0);
    }glEnd();
}

struct Floor: Object{

    bitmap_image texture;
    Floor(int floor_width, int tile_width){
        reference_point = Point(-1.0*floor_width/2, -1.0*floor_width/2, 0);
        width = tile_width;
        length = floor_width;
        coEfficients[0] = 0.1;
        coEfficients[1] = 0.4;
        coEfficients[2] = 0.1;
        coEfficients[3] = 0.5;
        shine = 1;
        refractionCoeff = 0;

        if (isTextureOn) {
            texture = bitmap_image("radha-vrindavanChandra.bmp");
            cout << "got image" << endl;
            cout << texture.width() << " " << texture.height() << endl;
        }
    }

    void draw() override{
        int limit = (int)(length/width + 1e-9);
        for (int i = 0; i < limit; i++){
            for (int j = 0; j < limit; j++){
                Point cur = reference_point;
                cur.x += i * width;
                cur.y += j * width;
                glPushMatrix();
                glTranslatef(cur.x, cur.y, 0);
                drawSquare(width, (i+j)&1);
                glPopMatrix();
            }
        }
    }

    Color getColorAt(Point &p) override {
        int x = max(0, int(EPS + (p.x - reference_point.x)/ width));
        int y = max(0, int(EPS + (p.y - reference_point.y)/ width));
        bool parity = (x + y) & 1;

        if (!isTextureOn || parity) {
            return Point(parity, parity, parity);
        }
        else {
            double du = width / texture.width();
            double dv = width / texture.height();
            double extra_x = p.x - reference_point.x - width * x;
            double extra_y = p.y - reference_point.y - width * y;
//            extra_x = width - extra_x;
            extra_y = width - extra_y;
            int i = extra_x/du;
            int j = extra_y/dv;
            unsigned char r, g, b;
            texture.get_pixel(i, j, r, g, b);
            return Point(r/255.0, g/255.0, b/255.0);
        }
    }

    Point getNormal(Point &p) override {
        return Point (0, 0, 1);
    }

    double nearestTouch(Ray &r) override {
        double d = 0;
        Point n = Point(0,0,1);
        double denom = dot(n, r.dir);
        if (fabs(denom) < EPS) {
            return -1.0; // ray is parallel to the plane
        }
        double t = -(d + dot(n, r.start)) / denom;
        if (t < 0) return -1.0;
        Point intersectingPoint = r.start + t * r.dir;
        assert(fabs(intersectingPoint.z) < EPS);

        bool isXinside = reference_point.x < intersectingPoint.x + EPS && intersectingPoint.x < reference_point.x + length + EPS;
        bool isYinside = reference_point.y < intersectingPoint.y + EPS && intersectingPoint.y < reference_point.y + length + EPS;

        if (isXinside && isYinside) {
            return t;
        }
        return -1.0;
    }

    istream &input(istream &is) override {
        return is;
    }

    ostream &output(ostream &os) override {
        os << "Floor" << endl;
        os << "Ref Point " << reference_point << endl;
        os << "floor width: " << length << " tile width " << width << endl;
        return os;
    }
};


#endif //RAY_TRACING_1605003_FLOOR_H
