/* main.c
 *
 * Currently, I have created a scene in a large 3D rectangle with texture mapped iced
 * and a puck that moves based on two key commands: 'p' and 'P'. 'p' moves the puck
 * 20 units in the positive z direction while 'P' moves the puck 50 units in the 
 * positive z direction. Also, I've rendered my power bar using various shades of green
 * and red. I'll probably end up using more of a gradient effect in my final project though.
 * As of this project check, I'm working on rendering my stick and goal, texture mapping the 
 * ice rink's seats and boards, and the physics of the actual puck movement on the ice. 
 * I will also be adding shading and texture mapping to the puck itself.
 *
 * Note: I have kept the default keys function to move along any function so that I can move
 * anywhere in my scene.
 *
 * Created by John Tabone on 11/7/13
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define PI 3.14159
#if defined(__APPLE__)
#define ICE "/Users/John/Desktop/hockey/ice.ppm"
#define PUCK "/Users/John/Desktop/hockey/puck.ppm"
#define NET "/Users/John/Desktop/hockey/net.ppm"
#define BOARDS "/Users/John/Desktop/hockey/boards.ppm"
#define BLEACHERS "/Users/John/Desktop/hockey/bleachers.ppm"
#define WALL "/Users/John/Desktop/hockey/brick.ppm"
#define STICK "/Users/John/Desktop/hockey/stick.ppm"
#else
#define ICE "ice.ppm"
#define PUCK "puck.ppm"
#define NET "net.ppm"
#define BOARDS "boards.ppm"
#define BLEACHERS "bleachers.ppm"
#define WALL "brick.ppm"
#define STICK "stick.ppm"
#endif

GLUquadricObj *quad;
GLfloat viewer[3] = {250.0, 50.0, 1000.0};
GLfloat reference[3] = {250.0, 50.0, 1.0};
GLfloat puckAtTime[3]; //puck at time t
GLfloat initPuck[3] = {250.0, 10.0, 900.0}; //initial puck coordinates
GLuint imageID[6];
int width, height;
GLubyte *imageData;
typedef GLfloat point3[3];
point3 meter[2] = {{230.0, 150.0, 801.0}, {230.0, 170.0, 801.0}};
GLfloat power = 0;
int reachedRight = 0;
int stopPowerBar = 0;
GLfloat time = 0.0;
GLfloat velocity;
GLfloat swivel;
GLfloat tilt;
int puckShot = 0;
int puckCam = 0;
char text[25][80];
int timer = 90;
int score = 0;
GLfloat scoreColor[3] = {1.0, 1.0, 1.0};
int textNum = 999;
int flash = 0;
int i;


void myinit(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, 1.0, 1.0, 1100.0);
	glMatrixMode(GL_MODELVIEW);
    quad = gluNewQuadric();
    gluQuadricDrawStyle(quad, GLU_FILL);
    gluQuadricOrientation(quad, GLU_INSIDE);
    gluQuadricNormals(quad, GLU_SMOOTH);

}

/*
 * Renders text based on puck's status
 */

