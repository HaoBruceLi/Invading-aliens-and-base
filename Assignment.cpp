
#include <iostream>
#include <fstream>
#include <climits>
#include <math.h>
#include <GL/freeglut.h>
#include <cmath>
#include <GL/glut.h>
#include "loadTGA.h"
#include "loadBMP.h"


using namespace std;
GLuint txId[9];


//--Globals ---------------------------------------------------------------

float sphere_x = 38.88;
float sphere_y = 64;

float *x, *y, *z;  //vertex coordinate arrays
int *t1, *t2, *t3; //triangles
int nvrt, ntri;    //total number of vertices and triangles


float theta = 20;  // robot arm degree
int decrement = 1;
float donut = 0;

float ufo = 0;
float fly_height = 50;
float ufo_spot = 20;

int robot1 = 100;
int check_robot1 = 1;
int robot2 = 130;
int check_robot2 = 1;
int robot3 = -120;
int check_robot3 = 1;

float angle= -0.7, look_x=0.0, look_y= 249.5, look_z=0.0; //Camera parameters
float cam_hgt = 250, eye_x=200.0, eye_z=250.0;
int mode2 = 0;
int guardian_weapon = 0;

int erengy_diamond = 0;

//fly_robots
int fly_rob = 0;
int jump = -65;
int jump1 = -65;
int check_fly_robot = 0;
int check_fly_robot1 = 0;

//-- Loads mesh data in OFF format    -------------------------------------
void loadMeshFile(const char* fname)
{
    ifstream fp_in;
    int num, ne;

    fp_in.open(fname, ios::in);
    if(!fp_in.is_open())
    {
        cout << "Error opening mesh file" << endl;
        exit(1);
    }

    fp_in.ignore(INT_MAX, '\n');                //ignore first line
    fp_in >> nvrt >> ntri >> ne;                // read number of vertices, polygons, edges

    x = new float[nvrt];                        //create arrays
    y = new float[nvrt];
    z = new float[nvrt];

    t1 = new int[ntri];
    t2 = new int[ntri];
    t3 = new int[ntri];

    for(int i=0; i < nvrt; i++)                         //read vertex list
        fp_in >> x[i] >> y[i] >> z[i];

    for(int i=0; i < ntri; i++)                         //read polygon list
    {
        fp_in >> num >> t1[i] >> t2[i] >> t3[i];
        if(num != 3)
        {
            cout << "ERROR: Polygon with index " << i  << " is not a triangle." << endl;  //not a triangle!!
            exit(1);
        }
    }

    fp_in.close();
    cout << " File successfully read." << endl;
}

//--Function to compute the normal vector of a triangle with index tindx ----------
void normal(int tindx)
{
    float x1 = x[t1[tindx]], x2 = x[t2[tindx]], x3 = x[t3[tindx]];
    float y1 = y[t1[tindx]], y2 = y[t2[tindx]], y3 = y[t3[tindx]];
    float z1 = z[t1[tindx]], z2 = z[t2[tindx]], z3 = z[t3[tindx]];
    float nx, ny, nz;
    nx = y1*(z2-z3) + y2*(z3-z1) + y3*(z1-z2);
    ny = z1*(x2-x3) + z2*(x3-x1) + z3*(x1-x2);
    nz = x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2);
    glNormal3f(nx, ny, nz);
}

