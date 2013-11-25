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
#define BOARDS "boards.ppm"

GLUquadricObj *quad;
GLfloat viewer[3] = {250.0, 50.0, 1000.0};
GLfloat reference[3] = {250.0, 50.0, 1.0};
GLfloat puck[3];
GLfloat initPuck[3] = {250.0, 10.0, 900.0};
GLuint imageID[6];
int width, height;
GLubyte *imageData;
typedef GLfloat point3[3];
point3 meter[2] = {{230.0, 150.0, 800.0}, {230.0, 190.0, 800.0}};
GLfloat time = 0.0;
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

void shootPuck (GLfloat swivelDegrees, GLfloat tiltDegrees, GLfloat length, GLfloat velocity, GLfloat time){
    GLfloat g = 9.8; //gravity
    GLfloat swivelRadians = (swivelDegrees) * (M_PI/180);
    GLfloat tiltRadians = (tiltDegrees) * (M_PI/180);
    GLfloat puckTipY = (length) * sin(tiltRadians);
    GLfloat puckProjection = (length) * cos(tiltRadians);
    GLfloat puckTipX = (puckProjection) * cos(swivelRadians);
    GLfloat puckTipZ = (puckProjection) * sin(swivelRadians);
    GLfloat puckCompX = puckTipX - 0.0;
    GLfloat puckCompY = puckTipY - 0.0;
    GLfloat puckCompZ = puckTipZ - 0.0;
    GLfloat puckDirCosX = puckCompX/length;
    GLfloat puckDirCosY = puckCompY/length;
    GLfloat puckDirCosZ = puckCompZ/length;
    
    //Solve velocity components
    GLfloat puckVelX = puckDirCosX * velocity;
    GLfloat puckVelY = puckDirCosY * velocity;
    GLfloat puckVelZ = puckDirCosZ * velocity;
    
    //Solve position at time
    
    puck[0] = puckTipX + (puckVelX * time);
    puck[1] = puckTipY + (puckVelY * time) + ((1/2)*(g)*powf(time, 2.0));
    puck[2] = puckTipZ + (puckVelZ * time);
    
}


void drawPuck(void){
    
    shootPuck(20.0, 0.0, 10, 20.0, time); //make swivel and tilt degrees global
    initPuck[0] = puck[0];
    initPuck[1] = puck[1];
    initPuck[2] = puck[2];
    
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
    
    
}

void drawStick(void){
    glPushMatrix();
    
    
    glPopMatrix();
}

void drawShotMeter(void){
    int i;
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
    
    //Black bar that will move back and forth
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
    drawPuck();
    drawShotMeter();
    
    
    glFlush();
	glutSwapBuffers(); /*Display next buffer*/
}

void idle(void) {
	//Put code for power meter here
    
    if (time < 1.0 && puckShot == 1){
        time += 0.02;
    }
    
	display();
}

void keys(unsigned char key, int x, int y)
{
    int i;
    
	if(key == 'x') viewer[0] -= 1.0;
	if(key == 'X') viewer[0] += 1.0;
	if(key == 'y') viewer[1] -= 1.0;
	if(key == 'Y') viewer[1] += 1.0;
	if(key == 'z') viewer[2] -= 1.0;
	if(key == 'Z') viewer[2] += 1.0;
    
    
    if(key == 'p') {
        puckShot = 1;
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
    readPPM(BOARDS);
    generateTextures(2);
    myinit();
	glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keys);
	glutMainLoop();
	
	return 0;
}