void renderPuckText(void){
    
    int lengthOfText, i;
    lengthOfText = (int)strlen(text[textNum]);
    
    //Conditionals for text banner size based
    //on message
    if (textNum > 2){
        glBegin(GL_QUADS);
            glColor3f(0.0, 0.0, 0.0);
            glVertex3f(155.0, 225.0, 799.0);
            glVertex3f(155.0, 245.0, 799.0);
            glVertex3f(350.0, 245.0, 799.0);
            glVertex3f(350.0, 225.0, 799.0);
        glEnd();
        
        glBegin(GL_LINE_STRIP);
            glColor3f(1.0, 1.0, 1.0);
            glVertex3f(156.0, 225.0, 800.0);
            glVertex3f(156.0, 245.0, 800.0);
            glVertex3f(350.0, 245.0, 800.0);
            glVertex3f(350.0, 225.0, 800.0);
            glVertex3f(156.0, 225.0, 800.0);
        glEnd();

    } else if (textNum < 2){
        glBegin(GL_QUADS);
            glColor3f(0.0, 0.0, 0.0);
            glVertex3f(210.0, 225.0, 799.0);
            glVertex3f(210.0, 245.0, 799.0);
            glVertex3f(300.0, 245.0, 799.0);
            glVertex3f(300.0, 225.0, 799.0);
        glEnd();
        
        glBegin(GL_LINE_STRIP);
            glColor3f(1.0, 1.0, 1.0);
            glVertex3f(210.0, 225.0, 800.0);
            glVertex3f(210.0, 245.0, 800.0);
            glVertex3f(300.0, 245.0, 800.0);
            glVertex3f(300.0, 225.0, 800.0);
            glVertex3f(210.0, 225.0, 800.0);
        glEnd();
        
    } else if (textNum == 2){
        glBegin(GL_QUADS);
            glColor3f(0.0, 0.0, 0.0);
            glVertex3f(193.0, 225.0, 799.0);
            glVertex3f(193.0, 245.0, 799.0);
            glVertex3f(320.0, 245.0, 799.0);
            glVertex3f(320.0, 225.0, 799.0);
        glEnd();
        
        glBegin(GL_LINE_STRIP);
            glColor3f(1.0, 1.0, 1.0);
            glVertex3f(193.0, 225.0, 800.0);
            glVertex3f(193.0, 245.0, 800.0);
            glVertex3f(320.0, 245.0, 800.0);
            glVertex3f(320.0, 225.0, 800.0);
            glVertex3f(193.0, 225.0, 800.0);
        glEnd();
    }
    

    glPushMatrix();
    
    glTranslatef(215.0, 230.0, 800.0);
    glScalef(0.1, 0.1, 0.1);
    
    //Conditionals to center text based on message
    if (textNum > 2){
        glTranslatef(-(lengthOfText*20), 0.0, 0.0);
    } else if (textNum == 2){
        glTranslatef(-(lengthOfText*10), 0.0, 0.0);
    }
    
    for (i = 0; i < lengthOfText; i++){
        glColor3f(1.0, 1.0, 1.0);
        glutStrokeCharacter(GLUT_STROKE_ROMAN, text[textNum][i]);

    }
    
    glPopMatrix();
}

/*
 * Renders UI text
 */

