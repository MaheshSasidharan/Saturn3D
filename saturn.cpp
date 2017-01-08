/* Program:: Saturn and it’s Revolution around the Sun */

#include <stdlib.h>
#include <glut.h>
#include <math.h>
#include <stdio.h>

/***Defined Values***/
#define EXIT 1 // Menu Item to Exit
#define SPACE 2 // Menu Item for Space
#define RESET 3 // Menu Item to Reset
#define CAMERA_LOCK 4 // Menu Item for Camera ON
#define PLAY_PAUSE 5 // Menu Item to Play
#define SHOW_ORBIT_HIDE_ORBIT 6 // Menu Item to Show Orbit
#define SHOW_TEXT_HIDE_TEXT 7 // Menu Item to Show Text
#define ENABLE_DISABLE_LIGHTING 8 // Menu Item to Enable or Disable lighting
#define LOOK_AT_SUN 9 // Menu to change Camera Lock to Sun
#define LOOK_AT_SATURN 10 // Menu to change Camera Lock to Saturn

#define SaturnRings 123 // Display List Value for Saturns Ring
#define SatOrb 456 // Display List Value for Saturn's Orbit

/***Global Variables***/
int lighting = 1; // Enable or Disable lighting
int play = 1; // Play Or Pause Animation
int view = 1; // Camera Lock
int lookatSun = 1; // Camera Locked to Sun else to Saturn
int text = 1; // Toggle Text
int showOrbit = 1; // Hide Or Show Saturn's Orbit
int speed = 200; // Speed of Rotation
int Orbit_Index = 4; // Initial Obit Index
static GLdouble Angle = 0.0; // Initial Angle
static GLdouble Saturn_Orbit[720][2]; // Array for Saturn's Orbit
static GLdouble Saturn_Moon_Orbit[720][2]; // Array for Saturn's Moon's Orbit
static GLdouble viewer[] = { 0, 5, 13 }; // Initial Camera Position

