/* main.c
 *
 * A hockey mini game with a simple UI using basic projectile function physics equations 
 * to shoot a puck towards the net. Stick is rotated based on tilt and swivel angles used 
 * to aim shooting of puck. Tilt and swivel are set by arrow keys. Velocity is calculated 
 * by stopping a simple power meter by pressing 'p'. After setting velocity and aiming the 
 * puck, press space to shoot the puck. If you'd like to change the camera angle, press 'c' 
 * before shooting the puck. After shooting the puck, text based on puck status is rendered. 
 * If a goal is scored, your 'Goal Score' count increases. 
 *
 * Created by John Tabone on 11/7/13
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Conditional used to define environment where compilation takes place
#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//Definitions of PI and texture locations relative to main.c
#define PI 3.14159
#define ICE "ice.ppm"
#define NET "net.ppm"
#define BOARDS "boards.ppm"
#define BLEACHERS "bleachers.ppm"
#define WALL "brick.ppm"
#define STICK "stick.ppm"

//Global variable declarations
GLUquadricObj *quad; //quad variable for drawing cylinders and disks
GLfloat viewer[3] = {250.0, 50.0, 1000.0}; //viewer variable for gluLookAt()
GLfloat reference[3] = {250.0, 50.0, 1.0}; //reference variable for gluLookAt()
GLfloat puckAtTime[3]; //puck positionat time t
GLfloat initPuck[3] = {250.0, 10.0, 900.0}; //initial puck coordinates
GLuint imageID[5]; //holds texture ID
int width, height; //used in readPPM()
GLubyte *imageData; //used in readPPM()
typedef GLfloat point3[3]; //typedef to easily store 3D vectors
point3 meter[2] = {{230.0, 150.0, 801.0}, {230.0, 170.0, 801.0}}; //position of bar in power meter
GLfloat power = 0; //power decided by position of bar in power meter
int reachedRight = 0; //flag used in animation of power meter
int stopPowerBar = 0; //flag to see if power meter was stopped
GLfloat time = 0.0; //time used to calculate puck's position in projectile motion
GLfloat velocity; //velocity used to calculate puck's position in projectile motion
GLfloat swivel; //swivel angle used to calculate puck's position in projectile motion
GLfloat tilt; //tilt angle used to calculate puck's position in projectile motion
int puckShot = 0; //flag used to see if puck was shot
int puckCam = 0; //flag to see which camera is active
char text[15][40]; //stores text used in UI
int score = 0; //stores amount of goals scored
GLfloat scoreColor[3] = {1.0, 1.0, 1.0}; //variable used to flash 'Goals Scored' yellow
int textNum = 999; //variable used to see which line of text is being rendered
int flash = 0; //variable used to flash 'Goals Scored' yellow


/* myinit()
 *
 * Init funtion called in main() that specifies the clipping volume and certain
 * properties of the scene.
 *
 */

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

/* renderPuckText()
 *
 * Renders pop-up text based on puck's status
 *
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

/* renderUI()
 *
 * Renders UI text including controls and goals scored.
 *
 */