void loadTexture()
{

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glGenTextures(8, txId);     // Create 2 texture ids
    glBindTexture(GL_TEXTURE_2D, txId[0]);  //Use this texture
    loadTGA("./images/Water_Pool_Light.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);


    glBindTexture(GL_TEXTURE_2D, txId[1]);  //Use this texture
    loadTGA("./images/door.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

     glBindTexture(GL_TEXTURE_2D, txId[2]);
    loadTGA("./images/left.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** front ***
    glBindTexture(GL_TEXTURE_2D, txId[3]);
    loadTGA("./images/front.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** right ***
    glBindTexture(GL_TEXTURE_2D, txId[4]);
    loadTGA("./images/right.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** back***
    glBindTexture(GL_TEXTURE_2D, txId[5]);
    loadTGA("./images/back.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** top ***
    glBindTexture(GL_TEXTURE_2D, txId[6]);
    loadTGA("./images/top.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** down ***
    glBindTexture(GL_TEXTURE_2D, txId[7]);
    loadTGA("./images/down.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


}


//--------draws the mesh model of the cannon----------------------------
void drawCannon(int i)
{
    if (i) glColor3f(0.2, 0.2, 0.2);

    //Construct the object model here using triangles read from OFF file
    glBegin(GL_TRIANGLES);
        for(int tindx = 0; tindx < ntri; tindx++)
        {
           normal(tindx);
           glVertex3d(x[t1[tindx]], y[t1[tindx]], z[t1[tindx]]);
           glVertex3d(x[t2[tindx]], y[t2[tindx]], z[t2[tindx]]);
           glVertex3d(x[t3[tindx]], y[t3[tindx]], z[t3[tindx]]);
        }
    glEnd();
}


//----------draw a floor plane-------------------
void drawFloor()
{
    bool flag = false;
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    for(int x = -200; x <= 500; x += 10)
    {
        for(int z = -220; z <= 180; z += 10)
        {

            if(flag) glColor3f(0,0.5,1);
            else glColor3f(0,0.6,1);
            glVertex3f(x, -0.1, z);
            glVertex3f(x, -0.1, z+10);
            glVertex3f(x+10, -0.1, z+10);
            glVertex3f(x+10, -0.1, z);
            flag = !flag;
        }
    }
    glEnd();
}

void diamond(){
    glColor3f(0., 0, 1);
    glScalef(5,7,5);
    glRotatef(90,0,0,1);
    glutSolidOctahedron();
}

void wall_texture(){

       //LEFT WALL
     glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[0]);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
      glVertex3f(-90,-5,102.5);
        glTexCoord2f(3, 0);
      glVertex3f(60,-5,102.5);
       glTexCoord2f(3, 1);
      glVertex3f(60,45,102.5);
       glTexCoord2f(0, 1);
      glVertex3f(-90,45,102.5);


    glBindTexture(GL_TEXTURE_2D, txId[0]);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
      glVertex3f(-90,-5,97);
        glTexCoord2f(3, 0);
      glVertex3f(60,-5,97);
       glTexCoord2f(3, 1);
      glVertex3f(60,45,97);
       glTexCoord2f(0, 1);
      glVertex3f(-90,45,97);


    //right wall
    glBindTexture(GL_TEXTURE_2D, txId[0]);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
      glVertex3f(-90,-5,-97.5);
        glTexCoord2f(3, 0);
      glVertex3f(60,-5,-97.5);
       glTexCoord2f(3, 1);
      glVertex3f(60,45,-97.5);
       glTexCoord2f(0, 1);
      glVertex3f(-90,45,-97.5);

    glBindTexture(GL_TEXTURE_2D, txId[0]);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
      glVertex3f(-90,-5,-103);
        glTexCoord2f(3, 0);
      glVertex3f(60,-5,-103);
       glTexCoord2f(3, 1);
      glVertex3f(60,45,-103);
       glTexCoord2f(0, 1);
      glVertex3f(-90,45,-103);


    //front wall
    glBindTexture(GL_TEXTURE_2D, txId[0]);  //outside
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
      glVertex3f(62.5,-5,102.5);
        glTexCoord2f(3, 0);
      glVertex3f(62.5, -5, -102.5);
       glTexCoord2f(3, 1);
      glVertex3f(62.5,45,-102.5);
       glTexCoord2f(0, 1);
      glVertex3f(62.5,45,102.5);


    glBindTexture(GL_TEXTURE_2D, txId[0]);   //inside
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
      glVertex3f(57,-5,102.5);
        glTexCoord2f(3, 0);
      glVertex3f(57, -5, -102.5);
       glTexCoord2f(3, 1);
      glVertex3f(57,45,-102.5);
       glTexCoord2f(0, 1);
      glVertex3f(57,45,102.5);


    //~ //back wall
    glBindTexture(GL_TEXTURE_2D, txId[0]);  //outside
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
      glVertex3f(-95,-5,97);
        glTexCoord2f(3, 0);
      glVertex3f(-95, -5, -97);
       glTexCoord2f(3, 1);
      glVertex3f(-95,45,-97);
       glTexCoord2f(0, 1);
      glVertex3f(-95,45,97);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[0]);   //inside
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
      glVertex3f(-89.5,-5,102.5);
        glTexCoord2f(3, 0);
      glVertex3f(-89.5, -5, -102.5);
       glTexCoord2f(3, 1);
      glVertex3f(-89.5,45,-102.5);
       glTexCoord2f(0, 1);
      glVertex3f(-89.5,45,102.5);

    glEnd();
    glDisable(GL_TEXTURE_2D);

}

void door_texture(){

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[1]);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex3f(63,45,15);
        glTexCoord2f(1, 0);
      glVertex3f(63,45,-15);
       glTexCoord2f(1, 1);
      glVertex3f(63, -5, -15);
       glTexCoord2f(0, 1);
      glVertex3f(63,-5,15);

    glEnd();
    glDisable(GL_TEXTURE_2D);

}
void walls(){

    glPushMatrix();
    glTranslatef(-16, 0, 100);
    glScalef(100, 60, 3);
    glColor3f(0.5,1,0);
    glutSolidCube(1.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-16, 0, -100);
    glScalef(100, 60, 3);
    glColor3f(0.5,1,0);
    glutSolidCube(1.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-92, 0, 0);
    glScalef(3, 60, 136);
    glColor3f(0.5,1,0);
    glutSolidCube(1.5);
    glPopMatrix();

    //front
    glNormal3f(0.0, 0.0, 1.0);
    glPushMatrix();
    glTranslatef(60, 0, 0);
    glScalef(3, 60, 130);
    glColor3f(0.5,1,0);
    glutSolidCube(1.5);
    glPopMatrix();

    //front diamond

    glPushMatrix();
    glTranslatef(60, 55, 70);
    diamond();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(60, 55, 40);
    diamond();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(60, 55, -70);
    diamond();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(60, 55, -40);
    diamond();
    glPopMatrix();

    //back diamond

    glPushMatrix();
    glTranslatef(-90, 55, 70);
    diamond();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-90, 55, 40);
    diamond();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-90, 55, -70);
    diamond();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-90, 55, -40);
    diamond();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-90, 55, -2);
    diamond();
    glPopMatrix();

    //left diamond
    glPushMatrix();
    glTranslatef(-16, 55, -100);
    diamond();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(18, 55, -100);
    diamond();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-50, 55, -100);
    diamond();
    glPopMatrix();

    //right diamond
    glPushMatrix();
    glTranslatef(-16, 55, 100);
    diamond();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(18, 55, 100);
    diamond();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-50, 55,100);
    diamond();
    glPopMatrix();

}