void renderUI (void){
    int i;
    //UI text
    sprintf(text[7], "Goals Scored: %d", score);
    strcpy(text[8], "1. Press p to stop power meter");
    strcpy(text[9], "2. Aim puck using arrow keys");
    strcpy(text[10], "3. Press space to shoot!");
    strcpy(text[11], "Press c to change to puck view");
    strcpy(text[12], "Press r to reset shot");
    sprintf(text[20], "%d", timer);
    
    //Score
    glPushMatrix();

    glTranslatef(200.0, 180.0, 800.0);
    glScalef(0.1, 0.1, 0.1);
    
    for (i = 0; i < (int)strlen(text[7]); i++){
        glColor3f(scoreColor[0], scoreColor[1], scoreColor[2]);
        glutStrokeCharacter(GLUT_STROKE_ROMAN, text[7][i]);
        
    }
    glPopMatrix();
    
    //Draw banner for score
    
    glBegin(GL_QUADS);
        glColor3f(0.0, 0.0, 0.0);
        glVertex3f(195.0, 175.0, 799.0);
        glVertex3f(195.0, 195.0, 799.0);
        glVertex3f(310.0, 195.0, 799.0);
        glVertex3f(310.0, 175.0, 799.0);
    glEnd();
    
    glBegin(GL_LINE_STRIP);
        glColor3f(scoreColor[0], scoreColor[1], scoreColor[2]);
        glVertex3f(195.0, 175.0, 800.0);
        glVertex3f(195.0, 195.0, 800.0);
        glVertex3f(310.0, 195.0, 800.0);
        glVertex3f(310.0, 175.0, 800.0);
        glVertex3f(195.0, 175.0, 800.0);
    glEnd();
    
    //Controls
    glPushMatrix();
    
    glTranslatef(200.0, 140.0, 800.0);
    glScalef(0.05, 0.05, 0.05);
    
    for (i = 0; i < (int)strlen(text[8]); i++){
        glColor3f(1.0, 1.0, 1.0);
        glutStrokeCharacter(GLUT_STROKE_ROMAN, text[8][i]);
        
    }
    glPopMatrix();
    
    
    glPushMatrix();
    
    glTranslatef(200.0, 130.0, 800.0);
    glScalef(0.05, 0.05, 0.05);

    for (i = 0; i < (int)strlen(text[9]); i++){
        glColor3f(1.0, 1.0, 1.0);
        glutStrokeCharacter(GLUT_STROKE_ROMAN, text[9][i]);
        
    }
    glPopMatrix();
    
    
    glPushMatrix();
    
    glTranslatef(200.0, 120.0, 800.0);
    glScalef(0.05, 0.05, 0.05);
    
    for (i = 0; i < (int)strlen(text[10]); i++){
        glColor3f(1.0, 1.0, 1.0);
        glutStrokeCharacter(GLUT_STROKE_ROMAN, text[10][i]);
        
    }
    glPopMatrix();
    
    
    glPushMatrix();
    
    glTranslatef(200.0, 110.0, 800.0);
    glScalef(0.05, 0.05, 0.05);
    
    for (i = 0; i < (int)strlen(text[11]); i++){
        glColor3f(1.0, 1.0, 1.0);
        glutStrokeCharacter(GLUT_STROKE_ROMAN, text[11][i]);
        
    }
    glPopMatrix();
    
    
    glPushMatrix();
    
    glTranslatef(200.0, 100.0, 800.0);
    glScalef(0.05, 0.05, 0.05);
    
    for (i = 0; i < (int)strlen(text[12]); i++){
        glColor3f(1.0, 1.0, 1.0);
        glutStrokeCharacter(GLUT_STROKE_ROMAN, text[12][i]);
        
    }
    glPopMatrix();
    
    //Draw banner for controls
    
    glBegin(GL_QUADS);
        glColor3f(0.0, 0.0, 0.0);
        glVertex3f(198.0, 98.0, 799.0);
        glVertex3f(198.0, 146.0, 799.0);
        glVertex3f(310.0, 146.0, 799.0);
        glVertex3f(310.0, 98.0, 799.0);
    glEnd();
    
    glBegin(GL_LINE_STRIP);
        glColor3f(1.0, 1.0, 1.0);
        glVertex3f(198.0, 98.0, 800.0);
        glVertex3f(198.0, 146.0, 800.0);
        glVertex3f(310.0, 146.0, 800.0);
        glVertex3f(310.0, 98.0, 800.0);
        glVertex3f(198.0, 98.0, 800.0);
    glEnd();

    
}

void shootPuck (GLfloat swivelDegrees, GLfloat tiltDegrees, GLfloat v, GLfloat t){
    GLfloat g = -9.8; //gravity
    GLfloat swivelRadians = (swivelDegrees) * (M_PI/180);
    GLfloat tiltRadians = (tiltDegrees) * (M_PI/180);
    GLfloat velCompY = v * sin(tiltRadians);
    GLfloat velCompX = v * cos(tiltRadians) * sin(swivelRadians);
    GLfloat velCompZ = (-v) * cos(tiltRadians) * cos(swivelRadians);
    
    //Solve position at time t
    
    puckAtTime[0] = 250.0 + (velCompX * t);
    puckAtTime[1] = 10.0 + (velCompY * t) + ((1/2)*(g)*powf(t, 2));
    puckAtTime[2] = 900.0 + (velCompZ * t);
    
}