void renderUI (void){
    int i;
    //UI text
    sprintf(text[7], "Goals Scored: %d", score);
    strcpy(text[8], "1. Press p to stop power meter");
    strcpy(text[9], "2. Aim puck using arrow keys");
    strcpy(text[10], "3. Press space to shoot!");
    strcpy(text[11], "Press c to toggle camera");
    strcpy(text[12], "Press r to reset shot/camera");
    
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

/* shootPuck()
 *
 * Does all the physics behind projectile motion of the puck based on tilt/swivel angles 
 * and initial velocity and position. In this case, we're shooting along the -z axis with
 * y as our vertical axis and x as our horizontal axis.
 *
 */

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

/* drawPuck()
 *
 * Draws the puck at the puck's position at time t which is solved
 * by shootPuck().
 *
 */


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

/* drawGoal()
 *
 * Draws goal including the left post, right post, crossbar, and the net,
 * which I texture map.
 */

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

/* drawStick()
 *
 * Draw a classic, wooden stick behind the puck facing the -z direction. 
 * Decided to draw a lefty stick rather than a righty. However, I'll 
 * definitely implement a righty in due time.
 *
 */

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

/* moveStick()
 *
 * Moves the stick drawn by drawStick() based on the tilt and swivel angles 
 * incremented or decremented by the arrow keys. Stick position at (220, 0, 930) 
 * translated to puck position of (250, 10, 920) so that the stick would rotate
 * about the puck for a more realistic effect. After applying transformations to
 * the stick, drawStick() is called.
 *
 * Note: puck-z is 920 to take into account its radius of 20
 *
 */

void moveStick() {
    
    glPushMatrix();
    
    glTranslatef(-30.0, -10.0, 10.0);
    glRotatef(-swivel/50, 0.0, 1.0, 0.0);
    glRotatef(tilt/80, 1.0, 0.0, 0.0);
    glTranslatef(30.0, 10.0, -10.0);
    
    drawStick();
    
    glPopMatrix();
    
}

/* drawShotMeter()
 *
 * Draws a shot meter that helps determine velocity based on the position of the moving 
 * bar when the user stops it by pressing the 'p' key. The meter's colors range from
 * dark red to dark green (left to right) with dark red signifying a low velocity and 
 * dark green signifying a high velocity.
 *
 */

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

/* drawRink()
 *
 * Draws our hockey rink including the ice, boards, and walls/ceiling. All are texture mapped.
 * Decided on a brick wall instead of actual bleachers because of the insane difficulty of 
 * finding a bleacher or hockey glass texture online. And I'm definitely no artist so drawing
 * actual bleachers was out of the question (for now). I feel that the choice of a brick texture
 * for the walls and ceiling gives the project a nice vintage video game look.
 *
 */

void drawRink(void){
    point3 vertices[12] = { {0.0, 0.0, 1000.0}, {0.0, 500.0, 1000.0}, {500.0, 500.0, 1000.0},
        {500.0, 0.0, 1000.0},{500.0, 0.0, 1.0}, {500.0, 500.0, 1.0}, {0.0, 500.0, 1.0},
        {0.0, 0.0, 1.0}, {0.0, 250.0, 1000.0}, {500.0, 250.0, 1000.0},
        {0.0, 250.0, 1.0}, {500.0, 250.0, 1.0}};
    
    glPushMatrix();
    
    glDepthMask(0);
    
    /*Boards*/
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, imageID[0]);
    
    //Front face bottom half
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);
        glVertex3fv(vertices[0]);
        glTexCoord2f(0.0, 1.0);
        glVertex3fv(vertices[8]);
        glTexCoord2f(1.0, 1.0);
        glVertex3fv(vertices[9]);
        glTexCoord2f(1.0, 0.0);
        glVertex3fv(vertices[3]);
    glEnd();
    
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
    
    //Front face top half
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);
        glVertex3fv(vertices[8]);
        glTexCoord2f(0.0, 1.0);
        glVertex3fv(vertices[1]);
        glTexCoord2f(1.0, 1.0);
        glVertex3fv(vertices[2]);
        glTexCoord2f(1.0, 0.0);
        glVertex3fv(vertices[9]);
    glEnd();
    
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

/* resetPuck()
 *
 * Resets the puck to its initial position. Along with the puck's initial position, several
 * other parameters key in evaluating the puck's position and status (goal, no goal, post, etc.)
 * are reset as well.
 *
 */

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

/* checkGoal()
 *
 * The brain of my hockey mini game. The following checks are made:
 *      1. Checks if velocity is 0. If so, puck is reset and textNum is set to correspond to my 
 *         text array that stores all of my UI text.
 *      2. Checks if a goal is scored. If so, score is incremenetd, textNum is set, and puck is
 *         reset.
 *      3. Checks if puck hits crossbar. If so, textNum is set and puck is reset.
 *      4. Checks if puck hits left post. If so, textNum is set and puck is reset.
 *      5. Checks if puck hits right post. If so, textNum is set and puck is reset.
 *
 */

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