void cannon(int i){

    glPushMatrix();           //wheel
    if (i) glColor3f(1., 1., 1.);
    glTranslatef(-15, 30, 15);
    glRotatef(180,0,1,0);
    glutSolidTorus(4, 16, 20, 30);
    glPopMatrix();

    glPushMatrix();
    if (i) glColor3f(1., 1., 1.);
    glTranslatef(-15, 30, -15);
    glRotatef(180,0,1,0);
    glutSolidTorus(4, 16, 20, 30);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-15, 30, 15);
    glScalef(20,20,20);
    glTranslatef(0, -1., 0);
    glRotatef(-90,1,0,0);
    GLUquadricObj *obj = gluNewQuadric();
    gluCylinder(obj, 0.1, 0.1, 2, 30, 30);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-15, 30, 15);
    glScalef(20,20,20);
    glTranslatef(-0.9, 0., 0);
    glRotatef(90,0,1,0);
    gluCylinder(obj, 0.1, 0.1, 1.8, 30, 30);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(-15, 30, -15);
    glScalef(20,20,20);
    glTranslatef(0, -1., 0);
    glRotatef(-90,1,0,0);
    gluCylinder(obj, 0.1, 0.1, 2, 30, 30);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-15, 30, -15);
    glScalef(20,20,20);
    glTranslatef(-0.9, 0., 0);
    glRotatef(90,0,1,0);
    gluCylinder(obj, 0.1, 0.1, 1.8, 30, 30);
    glPopMatrix();

    //barrel
    glPushMatrix();
    glTranslatef(-20, 30, 0);    //Pivot point coordinates
    glRotatef(30, 0, 0, 1);      //Rotation
    glTranslatef(20, -30, 0);
    drawCannon(i);
    glPopMatrix();

    //bullet
    glPushMatrix();
    glTranslatef(sphere_x, sphere_y, 0);
    if (i) glColor3f(1,0,0);
    glutSolidSphere(5, 36, 18);
    glPopMatrix();

}

