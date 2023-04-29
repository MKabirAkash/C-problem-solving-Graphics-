//
// Created by bishwajit on ১/৭/২১.
//

#ifndef RAY_TRACING_1605003_GENERAL_H
#define RAY_TRACING_1605003_GENERAL_H

#include "1605003_Objects.h"

struct General:Object {
    double A, B, C, D, E, F, G, H, I, J;

    void draw() override {
        Object::draw();
    }

    bool isInside(Ray &r, double t) {
        Point p = r.start + r.dir * t;
        Point ref = reference_point;

        bool xOkay = (fabs(length) < EPS) || (ref.x < p.x + EPS && p.x < ref.x + length + EPS);
        bool yOkay = (fabs(width) < EPS) || (ref.y < p.y + EPS && p.y < ref.y + width + EPS);
        bool zOkay = (fabs(height) < EPS) || (ref.z < p.z + EPS && p.z < ref.z + height + EPS);

        return xOkay && yOkay && zOkay;
    }

    Color getColorAt(Point &p) override {
        return this->color;
    }

    Point getNormal(Point &p) override {
        Point n;
        n.x = 2*A*p.x + D*p.y + E*p.z + G;
        n.y = 2*B*p.y + D*p.x + F*p.z + H;
        n.z = 2*C*p.z + E*p.x + F*p.y + I;
        return normalize(n);
    }

    double nearestTouch(Ray &r) override {
        Point dir = r.dir, o = r.start;

        double a = A*dir.x*dir.x + B*dir.y*dir.y + C*dir.z*dir.z +
                D*dir.x*dir.y + E*dir.x*dir.z + F*dir.y*dir.z;

        double b = 2*A*o.x*dir.x + 2*B*o.y*dir.y + 2*C*o.z*dir.z +
                D*(o.x*dir.y + o.y*dir.x) + E*(o.x*dir.z + o.z*dir.x) +
                F*(o.y*dir.z + dir.y*o.z) + G*dir.x + H*dir.y + I*dir.z;

        double c = A*o.x*o.x + B*o.y*o.y + C*o.z*o.z + D*o.x*o.y +
                E*o.x*o.z + F*o.y*o.z + G*o.x + H*o.y + I*o.z + J;

        double t = -1;
        if (fabs(a) < EPS) {
            t = -c / b;
            if (t + EPS >= 0 && isInside(r, t)) return t;
        }
        else {
            if (a < 0) a *= -1, b *= -1, c *= -1;

            double d = b*b - 4*a*c;
            if (d < 0) return -1;
            d = sqrt(d);
            double t_m = (-b - d)/(2*a);
            if (t_m + EPS >= 0 && isInside(r, t_m)) return t_m;
            double t_p = (-b + d)/(2*a);
            if (t_p + EPS >= 0 && isInside(r, t_p)) return t_p;
        }
        return -1;
    }

    istream &input(istream &is) override {
        is >> A >> B >> C >> D >> E >> F >> G >> H >> I >> J;
        is >> reference_point >> length >> width >> height;
        is >> color;
        for (auto &coEfficient: coEfficients) is >> coEfficient;
        is >> shine;
        return is;
    }

    ostream &output(ostream &os) override {
        os << "general" << endl;
        os << A <<" " << B << " " <<  C <<" " <<  D <<" " <<  E << " " << F << " " << G << " " <<  H << " " <<  I <<" " <<  J << endl;
        cout << reference_point << endl;
        os << length << " " <<  width << " " <<  height << endl;
        os << color << endl;
        for (auto &coEfficient: coEfficients) os << coEfficient;
        os << shine << endl;
        return os;
    }
};

#endif //RAY_TRACING_1605003_GENERAL_H
