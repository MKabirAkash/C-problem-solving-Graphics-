#include <GL/glut.h>
#include <bits/stdc++.h>
#include "1605003_Point.h"
#include "1605003_Objects.h"
#include "1605003_Sphere.h"
#include "1605003_Light.h"
#include "1605003_Floor.h"
#include "bitmap_image.hpp"
#include "1605003_Ray.h"
#include "1605003_Triangle.h"
#include "1605003_General.h"

using namespace std;

#define pi (2*acos(0.0))

int drawaxes;
double keySensitivity = 2, angleSensitivity = 1;
const int windowHeight = 500, windowWidth = 500;
const int FlOORWIDTH = 1000, TILEWIDTH = 20;

// constants about shapes
vector<Object*> objects;
vector<Light*> lights;
int level_of_recursion;
bool isShadowOn = true;
bool isRefractionOn = false;


Point u, l, r, eye;
int pixels, imageWidth, imageHeight;
double const viewAngle = 80;

void drawAxes() {
    if (drawaxes == 1) {
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINES);
        {
            glVertex3f(100, 0, 0);
            glVertex3f(-100, 0, 0);

            glVertex3f(0, -100, 0);
            glVertex3f(0, 100, 0);

            glVertex3f(0, 0, 100);
            glVertex3f(0, 0, -100);
        }
        glEnd();
    }
}

void drawScreen(){
    drawAxes();

    for (auto object: objects){
        object->draw();
    }

    for (auto light: lights) {
        light->draw();
    }
}

void capture(){
    auto st = clock();

    bitmap_image image(imageWidth, imageHeight);
    for (int i = 0; i < imageWidth; i++){
        for (int j = 0; j < imageHeight; j++){
            image.set_pixel(i, j, 0, 0, 0); // background black
        }
    }
    double planeDistance = (windowHeight / 2.0) / tan(viewAngle/2 * pi/180 );
    Point topLeft = eye + l * planeDistance - r * windowWidth/2 + u * windowHeight/2;
    double du = 1.0 * windowWidth / imageWidth;
    double dv = 1.0 * windowHeight / imageHeight;
    topLeft = topLeft + r* 0.5*du - u * 0.5*dv;

    cout << "eyePosition: " << eye << endl;

    for (int i = 0; i < imageWidth; i++){
        for (int j = 0; j < imageHeight; j++){
            Point curPixel = topLeft + r*i*du - u*j*dv;
            Ray ray(eye, curPixel - eye);
            Color dummy, color;
            double tMin = 1e9;
            Object* nearestObject = nullptr;
            for (auto object : objects){
                double t = object->intersect(ray, dummy, 0);
                if (t < 0) continue;
                if (t < tMin) {
                    tMin = t;
                    nearestObject = object;
                }
            }
            if (nearestObject){
                tMin = nearestObject->intersect(ray, color, 1);
                color.clip();
                image.set_pixel(i, j, color.x * 255, color.y * 255 , color.z * 255); // color should be multiplied by 255
            }
        }
    }
    image.save_image("my_out.bmp");

    cout << "time required " <<  1.0 * (clock() - st)/ CLOCKS_PER_SEC << endl;
}

void keyboardListener(unsigned char key, int x,int y){
    switch(key){
        case '0':
            capture();
            break;
        case '1':
            l = rotateAroundAnAxis(u, l, angleSensitivity);
            r = rotateAroundAnAxis(u, r, angleSensitivity);
            break;
        case '2':
            l = rotateAroundAnAxis(u, l, -angleSensitivity);
            r = rotateAroundAnAxis(u, r, -angleSensitivity);
            break;
        case '3':
            l = rotateAroundAnAxis(r, l, angleSensitivity);
            u = rotateAroundAnAxis(r, u, angleSensitivity);
            break;
        case '4':
            l = rotateAroundAnAxis(r, l, -angleSensitivity);
            u = rotateAroundAnAxis(r, u, -angleSensitivity);
            break;
        case '5':
            r = rotateAroundAnAxis(l, r, angleSensitivity);
            u = rotateAroundAnAxis(l, u, angleSensitivity);
            break;
        case '6':
            r = rotateAroundAnAxis(l, r, -angleSensitivity);
            u = rotateAroundAnAxis(l, u, -angleSensitivity);
            break;

        case 's':
            isShadowOn ^= 1;
            break;

        case 'r':
            isRefractionOn ^= 1;
            break;

        default:
            break;
    }
}


