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
#define ICE "ice.ppm"
#define PUCK "puck.ppm"
#define NET "net.ppm"
#define BOARDS "boards.ppm"

GLUquadricObj *quad;
GLfloat viewer[3] = {250.0, 50.0, 1000.0};
GLfloat reference[3] = {250.0, 50.0, 1.0};
GLfloat puckAtTime[3]; //puck at time t
GLfloat initPuck[3] = {250.0, 10.0, 900.0}; //initial puck coordinates
GLfloat startPuck[3] = {250.0, 10.0, 900.0}; //starting position for puck
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

void myinit(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, 1.0, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
    quad = gluNewQuadric();
    gluQuadricDrawStyle(quad, GLU_FILL);
    gluQuadricOrientation(quad, GLU_INSIDE);
    gluQuadricNormals(quad, GLU_SMOOTH);

}

void shootPuck (GLfloat swivelDegrees, GLfloat tiltDegrees, GLfloat v, GLfloat t){
    GLfloat g = -9.8; //gravity
    GLfloat swivelRadians = (swivelDegrees) * (M_PI/180);
    GLfloat tiltRadians = (tiltDegrees) * (M_PI/180);
    GLfloat velCompY = v * sin(tiltRadians);
    GLfloat velCompX = v * cos(tiltRadians) * sin(swivelRadians);
    GLfloat velCompZ = (-v) * cos(tiltRadians) * cos(swivelRadians);
    
    //Solve position at time t
    
    puckAtTime[0] = startPuck[0] + (velCompX * t);
    puckAtTime[1] = startPuck[1] + (velCompY * t) + ((1/2)*(g)*powf(t, 2));
    puckAtTime[2] = startPuck[2] + (velCompZ * t);
    
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
    point3 vertices[8] = { {0.0, 0.0, 1000.0}, {0.0, 500.0, 1000.0}, {500.0, 500.0, 1000.0}, {500.0, 0.0, 1000.0},
        {500.0, 0.0, 1.0}, {500.0, 500.0, 1.0}, {0.0, 500.0, 1.0}, {0.0, 0.0, 1.0}};
    
    glPushMatrix();
    
    //Front face
    glBegin(GL_QUADS);
        glColor3f(0.0, 0.0, 0.0); //black
        glVertex3fv(vertices[0]);
        glVertex3fv(vertices[1]);
        glVertex3fv(vertices[2]);
        glVertex3fv(vertices[3]);
    glEnd();
    
    
    //Back face (note: facing this face)
    glBegin(GL_QUADS);
        glColor3f(0.0, 0.0, 1.0); //blue
        glVertex3fv(vertices[4]);
        glVertex3fv(vertices[5]);
        glVertex3fv(vertices[6]);
        glVertex3fv(vertices[7]);
    glEnd();
    
    //Left face
    glBegin(GL_QUADS);
        glColor3f(1.0, 0.0, 0.0); //red
        glVertex3fv(vertices[7]);
        glVertex3fv(vertices[6]);
        glVertex3fv(vertices[1]);
        glVertex3fv(vertices[0]);
    glEnd();
    
    //Right face
    glBegin(GL_QUADS);
        glColor3f(0.0, 1.0, 0.0); //green
        glVertex3fv(vertices[3]);
        glVertex3fv(vertices[2]);
        glVertex3fv(vertices[5]);
        glVertex3fv(vertices[4]);
    glEnd();
    
    //Bottom face i.e. the ice surface
    glEnable(GL_TEXTURE_2D);
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
    
    //Top face
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
        glColor3f(1.0, 0.0, 1.0); //purple
        glVertex3fv(vertices[1]);
        glVertex3fv(vertices[6]);
        glVertex3fv(vertices[5]);
        glVertex3fv(vertices[2]);
    glEnd();
    
    glPopMatrix();

    
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
    
    glFlush();
	glutSwapBuffers(); /*Display next buffer*/
}

void idle(void) {
	//Put code for power meter here
    
    /*
     * Increment time to render puck at various times in its trajectory
     */
    if (time < 2.0 && puckShot == 1){
        time += 0.02;
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
    
	display();
}

void keys(unsigned char key, int x, int y)
{
    int i;
    
	if(key == 'x') viewer[0] -= 5.0;
	if(key == 'X') viewer[0] += 5.0;
	if(key == 'y') viewer[1] -= 5.0;
	if(key == 'Y') viewer[1] += 5.0;
	if(key == 'z') viewer[2] -= 20.0;
	if(key == 'Z') viewer[2] += 20.0;
    
    //Stop power bar
    if(key == 'p') {
        stopPowerBar = 1;
        velocity = (power/50.0)*100.0;
    }
    
    //Shoot puck with space bar (note: 32 is ASCII code for space)
    if(key == 32) {
        puckShot = 1;
    }
   
	display();
}

/*
 * Used to implement special keys (e.g. arrow keys) in
 * OpenGL. In my case, I will be adjusting the swivel angle
 * with the left/right keys and the tilt angle with the 
 * up/down keys.
 */

void special(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_LEFT:
            swivel -= 1.0;
            break;
        case GLUT_KEY_RIGHT:
            swivel += 1.0;
            break;
        case GLUT_KEY_DOWN:
            tilt -= 1.0;
            break;
        case GLUT_KEY_UP:
            tilt += 1.0;
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
	glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000,1000);
	glutInitWindowPosition(50,50);
	glutCreateWindow("Hockey Guy");
    glEnable(GL_DEPTH_TEST);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    //readPPM(PUCK2);
    //generateTextures(0);
    readPPM(ICE);
    generateTextures(1);
    readPPM(NET);
    generateTextures(2);
    myinit();
	glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keys);
    glutSpecialFunc(special);
	glutMainLoop();
	
	return 0;
}