void drawPuck(void){
    
    shootPuck(swivel, tilt, velocity, time);
    initPuck[0] = puckAtTime[0];
    initPuck[1] = puckAtTime[1];
    initPuck[2] = puckAtTime[2];
    
    glPushMatrix();
    
    glColor3ub(284, 284, 284); //uses RGB values from HTML codes
    glTranslatef(initPuck[0], initPuck[1], initPuck[2]);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    gluCylinder(quad,20.0,20.0,10.0,30,30);
    glRotatef(180.0, 1.0, 0.0, 0.0);
    gluDisk(quad, 0.0, 20.0, 30, 1);
    glRotatef(180.0, 1.0, 0.0, 0.0);
    gluDisk(quad, 0.0, 20.0, 30, 1);
    
    glPopMatrix();
    
}

void drawGoal(void){
    
    //Left Post
    glPushMatrix();

    glColor3ub(255, 0, 0); //uses RGB values from HTML codes
    glTranslatef(150.0, 200.0, 200.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    gluCylinder(quad,10.0,10.0,200.0,30,30);
    glRotatef(180.0, 1.0, 0.0, 0.0);
    gluDisk(quad, 0.0, 10.0, 30, 1);
    glRotatef(180.0, 1.0, 0.0, 0.0);
    gluDisk(quad, 0.0, 10.0, 30, 1);
    
    glPopMatrix();

    //Right Post
    glPushMatrix();
    
    glColor3ub(255, 0, 0); //uses RGB values from HTML codes
    glTranslatef(350.0, 200.0, 200.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    gluCylinder(quad,10.0,10.0,200.0,30,30);
    glRotatef(180.0, 1.0, 0.0, 0.0);
    gluDisk(quad, 0.0, 10.0, 30, 1);
    glRotatef(180.0, 1.0, 0.0, 0.0);
    gluDisk(quad, 0.0, 10.0, 30, 1);
    
    glPopMatrix();
    
    //Crossbar
    glPushMatrix();
    
    glColor3ub(255, 0, 0); //uses RGB values from HTML codes
    glTranslatef(142.0, 200.0, 200.0);
    glRotatef(90.0, 0.0, 1.0, 0.0);
    gluCylinder(quad,10.0,10.0,216.0,30,30);
    glRotatef(180.0, 1.0, 0.0, 0.0);
    gluDisk(quad, 0.0, 10.0, 30, 1);
    glRotatef(180.0, 1.0, 0.0, 0.0);
    gluDisk(quad, 0.0, 10.0, 30, 1);
    
    glPopMatrix();
    
    
    //Sides of net
    glPushMatrix();
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, imageID[2]);
    glBegin(GL_POLYGON);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(150.0, 5.0, 200.0);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(150.0, 200.0, 200.0);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(150.0, 5.0, 110.0);
    glEnd();
    
    glTranslatef(200.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(150.0, 5.0, 200.0);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(150.0, 200.0, 200.0);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(150.0, 5.0, 110.0);
    glEnd();
    
    glPopMatrix();
    
    //Back of net
    
    glPushMatrix();
    
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(150.0, 5.0, 110.0);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(150.0, 200.0, 200.0);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(350.0, 200.0, 200.0);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(350.0, 5.0, 110.0);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
    
}

void drawStick(void){
    glPushMatrix();
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, imageID[3]);
    
    //Blade of stick
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(220.0, 0.0, 930.0);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(220.0, 10.0, 930.0);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(270.0, 10.0, 930.0);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(270.0, 0.0, 930.0);
    glEnd();
    
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(220.0, 0.0, 925.0);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(220.0, 10.0, 925.0);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(220.0, 10.0, 930.0);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(220.0, 0.0, 930.0);
    glEnd();
    
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(270.0, 0.0, 925.0);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(270.0, 10.0, 925.0);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(220.0, 10.0, 925.0);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(220.0, 0.0, 925.0);
    glEnd();
    
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(270.0, 0.0, 930.0);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(270.0, 10.0, 930.0);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(270.0, 10.0, 925.0);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(270.0, 0.0, 925.0);
    glEnd();
    
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(220.0, 10.0, 930.0);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(220.0, 10.0, 925.0);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(270.0, 10.0, 925.0);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(270.0, 10.0, 930.0);
    glEnd();
    
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(220.0, 0.0, 925.0);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(220.0, 0.0, 930.0);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(270.0, 0.0, 930.0);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(270.0, 0.0, 925.0);
    glEnd();
    
    //Shaft of stick
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(270.0, 0.0, 930.0);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(270.0, 10.0, 930.0);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(510.0, 250.0, 930.0);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(520.0, 250.0, 930.0);
    glEnd();
    
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(270.0, 10.0, 930.0);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(270.0, 10.0, 925.0);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(510.0, 250.0, 925.0);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(510.0, 250.0, 930.0);
    glEnd();
    
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(520.0, 250.0, 925.0);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(510.0, 250.0, 925.0);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(270.0, 10.0, 925.0);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(270.0, 0.0, 925.0);
    glEnd();
    
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(270.0, 0.0, 925.0);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(270.0, 0.0, 930.0);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(520.0, 250.0, 930.0);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(520.0, 250.0, 925.0);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
    
    glPopMatrix();
}

void moveStick() {
    
    glPushMatrix();
    
    glTranslatef(-30.0, -10.0, 10.0);
    glRotatef(-swivel/50, 0.0, 1.0, 0.0);
    glRotatef(tilt/80, 1.0, 0.0, 0.0);
    glTranslatef(30.0, 10.0, -10.0); //Stick position at (220, 0, 930) translated to puck
                                     //position of (250, 10, 920)
                                     // Note: puck-z is 920 to take into account its radius
                                     // of 20
    
    drawStick();
    
    glPopMatrix();
    
}

void drawShotMeter(void){
    int i;
    int p;
    int incr = 0;
    point3 colors[10]= {{223, 1, 1}, {255, 0, 0}, {254, 46, 46}, {250, 88, 88}, {130, 250, 88},
        {100, 254, 46}, {64, 255, 0}, {58, 223, 0}, {49, 180, 4}, {41, 138, 8}};
    
    
    glPushMatrix();
    
    for (i = 0 ; i<10; i++){
        glBegin(GL_POLYGON);
            glColor3ub(colors[i][0], colors[i][1], colors[i][2]);
            glVertex3f(230.0+incr, 150.0, 800.0);
            glVertex3f(235.0+incr, 150.0, 800.0);
            glVertex3f(235.0+incr, 170.0, 800.0);
            glVertex3f(230.0+incr, 170.0, 800.0);
        glEnd();
        incr +=5;
    }
    
    //White bar that will move back and forth
    glTranslatef(power, 0.0, 0.0);
    glBegin(GL_LINES);
        glColor3f(1.0, 1.0, 1.0);
        glVertex3fv(meter[0]);
        glVertex3fv(meter[1]);
    glEnd();
    
    glPopMatrix();
    
}


void drawRink(void){
    point3 vertices[12] = { {0.0, 0.0, 1000.0}, {0.0, 500.0, 1000.0}, {500.0, 500.0, 1000.0}, {500.0, 0.0, 1000.0},
        {500.0, 0.0, 1.0}, {500.0, 500.0, 1.0}, {0.0, 500.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 250.0, 1000.0}, {500.0, 250.0, 1000.0},
        {0.0, 250.0, 1.0}, {500.0, 250.0, 1.0}};
    
    glPushMatrix();
    
    glDepthMask(0);
    
    //Front face
    glBegin(GL_QUADS);
        glColor3f(0.0, 0.0, 0.0); //black
        glVertex3fv(vertices[0]);
        glVertex3fv(vertices[1]);
        glVertex3fv(vertices[2]);
        glVertex3fv(vertices[3]);
    glEnd();
    
    /*Boards*/
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, imageID[0]);
    
    //Back face bottom half (note: facing this face)
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);
        glVertex3fv(vertices[4]);
        glTexCoord2f(0.0, 1.0);
        glVertex3fv(vertices[11]);
        glTexCoord2f(1.0, 1.0);
        glVertex3fv(vertices[10]);
        glTexCoord2f(1.0, 0.0);
        glVertex3fv(vertices[7]);
    glEnd();
    
    //Left face bottom half
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);
        glVertex3fv(vertices[7]);
        glTexCoord2f(0.0, 1.0);
        glVertex3fv(vertices[10]);
        glTexCoord2f(1.0, 1.0);
        glVertex3fv(vertices[8]);
        glTexCoord2f(1.0, 0.0);
        glVertex3fv(vertices[0]);
    glEnd();
    
    //Right face bottom half
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);
        glVertex3fv(vertices[3]);
        glTexCoord2f(0.0, 1.0);
        glVertex3fv(vertices[9]);
        glTexCoord2f(1.0, 1.0);
        glVertex3fv(vertices[11]);
        glTexCoord2f(1.0, 0.0);
        glVertex3fv(vertices[4]);
    glEnd();
    
    /*Walls/Ceiling*/
    
    glBindTexture(GL_TEXTURE_2D, imageID[4]);
    
    //Back face top half
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);
        glVertex3fv(vertices[11]);
        glTexCoord2f(0.0, 1.0);
        glVertex3fv(vertices[5]);
        glTexCoord2f(1.0, 1.0);
        glVertex3fv(vertices[6]);
        glTexCoord2f(1.0, 0.0);
        glVertex3fv(vertices[10]);
    glEnd();
    
    //Left face top half
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);
        glVertex3fv(vertices[10]);
        glTexCoord2f(0.0, 1.0);
        glVertex3fv(vertices[6]);
        glTexCoord2f(1.0, 1.0);
        glVertex3fv(vertices[1]);
        glTexCoord2f(1.0, 0.0);
        glVertex3fv(vertices[8]);
    glEnd();
    
    //Right face top half
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);
        glVertex3fv(vertices[9]);
        glTexCoord2f(0.0, 1.0);
        glVertex3fv(vertices[2]);
        glTexCoord2f(1.0, 1.0);
        glVertex3fv(vertices[5]);
        glTexCoord2f(1.0, 0.0);
        glVertex3fv(vertices[11]);
    glEnd();
    
    //Top face
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);
        glVertex3fv(vertices[1]);
        glTexCoord2f(0.0, 1.0);
        glVertex3fv(vertices[6]);
        glTexCoord2f(1.0, 1.0);
        glVertex3fv(vertices[5]);
        glTexCoord2f(1.0, 0.0);
        glVertex3fv(vertices[2]);
    glEnd();
    
    glDepthMask(1);
    
    /*Ice*/
    
    //Bottom face i.e. the ice surface
    glBindTexture(GL_TEXTURE_2D, imageID[1]);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);
        glVertex3fv(vertices[7]);
        glTexCoord2f(0.0, 1.0);
        glVertex3fv(vertices[0]);
        glTexCoord2f(1.0, 1.0);
        glVertex3fv(vertices[3]);
        glTexCoord2f(1.0, 0.0);
        glVertex3fv(vertices[4]);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
    
    glPopMatrix();

    
}