void pillar(){

    glPushMatrix();
    glTranslatef(-10, 60, -10);
    glRotatef(90,1,0,0);
    glScalef(5,5,20);
    glColor3f(0.6,0.6,0.6);
    GLUquadricObj *obj = gluNewQuadric();
    gluCylinder(obj, 1, 1, 3, 30, 30);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-10, 70, -10);
    glRotatef(90,1,0,0);
    glScalef(8,8,3);
    glColor3f(0.6,0.6,0.6);
    GLUquadricObj *obj1 = gluNewQuadric();
    gluCylinder(obj1, 1.3, 1.3, 3, 30, 30);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-10, 65, -10);
    glRotatef(90,1,0,0);
    glScalef(10,10,3);
    glColor3f(0.6,0.6,0.6);
    GLUquadricObj *obj2 = gluNewQuadric();
    gluCylinder(obj2, 0.8, 0.8, 3, 30, 30);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(-10, 5, -10);
    glRotatef(90,1,0,0);
    glScalef(5,5,20);
    glColor3f(0.6,0.6,0.6);
    glutSolidTorus(0.3, 1, 10, 30);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-10, 82, -10);
    glRotatef(erengy_diamond,0,1,0);
    glScalef(5,5,5);
    glColor3f(1, 1, 0);
    glutSolidDodecahedron();
    glPopMatrix();

}

void walking_robot(){

    glColor3f(1,1,1);      //Head
    glPushMatrix();
    glTranslatef(0, 7.7, 0);
    glutSolidCube(1.2);
    glPopMatrix();


    glColor3f(0.2,0.8,1);          //Torso
    glPushMatrix();
    glTranslatef(0, 5.5, 0);
    glScalef(3, 3, 1.4);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();           //donut
    glColor3f(1., 1., 0.);
    glRotatef(donut, 0, 1, 0);
    glTranslatef(0.1, 2.2, 0);
    glRotatef(donut, 0, 1, 0);
    glRotatef(90,1,0,0);
    glutSolidTorus(0.5, 2, 20, 30);
    glPopMatrix();

    glColor3f(1,0,0);     // right arm
    glPushMatrix();
    glTranslatef(-2, 6.5, 0);
    glRotatef(40, 1, 0, 0);
    glTranslatef(2, -6.5, 0);
    glTranslatef(-2, 5, 0);
    glScalef(1, 5, 1);
    glutSolidCube(1);
    glPopMatrix();

    glColor3f(1,0,0);          //Left arm
    glPushMatrix();
    glTranslatef(2, 6.5, 0);
    glRotatef(-theta, 1, 0, 0);
    glTranslatef(-2, -6.5, 0);
    glTranslatef(2, 5, 0);
    glScalef(1, 5, 1);
    glutSolidCube(1);
    glPopMatrix();

    glColor3f(1,1,0);                //weapon
    glPushMatrix();
    glTranslatef(-2,3, 0);
    glRotatef(-45, 1.0, 0.0, 0.0);
    glScalef(0.3, 13, 0.3);
    glutSolidCube(1.1);
    glPopMatrix();

}

void fly_robot(){

    glPushMatrix();
    glColor3f(0,0.5,0);
    glTranslatef(5, 25, 150);
    glScalef(6,6,6);
    glutSolidSphere(2, 30,30);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1,1,1);
    glTranslatef(5, 40, 150);
    glScalef(0.5,8,0.5);
    glutSolidCube(3);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1,1,1);
    glTranslatef(5, 50, 150);
    glRotatef(fly_rob, 1, 0, 0);
    glScalef(0.5,9,0.5);
    glutSolidCube(3);
    glPopMatrix();
}