void drawText(GLdouble x, GLdouble y, GLdouble z, char* s)
{
    glRasterPos3f(x, y, z); //Specifies the raster position
    for (char* c = s; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
}

void printtext()
{
    glColor3f(1, 0, 1);
    drawText(-2, 11, -5, "THE SATURN");
    glColor3f(1, 1, 1);
    drawText(-11, 10, -5, "Hold  I  to Increase speed and  D  to Decrease speed");
    glColor3f(1, 1, 1);
    drawText(-11, 9, -5, "Press P to toggle Play or Pause animation");
    glColor3f(1, 1, 1);
    drawText(-11, 8, -5, "Press O to toggle Orbit view");
    glColor3f(1, 1, 1);
    drawText(-11, 7, -5, "Press x,X,y,Y,z or Z to change Camera position");
    glColor3f(1, 1, 1);
    drawText(-11, 6, -5, "Press R to Reset camera");
    glColor3f(1, 1, 1);
    drawText(-11, 5, -5, "Press Q to Quit");
    glColor3f(1, 1, 1);
    drawText(-11, 4, -5, "Press C to toggle Camera Lock");
    glColor3f(1, 1, 1);
    drawText(-11, 3, -5, "Press L to Enable or Disable Lighting");
    glColor3f(1, 0, 0);
    drawText(-11, 2, -5, "Right Click to enter Menu");
    glColor3f(0, 1, 0);
    drawText(-11, 1, -5, "Press T to Hide or View this information");
}

void generateOrbit()
{
    int i;
for(i=1; i
{
        Saturn_Orbit[i][0] = sin(i * 3.1416 / 180) * 8.5; // 6.5 and 5 to have elliptical orbit
        Saturn_Orbit[i][1] = cos(i * 3.1416 / 180) * 7.5;
}
 
 
for(i=1; i
{
        Saturn_Moon_Orbit[i][0] = sin(i * 3.1416 / 180) * 0.7;
        Saturn_Moon_Orbit[i][1] = cos(i * 3.1416 / 180) * 0.7;
}
}

void myInit(void)
{
    int i = 0;
    glClearColor(0, 0, 0, 0);

    glNewList(SaturnRings, GL_COMPILE); // Display List for Saturn's Rings
    glBegin(GL_QUAD_STRIP);
    glColor3f(0.2, 0.2, 0.5); // Saturns rings color followed by saturn's ring formation
for(i=0; i
{
        glVertex3f(sin(i * 3.1416 / 180) * .8, 0, cos(i * 3.1416 / 180) * .8); // Inner Ring Border
        glVertex3f(sin(i * 3.1416 / 180) * 1.2, 0, cos(i * 3.1416 / 180) * 1.2); // Outer Ring Border
}
glEnd();
glEndList();
 
glNewList(SatOrb,GL_COMPILE);	// Display List for Saturn's Orbit
glColor3f(1,1,1);
glPointSize(2);
glBegin(GL_POINTS);
for(i=1; i
{
        glVertex3f(sin(i * 3.1416 / 180) * 8.5, 0, cos(i * 3.1416 / 180) * 7.5);
}
glEnd();
glEndList();
 
 
// ***Lighting*** //
GLfloat diffuseMaterial[4] = { 1,1,1,1 };
GLfloat mat_specular[] = { 1,1,1,1 };
GLfloat light_position[] = { 0,0,0,1 };
GLfloat ambient[]={ 0,0,0,1 };
 
 
glShadeModel (GL_SMOOTH);
glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
glMaterialf(GL_FRONT, GL_SHININESS, 15.0);
glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
glLightfv(GL_LIGHT0, GL_POSITION, light_position);
glEnable(GL_LIGHT0);
glEnable(GL_LIGHTING);
glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
glEnable(GL_COLOR_MATERIAL);
}

void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    if (view) {
        if (lookatSun)
            gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); // Camera Locked at Sun
        else
            gluLookAt(viewer[0], viewer[1], viewer[2], Saturn_Orbit[Orbit_Index][0], 0, Saturn_Orbit[Orbit_Index][1], 0, 1, 0); //// Camera Locked at Saturn
    }
    else
        gluLookAt(viewer[0], viewer[1], viewer[2], viewer[0], viewer[1], 0.0, 0.0, 1.0, 0.0); // Camera Lock Off - Manual Mode

    if (lighting)
        glEnable(GL_LIGHTING);
    else
        glDisable(GL_LIGHTING);

    if (text)
        printtext();

    if (Orbit_Index > 720)
        Orbit_Index = 4; // Index Mapping Saturn's Orbit

    // ***Saturn Orbit*** //
    if (showOrbit)
        glCallList(SatOrb); // Call to Saturn's Orbit's Display List

    // ***SUN*** //
    glPushMatrix();
    glRotatef(Angle, 0.0, 1.0, 0.0); // Rotate Sun about its own axis
    glRotatef(90, 1.0, 0.0, 0.0); // Rotate Sphere to make it stand vertically, hence the sphere is rendered properly
    glColor3f(1.0, 1.0, 0.0); // Color Sun
    glutSolidSphere(1.5, 20, 20); // Generate Sun
    glPopMatrix();

    // ***SATURN*** //
    // --> Saturn Planet
    glPushMatrix();
    glTranslatef(Saturn_Orbit[Orbit_Index][0], 0, Saturn_Orbit[Orbit_Index][1]); // Translate Saturn to its Orbit
    glRotatef(Angle, 0, 1, 0); // Rotate Saturn about its own axis
    glRotatef(90, 1.0, 0.0, 0.0); // Rotate Sphere to make it stand vertically
    glColor3f(.33, .33, .33); // Color Saturn
    glutSolidSphere(0.5, 10, 10); // Generate Saturn
    glPopMatrix();

    // --> Saturn Rings
    glPushMatrix();
    glTranslatef(Saturn_Orbit[Orbit_Index][0], 0, Saturn_Orbit[Orbit_Index][1]); // Translate Rings to Saturns Orbit
    glRotatef(15, 0, 0, 1); // Angle to get the tilt of the rings
    glCallList(SaturnRings); // Call to Saturn's Ring's Display List
    glPopMatrix();

    // --> Saturn's Moon
    glPushMatrix();
    glTranslatef(Saturn_Moon_Orbit[Orbit_Index][1], 0, Saturn_Moon_Orbit[Orbit_Index][0]); // Then translate Moon into its Orbit around Saturn
    glTranslatef(Saturn_Orbit[Orbit_Index][0], 0, Saturn_Orbit[Orbit_Index][1]); // Translate First to Saturn's Orbit
    glRotatef(Angle, 0, 1, 0); // Rotate Moon about its own axis
    glRotatef(90, 1, 0, 0); // Rotate Sphere to make it stand vertically
    glColor3f(1, 0, 0); // Color the Moon
    glutSolidSphere(0.1, 10, 10); // Generate the Moon
    glPopMatrix();

    glPopMatrix();
    glutSwapBuffers();
    glFlush();
}

void animation(int)
{
    if (play) // Animate
    {
        Angle += 15.0; // Angle Increment
        Orbit_Index += 4; // Increase Orbit Index
        glutPostRedisplay();
        glutTimerFunc(speed, animation, 0);
    }
    else // Dont Animate
    {
        glutPostRedisplay();
        glutTimerFunc(speed, animation, 0);
    }
}

void keys(unsigned char key, int x, int y)
{
    if ((key == 'c') | (key == 'C')) {
        if (view == 0)
            view = 1; // Toggle Camera Lock
        else
            view = 0;
    }
    if ((key == 'i') | (key == 'I')) {
        if (speed == 0)
            speed = 0; // Increase animation speed
        else
            speed -= 10;
    }
    if ((key == 'd') | (key == 'D')) {
        if (speed == 1000)
            speed = 1000; // Decrease animation speed
        else
            speed += 10;
    }
    if ((key == 'o') | (key == 'O')) {
        if (showOrbit)
            showOrbit = 0; // Dont Show Orbit
        else
            showOrbit = 1; // Show Orbit
    }
    if ((key == 'p') | (key == 'P')) {
        if (play == 1)
            play = 0; // Pause
        else
            play = 1; // Play
    }
    if ((key == 't') | (key == 'T')) {
        if (text == 1)
            text = 0; // Hide Text
        else
            text = 1; // Show Text
    }
    if ((key == 'l') | (key == 'L')) {
        if (lighting == 1)
            lighting = 0; // Disable lighting
        else
            lighting = 1; // Enable lighting
    }
    if ((key == 'r') | (key == 'R')) {
        viewer[0] = 0;
        viewer[1] = 5;
        viewer[2] = 13;
    } // Reset Camera
    if (key == 'x')
        viewer[0] += 1.0; // Move Camera
    if (key == 'X')
        viewer[0] -= 1.0;
    if (key == 'y')
        viewer[1] += 1.0;
    if (key == 'Y')
        viewer[1] -= 1.0;
    if (key == 'z')
        viewer[2] += 1.0;
    if (key == 'Z')
        viewer[2] -= 1.0;
    if ((key == 'q') | (key == 'Q'))
        exit(0); // Exit
}

void menu_select(int item)
{
    switch (item) {
    case EXIT:
        exit(0);
        break;
    case PLAY_PAUSE:
        if (play)
            play = 0;
        else
            play = 1;
        break;
    case CAMERA_LOCK:
        if (view)
            view = 0;
        else
            view = 1;
        break;
    case SHOW_ORBIT_HIDE_ORBIT:
        if (showOrbit)
            showOrbit = 0;
        else
            showOrbit = 1;
        break;
    case SHOW_TEXT_HIDE_TEXT:
        if (text)
            text = 0;
        else
            text = 1;
        break;
    case ENABLE_DISABLE_LIGHTING:
        if (lighting)
            lighting = 0;
        else
            lighting = 1;
        break;
    case RESET:
        viewer[0] = 0;
        viewer[1] = 5;
        viewer[2] = 13;
        break;
    case LOOK_AT_SUN:
        lookatSun = 1;
        view = 1;
        break;
    case LOOK_AT_SATURN:
        lookatSun = 0;
        view = 1;
        break;
    }
}

void menu_info()
{
    glutAddMenuEntry("Play/Pause animation", PLAY_PAUSE);
    glutAddMenuEntry("Toggle Camera Lock", CAMERA_LOCK);
    glutAddMenuEntry("View/Hide Orbit", SHOW_ORBIT_HIDE_ORBIT);
    glutAddMenuEntry("View/Hide Text", SHOW_TEXT_HIDE_TEXT);
    glutAddMenuEntry("Enable/Disable Lighting", ENABLE_DISABLE_LIGHTING);
    glutAddMenuEntry("Reset Camera", RESET);
    glutAddMenuEntry("", SPACE);
    glutAddMenuEntry("Focus on Sun", LOOK_AT_SUN);
    glutAddMenuEntry("Focus on Saturn", LOOK_AT_SATURN);
    glutAddMenuEntry("", SPACE);
    glutAddMenuEntry("Exit", EXIT);
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1, 1, -1, 1, 1, 1500);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); // GLUT_DEPTH for 3D
    glutInitWindowSize(1980, 1080);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("OpenGL");
    myInit();
    glutDisplayFunc(myDisplay);
    glutReshapeFunc(reshape);
    generateOrbit();
    glutTimerFunc(speed, animation, 0);
    glutKeyboardFunc(keys);
    glutCreateMenu(menu_select);
    menu_info();
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glEnable(GL_DEPTH_TEST); // Hidden surface removal
    glutMainLoop();
}