void resetPuck(void) {
    puckAtTime[0] = initPuck[0];
    puckAtTime[1] = initPuck[1];
    puckAtTime[2] = initPuck[2];
    power = 0;
    reachedRight = 0;
    stopPowerBar = 0;
    time = 0.0;
    velocity = 0.0;
    swivel = 0.0;
    tilt = 0.0;
    puckShot = 0;
    puckCam = 0;

}

void checkGoal(void){
    
    //Resets puck if the power meter is stoppped on the left border
    // and the user shoots
    if (velocity == 0){
        textNum = 6;
        resetPuck();
    }
    
    
    //Goal scored
    if ((puckAtTime[0] > 160 && puckAtTime[0] < 340) && (puckAtTime[1] < 190 && puckAtTime[1] > 0)
        && (puckAtTime[2] > 110 && puckAtTime[2] < 200)){
        textNum = 1;
        score += 1;
        resetPuck();
        
    }
    
    //Puck hits crossbar
    if ((puckAtTime[0] >= 160 && puckAtTime[0] <= 340)
        && (puckAtTime[1] >= 200 && puckAtTime[1] <= 220)
        && (puckAtTime[2] >= 190 && puckAtTime[2] <= 215)){
        textNum = 5;
        resetPuck();
        
    }
    
    //Puck hits left post
    if ((puckAtTime[0] >= 140 && puckAtTime[0] <= 160)
        && (puckAtTime[1] >= 10 && puckAtTime[1] <= 220)
        && (puckAtTime[2] >= 190 && puckAtTime[2] <= 210)){
        textNum = 3;
        resetPuck();
        
    }
    
    //Puck hits right post
    if ((puckAtTime[0] >= 340 && puckAtTime[0] <= 360)
        && (puckAtTime[1] >= 10 && puckAtTime[1] <= 220)
        && (puckAtTime[2] >= 190 && puckAtTime[2] <= 210)){
        textNum = 4;
        resetPuck();
        
    }
}