void guardians(int i){

    if (i) glColor3f(1,1,1 );      //Head
    glPushMatrix();
    glTranslatef(0, 7.7, 0);
    glutSolidCube(1);
    glPopMatrix();


    if (i) glColor3f(0.1,0.1,0.1);          //Torso
    glPushMatrix();
    glTranslatef(0, 5.5, 0);
    glScalef(3, 3, 1.4);
    glutSolidCube(1);
    glPopMatrix();

    if (i)  glColor3f(0.1,0.1,0.1);  // Right leg
    glPushMatrix();
    glTranslatef(-0.8, 2.2, 0);
    glScalef(1, 4.4, 1);
    glutSolidCube(1);
    glPopMatrix();


    if (i)  glColor3f(0.1,0.1,0.1);         //Left leg
    glPushMatrix();
    glTranslatef(0.8, 2.2, 0);
    glScalef(1, 4.4, 1);
    glutSolidCube(1);
    glPopMatrix();


    if (i)  glColor3f(0.1,0.1,0.1);     // right arm
    glPushMatrix();
    glRotatef(10, 0.0, 0.0, 1.0);
    glTranslatef(-2, 6.3, 0);
    glScalef(3.5, 1, 1);
    glutSolidCube(1);
    glPopMatrix();

    if (i)  glColor3f(0.1,0.1,0.1);          //Left arm
    glPushMatrix();
    glTranslatef(2, 4.8, 0);
    glScalef(1, 4, 1);
    glutSolidCube(1);
    glPopMatrix();

    if (i)  glColor3f(1,1,0);     //weapon
    glPushMatrix();
    glTranslatef(-4,6, 0);
    glRotatef(40, 1.0, 0.0, 0.0);
    glScalef(0.3, 13, 0.3);
    glutSolidCube(1.1);
    glPopMatrix();
}

void Ufo(int i){

    //bottom
    glPushMatrix();
    if (i) glColor3f(1, 0.2, 0.2);
    glRotatef(90, 0.0, 0.0, 1.0);
    glTranslatef(60, 10, 0);
    glScalef(1, 5, 5);
    glutSolidSphere(8, 100,100);
    glPopMatrix();

    //mid
    glPushMatrix();
    if (i) glColor3f(0,0,0);
    glRotatef(90, 0.0, 0.0, 1.0);
    glTranslatef(65, 10, 0);
    glutSolidSphere(16, 100,100);
    glPopMatrix();

    //top
    glPushMatrix();
    if (i) glColor3f(1, 0.5, 0.8);
    glRotatef(90, 0.0, 0.0, 1.0);
    glTranslatef(65, 10, 0);
    glScalef(1, 4, 4);
    glutSolidSphere(6, 100,100);
    glPopMatrix();

    glFlush();

}