void specialKeyListener(int key, int x,int y){
    switch(key){
        case GLUT_KEY_DOWN:		//down arrow key
            eye -= l * keySensitivity;
            break;
        case GLUT_KEY_UP:		// up arrow key
            eye += l * keySensitivity;
            break;
        case GLUT_KEY_RIGHT:
            eye += r * keySensitivity;
            break;
        case GLUT_KEY_LEFT:
            eye -= r * keySensitivity;
            break;
        case GLUT_KEY_PAGE_UP:
            eye += u * keySensitivity;
            break;
        case GLUT_KEY_PAGE_DOWN:
            eye -= u * keySensitivity;
            break;
        case GLUT_KEY_INSERT:
            break;
        case GLUT_KEY_HOME:
            break;
        case GLUT_KEY_END:
            break;
        default:
            break;
    }
}

void display(){

    //clear the display
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,0);	//color black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /********************
    / set-up camera here
    ********************/
    //load the correct matrix -- MODEL-VIEW matrix
    glMatrixMode(GL_MODELVIEW);

    //initialize the matrix
    glLoadIdentity();

    //now give three info
    //1. where is the camera (viewer)?
    //2. where is the camera looking?
    //3. Which direction is the camera's UP direction?

    //gluLookAt(100,100,100,	0,0,0,	0,0,1);
    //gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
    Point lookAt = eye + l;

    gluLookAt(eye.x, eye.y, eye.z,
              lookAt.x, lookAt.y, lookAt.z,
              u.x, u.y, u.z);


    //again select MODEL-VIEW
    glMatrixMode(GL_MODELVIEW);


    /****************************
    / Add your objects from here
    ****************************/
    drawScreen();

    //ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
    glutSwapBuffers();
}


void animate(){
    glutPostRedisplay();
}

void init(){
    //codes for initialization
    drawaxes=1;
    eye = Point(0, 0, 200);
    u = Point(0, 1, 0);
    l = Point(0, 0, -1);
    r = Point(1, 0, 0);

    //clear the screen
    glClearColor(0,0,0,0);

    /************************
    / set-up projection here
    ************************/
    //load the PROJECTION matrix
    glMatrixMode(GL_PROJECTION);

    //initialize the matrix
    glLoadIdentity();

    //give PERSPECTIVE parameters
    gluPerspective(viewAngle,	1,	1,	1000.0);
    //field of view in the Y (vertically)
    //aspect ratio that determines the field of view in the X direction (horizontally)
    //near distance
    //far distance
}

void loadData(){
    ifstream in;
    in.open("my_input.txt");
    if (!in.is_open()){
        cout << "file not found!" << endl;
        exit(1);
    }
    in >> level_of_recursion >> pixels;
    cout << "input " << level_of_recursion << " " << pixels << endl;
    imageHeight = imageWidth = pixels;

    int no_obs;
    in >> no_obs;

    cout << "no obs " << no_obs << endl;

    for (int i = 0; i < no_obs; i++){
        string name;
        in >> name;
        Object* obj = nullptr;

        if (name == "sphere"){
            obj = new Sphere();
        }
        else if (name == "triangle") {
            obj = new Triangle();
        }
        else if (name == "general"){
            obj = new General();
        }

        cout << name << endl;
        assert(obj != nullptr);
        in >> (*obj);
        objects.push_back(obj);
    }
    objects.push_back(new Floor(FlOORWIDTH, TILEWIDTH));

    int no_lights;
    in >> no_lights;

    cout << "no lights " << no_lights << endl;

    for (int i = 0; i < no_lights; i++){
        auto* tmp = new Light();
        in >> (*tmp);
        lights.push_back(tmp);
    }

    cout << "objects" << endl;
    for (auto object: objects){
        cout << *object << endl;
    }
    cout << "Lights" << endl;
    for (auto light: lights){
        cout << *light << endl;
    }

    cout << "input completed" << endl;
}



int main(int argc, char **argv){

    auto exiting = [](){
        cout << "Exiting " << endl;
        for (Light* light: lights){
            delete light;
        }
        for (Object* object: objects){
            delete object;
        }
        cout << "Done with cleaning!" <<endl;
        cout << "Now, I can die in peace" << endl;
    };

    atexit(exiting);

    loadData();
    /**
     * Do the openGL config here
     */
    glutInit(&argc,argv);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(300, 250);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

    glutCreateWindow("Ray Tracing!");

    init();

    glEnable(GL_DEPTH_TEST);	//enable Depth Testing

    glutDisplayFunc(display);	//display callback function
    glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occurring)

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);

    glutMainLoop();		//The main loop of OpenGL

    return 0;
}