/* display()
 *
 * Our standard display function. Calls all the functions that draws our scene including:
 *      drawRink()
 *      drawGoal()
 *      drawShotMeter()
 *      drawPuck()
 *      moveStick() --> Calls drawStick()
 *      renderUI()
 *      renderPuckText() --> Only called when textNum is not at its default value of 999
 * 
 * Also, gluLookAt() is called to control the camera.
 *
 */


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

/* idle()
 *
 * My idle function does a couple of things:
 *      1. Checks if the puck is going out of bounds by checking puck's position stored
 *         in puckAtTime. If out of bonds, the puck is reset and textNum is set to 2. 
 *         If not out of bounds, checkGoal() is called to see if a goal is scored.
 *         (Note: I left out collision detection out of this version, but it's ont my to-do list)
 *      2. Controls the animation of the bar moving back and forth along the shot meter. Uses a
 *         reachedRight flag to decide a negative or positive translation along the shot meter 
 *         based on the value of power.
 *      3. Sets camera values based on value of puckCam set by the user pressing the 'c' key. 
 *      4. Rapidly flashes the 'Goals Scored' text in my UI yellow when a goal is scored.
 *
 */

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
    
    /* Change camera view
     * puckCam = 0 -> Stick cam
     * puckCam = 1 -> Puck cam
     * puckCam = 2 -> Goal cam
     *
     */
    if (puckCam == 1){
        viewer[0] = puckAtTime[0];
        viewer[1] = puckAtTime[1];
        viewer[2] = puckAtTime[2] - 25;
    } else if (puckCam == 2){
        viewer[0] = 250.0;
        viewer[1] = 100.0;
        viewer[2] = 210.0;
        reference[0] = puckAtTime [0];
        reference[1] = puckAtTime [1];
        reference[2] = puckAtTime [2];
    }
    else if (puckCam == 0){
        viewer[0] = 250.0;
        viewer[1] = 50.0;
        viewer[2] = 1000.0;
        reference[0] = 250.0;
        reference[1] = 50.0;
        reference[2] = 1.0;
    }
    
    /*
     * Flash 'Goals Scored' yellow if goal scored
     */
    
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

/* keys()
 *
 * Contains the controls of my game:
 *      r - Resets the puck and shot
 *      p - Stops power bar
 *      c - Changes camera view
 *      32(ASCII code for 'Space' key) - Shoot the puck after stopping shot power meter
 *
 * And a debug tool:
 *      l - Provides the position of the puck in the terminal output
 *
 */

void keys(unsigned char key, int x, int y)
{
    
    //Reset puck
    
    if (key == 'r'){
        resetPuck();
    }
    
    //Stop power bar
    
    if(key == 'p') {
        stopPowerBar = 1;
        velocity = (power/50.0)*250.0;
    }
    
    if (key == 'c'){
        puckCam += 1;
        if (puckCam > 2){
            puckCam = 0;
        }
    }
    
    //Shoot puck with space bar if power bar is stopped
    //(note: 32 is ASCII code for space)

    if(key == 32) {
        if (stopPowerBar == 1){
            puckShot = 1;
        }
    }
    
    //Used for debugging purposes to calculate the position of the puck on
    //click of 'l'
    
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

/* special()
 *
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

/* readPPM()
 *
 * Code to read in PPM files for texture mapping.
 *
 */

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

/* generateTextures()
 *
 * Generates textures after reading PPM files into the program. Called 
 * in main().
 *
 */

void generateTextures(int IDnum ) {
    glGenTextures(1, &imageID[IDnum]);
    glBindTexture(GL_TEXTURE_2D, imageID[IDnum]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, imageData);
    free(imageData);
    
}

/* main()
 *
 * Our main function that handles standard main() duties. Generates our textures 
 * and sets the values for the text rendered for puck status.
 *
 */

int main(int argc, char** argv)
{
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