void skybox(){
    glEnable(GL_TEXTURE_2D);

    ////////////////////// LEFT WALL ///////////////////////

    glBindTexture(GL_TEXTURE_2D, txId[2]);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0);     glVertex3f(-300,  -0.4, 300);
        glTexCoord2f(1, 0);   glVertex3f(-300, -0.4, -300);
        glTexCoord2f(1, 1);    glVertex3f(-300, 300., -300);
        glTexCoord2f(0, 1);    glVertex3f(-300, 300, 300);
    glEnd();

    ////////////////////// FRONT WALL ///////////////////////
    glBindTexture(GL_TEXTURE_2D, txId[3]);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(-300,  -0.4, -300);
        glTexCoord2f(1, 0); glVertex3f(300, -0.4, -300);
        glTexCoord2f(1, 1); glVertex3f(300, 300, -300);
        glTexCoord2f(0, 1); glVertex3f(-300,  300, -300);
    glEnd();

    ////////////////////// RIGHT WALL ///////////////////////
    glBindTexture(GL_TEXTURE_2D, txId[4]);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(300,  -0.4, -300);
        glTexCoord2f(1, 0); glVertex3f(300, -0.4, 300);
        glTexCoord2f(1, 1); glVertex3f(300, 300,  300);
        glTexCoord2f(0, 1); glVertex3f(300,  300,  -300);
    glEnd();


    ////////////////////// REAR WALL ////////////////////////
    glBindTexture(GL_TEXTURE_2D, txId[5]);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f( 300, -0.4, 300);
        glTexCoord2f(1, 0); glVertex3f(-300, -0.4,  300);
        glTexCoord2f(1, 1); glVertex3f(-300, 300,  300);
        glTexCoord2f(0, 1); glVertex3f( 300, 300, 300);
    glEnd();

    /////////////////////// TOP //////////////////////////
    glBindTexture(GL_TEXTURE_2D, txId[6]);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(-300, 300, -300);
        glTexCoord2f(1, 0); glVertex3f(300, 300,  -300);
        glTexCoord2f(1, 1); glVertex3f(300, 300,  300);
        glTexCoord2f(0, 1); glVertex3f(-300, 300, 300);
    glEnd();

    /////////////////////// DOWN //////////////////////////
    glBindTexture(GL_TEXTURE_2D, txId[7]);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(-300, -0.4, 300);
        glTexCoord2f(1, 0); glVertex3f(300, -0.4,  300);
        glTexCoord2f(1, 1); glVertex3f(300, -0.4, -300);
        glTexCoord2f(0, 1); glVertex3f(-300, -0.4, -300);
    glEnd();
    glDisable(GL_TEXTURE_2D);

}

void display()
{
    float lpos[4] = {0.0f, 10.0f, 100.0f, 1.0f};  //light's position
    float ufo_lgt_pos[] = {20.0f, fly_height, 10.0f, 1.0f};
    float ufo_lgt_dir[]={-1, -10, 0};

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    //GL_LINE = Wireframe;   GL_FILL = Solid
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(eye_x, cam_hgt, eye_z,  look_x, look_y, look_z, 0, 1, 0);

    glLightfv(GL_LIGHT0, GL_POSITION, lpos);   //set light position
    glLightfv(GL_LIGHT1, GL_POSITION, ufo_lgt_pos);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, ufo_lgt_dir);

    glRotatef(angle, 0, 0, 1);        //rotate the whole scene

    drawFloor();
    walls();
    wall_texture();
    door_texture();
    skybox();

    glPushMatrix();
    glTranslatef(-45,0,jump1);
    fly_robot();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(20,0,jump);
    fly_robot();
    glPopMatrix();



    float shadowMat[16] = { 500,0,0,0, -100,0,-100,-1, 0,0,500,0, 0,0,0,500 };
    glDisable(GL_LIGHTING);
    glPushMatrix();
     glMultMatrixf(shadowMat);
     glTranslatef(130, 0, 0);
    glScalef(0.5, 0.5, 0.5);
     glColor4f(0., 0., 0., 1.0);
     cannon(0);
    glPopMatrix();
    glEnable(GL_LIGHTING);

    glPushMatrix();
    glTranslatef(130, 0, 0);
    glScalef(0.5, 0.5, 0.5);
    cannon(1);
    glPopMatrix();

    //guardians
    float shadowMat1[16] = {500,0,0,0, 200,0,-200,-1, 0,0,500,0, 0,0,0,500};
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glMultMatrixf(shadowMat1);
    glTranslatef(100, 0, 50);
    glRotatef(90, 0.0, 1.0, 0.0);
    glScalef(3, 5, 3);
    glColor4f(0., 0., 0., 1.0);
    guardians(0);
    glPopMatrix();
    glEnable(GL_LIGHTING);

    glPushMatrix();
    glTranslatef(100, 0, 50);
    glRotatef(90, 0.0, 1.0, 0.0);
    glScalef(3, 5, 3);
    guardians(1);
    glPopMatrix();

    glDisable(GL_LIGHTING);
    glPushMatrix();
    glMultMatrixf(shadowMat1);
    glTranslatef(100, 0, -50);
    glRotatef(90, 0.0, 1.0, 0.0);
    glScalef(3, 5, 3);
    glColor4f(0., 0., 0., 1.0);
    guardians(0);
    glPopMatrix();
    glEnable(GL_LIGHTING);

    glPushMatrix();
    glTranslatef(100, 0, -50);
    glRotatef(90, 0.0, 1.0, 0.0);
    glScalef(3, 5, 3);
    guardians(1);
    glPopMatrix();

    // 4 pillars
    glPushMatrix();
    glTranslatef(70, 0, 110);
    pillar();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(70, 0, -90);
    pillar();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-80, 0, -90);
    pillar();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-80, 0, 110);
    pillar();
    glPopMatrix();


    //robot-1
    glPushMatrix();
    glTranslatef(robot1, 0, 140);
    glRotatef(90, 0.0, 1.0, 0.0);
    glScalef(4, 4, 4);
    walking_robot();
    glPopMatrix();

    //robot-2
    glPushMatrix();
    glTranslatef(-120, 0, robot2);
    glRotatef(180, 0.0, 1.0, 0.0);
    glScalef(4, 4, 4);
    walking_robot();
    glPopMatrix();

    //robot-3
    glPushMatrix();
    glTranslatef(robot3, 0, -150);
    glRotatef(90, 0.0, 1.0, 0.0);
    glScalef(4, 4, 4);
    walking_robot();
    glPopMatrix();

    //~ // float shadowMat[16] = {500,0,0,0, 200,0,-200,-1, 0,0,500,0, 0,0,0,500};
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glMultMatrixf(shadowMat1);
    glRotatef(ufo, 0, 1, 0.1);
    glTranslatef(20, fly_height, -10);
    glColor3f(0,0,0);
    Ufo(0);
    glPopMatrix();
    glEnable(GL_LIGHTING);

    //UFO
    glPushMatrix();
    glRotatef(ufo, 0, 1, 0.1);
    glTranslatef(20, fly_height, -10);
    Ufo(1);
    glPopMatrix();
    glFlush();
    glutSwapBuffers();
}