void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
    gluLookAt(viewer[0], viewer[1], viewer[2], reference[0], reference[1], reference[2], 0.0, 1.0, 0.0);
    
    drawRink();
    drawGoal();
    drawShotMeter();
    drawPuck();
    moveStick();
    renderUI();
    if (textNum != 999){
        renderPuckText();
    }

    
    glFlush();
	glutSwapBuffers(); /*Display next buffer*/
}

void idle(void) {
    
    /*
     * Increment time to render puck at various times in its trajectory. Puck's trajectory is rendered until puck reaches bounds
     * of clipping volume, until time = 20, or if a goal is scored.
     */
    if (time < 20.0 && puckShot == 1 && puckAtTime[0] <= 500.0 && puckAtTime[0] >= 0.0 && puckAtTime[1] >= 10
        && puckAtTime[1] <= 500 && puckAtTime[2] >= 1.0 && puckAtTime[2] <= 900.0){
        time += 0.02;
        checkGoal();
    }
    else if (puckShot == 1){
        textNum = 2;
        resetPuck();
    }
    
    /*
     * Move power bar back and forth between x= 230 and
     * x = 280 (the min and max x values of power meter)
     */
    
    if (power < 50 && reachedRight == 0 && stopPowerBar == 0){
        power += 1;
        if (power == 50) {
            reachedRight = 1;
        }
    } else if (power <= 50 && reachedRight == 1 && stopPowerBar == 0){
        power += -1;
        if (power == 0){
            reachedRight = 0;
        }
    }
    
    if (puckCam == 1){
        viewer[0] = puckAtTime[0];
        viewer[1] = puckAtTime[1];
        viewer[2] = puckAtTime[2] - 25;
    } else{
        viewer[0] = 250.0;
        viewer[1] = 50.0;
        viewer[2] = 1000.0;
    }
    
    //Flash 'Goals Scored' yellow if goal scored
    if (textNum == 1){
        scoreColor[2] = 0.0+flash;
        if (flash == 0){
            flash++;
        } else if (flash == 1) {
            flash--;
        }
    } else{
        scoreColor[2] = 1.0;
    }
    
	display();
}

void keys(unsigned char key, int x, int y)
{
    
    //Reset puck
    if (key == 'r'){
        resetPuck();
    }
    
    //Stop power bar
    if(key == 'p') {
        stopPowerBar = 1;
        velocity = (power/50.0)*100.0;
    }
    
    if (key == 'c'){
        puckCam = 1;
    }
    
    //Shoot puck with space bar if power bar is stopped
    //(note: 32 is ASCII code for space)
    if(key == 32) {
        if (stopPowerBar == 1){
            puckShot = 1;
        }
    }
    
    if (key == 'l'){
        printf("x = %f", puckAtTime[0]);
        printf("\n");
        printf("y = %f", puckAtTime[1]);
        printf("\n");
        printf("z = %f", puckAtTime[2]);
        printf("\n");
    }
   
	display();
}

/*
 * Used to implement special keys (e.g. arrow keys) in
 * OpenGL. In my case, I will be adjusting the swivel angle
 * with the left/right keys and the tilt angle with the 
 * up/down keys.
 *
 * Note: Checking if puckShot is equal to 0 is necessary
 * to prevent user from adjusting shot's trajectory after 
 * taking shot.
 */

void special(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_LEFT:
            if (puckShot == 0){
                swivel -= 1.0;
            }
            break;
        case GLUT_KEY_RIGHT:
            if (puckShot == 0){
                swivel += 1.0;
            }
            break;
        case GLUT_KEY_DOWN:
            if (puckShot == 0){
                tilt -= 1.0;
            }
            break;
        case GLUT_KEY_UP:
            if (puckShot == 0){
                tilt += 1.0;
            }
            break;
        default:
            break;
    }
    
    display();
}