//------- Initialize OpenGL parameters -----------------------------------
void initialize()
{
    float grey[4] = {0.2, 0.2, 0.2, 1.0};
    float white[4]  = {1.0, 1.0, 1.0, 1.0};
    look_x = eye_x + sin(angle);
    look_z = eye_z - cos(angle);
    loadTexture();

    loadMeshFile("Cannon.off");             //Specify mesh file name here

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);   //Background colour

    glEnable(GL_LIGHTING);                  //Enable OpenGL states
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_TEXTURE_2D);

    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);


    glLightfv(GL_LIGHT1, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT,10);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 50, 1000);

}

void fly(int value){

    if (mode2 == 1){
        if (fly_height < 277) fly_height += 3, cam_hgt+=3;
    }
    else{
        if (fly_height < 277) fly_height +=3;

    }
    //printf("%f\n", fly_height);
    glutPostRedisplay();
}



void fly_back(int value){

    if (mode2 == 1){
        if (fly_height > 20)   fly_height -=3, cam_hgt-=3;
    }
    else{
        if (fly_height > 20)   fly_height -=3;
    }
    printf("%f \n", fly_height);

    glutPostRedisplay();

}

void myTimer(int value)
{
    //guardian weapon
    guardian_weapon += 30;
    if(guardian_weapon > 360) guardian_weapon = 0;

    //ufo rotation
    ufo +=3;
    if(ufo > 360) ufo = 0;

    //move fly_robot's wing
    fly_rob +=100;
    if(fly_rob > 360) fly_rob = 0;

    //move fly_robot1
    if(check_fly_robot == 0) {
        jump -=3;
        if (jump == -245)  check_fly_robot =1;
    }

    if(check_fly_robot == 1) {
        jump +=3;
        if (jump == -65) check_fly_robot =0;
    }

    //move fly_robot2
    if(check_fly_robot1 == 0) {
        jump1 -=4;
        if (jump1 == -245)  check_fly_robot1 =1;
    }

    if(check_fly_robot1 == 1) {
        jump1 +=4;
        if (jump1 == -65) check_fly_robot1 =0;
    }

    erengy_diamond +=10;
    if(erengy_diamond > 360) erengy_diamond = 0;

    //move robot arm and donut
    donut += 30;
    if(donut > 360) donut = 0;

    if (decrement == 1) {
        theta --;
        if (theta == -30){
            decrement = 0 ;
        }
    }

    if (decrement == 0) {
        theta ++;
        if (theta == 30){
            decrement = 1;
        }
    }

    // move robot1
    if (check_robot1 == 1) {
        robot1 --;
        if (robot1 == -100){
            check_robot1 = 0 ;
        }
    }

    if (check_robot1 == 0) {
        robot1 ++;
        if (robot1 == 100){
            check_robot1 = 1;
        }
    }

    //~ // move robot2
    if (check_robot2 == 1) {
        robot2 --;
        if (robot2 == -130){
            check_robot2 = 0 ;
        }
    }

    if (check_robot2 == 0) {
        robot2 ++;
        if (robot2 == 130){
            check_robot2 = 1;
        }
    }

    //~ // move robot3
    if (check_robot3 == 1) {
        robot3 ++;
        if (robot3 == 120){
            check_robot3 = 0 ;
        }
    }

    if (check_robot3 == 0) {
        robot3 --;
        if (robot3 == -120){
            check_robot3 = 1;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(50, myTimer, 0);

}


float go_speed= 7.0;
float top_speed = 4.0;

void fire(int value){

    top_speed -= 1;



    sphere_x += go_speed;
    sphere_y += top_speed;

    if (sphere_y < 10){
        sphere_y = 10;
        top_speed *= -0.6;
    }

    if (sphere_x < 300){
        glutTimerFunc(30, fire, 0);
    }
    glutPostRedisplay();

    //printf("%f %f\n",top_speed, sphere_y);

}
void keyboard(unsigned char key, int x, int y){
     switch(key)
     {
        case 'c':
            sphere_x = 38.88;
            sphere_y = 64;
            go_speed = 7.0;
            top_speed = 4.0;
            glutTimerFunc(30, fire, 0);
            break;
        case 's':
            glutTimerFunc(50, fly, 0);
            break;
        case 'a':
            glutTimerFunc(50, fly_back, 0);
            break;
     }
    glutPostRedisplay(); //update display
}


void special(int key, int x, int y)
{

    if(key == GLUT_KEY_HOME){

        if ( mode2 == 0){
            mode2 = 1;
            angle = -5;
            cam_hgt = 85;
            look_y = -249.5;
            look_x = look_z =  eye_x = eye_z = 0;

        }
        else if (mode2 == 1){
            mode2 = 0;
            angle = -0.7;
            cam_hgt = 250;
            look_y = 249.5;
            look_x = look_z = 0;
            eye_x = 200;
            eye_z = 250;

        }
    }

    if (key == GLUT_KEY_PAGE_UP) cam_hgt-=0.3;

    if (key == GLUT_KEY_PAGE_DOWN) cam_hgt+=0.3;


    if(key == GLUT_KEY_LEFT){
        angle -= (5.0/360.*3.1415926*2);

    }
    else if(key == GLUT_KEY_RIGHT) {
        angle += (5.0/360.*3.1415926*2);
    }

    else if(key == GLUT_KEY_DOWN)
    {  //Move backward
        eye_x -= 10*sin(angle);
        eye_z += 10*cos(angle);

    }
    else if(key == GLUT_KEY_UP)
    { //Move forward
        eye_x += 10*sin(angle);
        eye_z -= 10*cos(angle);

    }

    look_x = eye_x + sin(angle);
    look_z = eye_z - cos(angle);

    //printf("%f %f %f %f %f\n",angle,look_x,  look_z, eye_x,  eye_z );

    glutPostRedisplay();
}


//  ------- Main: Initialize glut window and register call backs -----------
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize (1200, 800);
   glutInitWindowPosition (10, 10);
   glutCreateWindow ("Assignment");
   initialize();
   glutTimerFunc(20, myTimer, 0);
   glutTimerFunc(20, fly, 0);
   glutKeyboardFunc(keyboard);
   glutDisplayFunc(display);
   glutSpecialFunc(special);
   glutMainLoop();
   return 0;
}