void readPPM( char *fname) {
    FILE *fp;
    char c;
    char b[70];
    int i, n, m, k, nm;
    int count;
    float s;
    GLubyte red, green, blue;
    
    /*Code to read in a PPM file */
    printf("loading PPM textures: %s\n", fname);
    fflush(stdout);
    fp = fopen(fname, "rb");
    if (fp == NULL)
        printf("Cannot open file: %s\n",fname);
    
    fscanf(fp, "%s", b);
    //printf("%s\n", b);
    
    if (b[0] != 'P' || b[1] != '6') {
        printf("%s is not a PPM v. 6 file!\n", fname);
        exit(0);
    }
    fscanf(fp, "%c", &c);
    fscanf(fp, "%c", &c);
    
    while(c == '#') {
        count = 0;
        fscanf(fp, "%c", &c);
        while ((c != '\n') && (count < 100)){
            fscanf(fp, "%c", &c);
            count += 1;
        }
    }
    
    ungetc(c, fp);
    fscanf(fp, "%c", &c);
    fscanf(fp, "%d %d %d", &n, &m, &k);
    printf("n = %d, m = %d, k = %d\n", n, m, k);
    nm = n * m;
    imageData = malloc(3 * sizeof(GLubyte)*nm);
    
    fgetc(fp);
    if (k <= 255) {
        for (i = 0; i < nm; i++) {
            imageData[3*nm - 3*i - 3] = fgetc(fp);
            imageData[3*nm - 3*i - 2] = fgetc(fp);
            imageData[3*nm - 3*i - 1] = fgetc(fp);
        }
    } else {
        printf("Data error. k>255 \n");
    }
    
    width = m;
    height = n;
    fclose(fp);
    printf(".\n");
    /*End code to read in PPM file*/
}


void generateTextures(int IDnum ) {
    glGenTextures(1, &imageID[IDnum]);
    glBindTexture(GL_TEXTURE_2D, imageID[IDnum]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, imageData);
    free(imageData);
    
}

int main(int argc, char** argv)
{
    /*
     * Setting the in-game text
     */
    
    //Puck status text
    strcpy(text[0],"Shoot the puck!");
    strcpy(text[1],"What a goal!");
    strcpy(text[2], "Everything, but net!");
    strcpy(text[3], "The puck hits the left post!");
    strcpy(text[4], "The puck hits the right post!");
    strcpy(text[5], "The puck hits the crossbar!");
    strcpy(text[6], "Shoot it a little harder, guy!");
    
	glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000,1000);
	glutInitWindowPosition(50,50);
	glutCreateWindow("Hockey Guy");
    glEnable(GL_DEPTH_TEST);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    readPPM(BOARDS);
    generateTextures(0);
    readPPM(ICE);
    generateTextures(1);
    readPPM(NET);
    generateTextures(2);
    readPPM(STICK);
    generateTextures(3);
    readPPM(WALL);
    generateTextures(4);
    myinit();
	glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keys);
    glutSpecialFunc(special);
	glutMainLoop();
	
	return 0;
}

