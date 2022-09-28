//*********************************************************************
//  Computer Graphic using OpenGL
//  by : Paulo Eduardo Ramos de Araujo
//  [02/10/2022]
//*********************************************************************

// LIBRARYS
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;

// VARIABLES
// -- Window
static int WindowSizeX = 720;
static int WindowSizeY = 720;

// -- Map
#define FLOOR 0
#define WALL 1
#define FOOD 2
#define POWER 3
#define DOOR 4
#define REDGHOST 5
#define BLUEGHOST 6
#define ORANGEGHOST 7
#define PURPLEGHOST 8
#define PACMAN 9

#define MapSizeX 20
#define MapSizeY 20

typedef struct {
	int gameMap[MapSizeY][MapSizeX];
	int foodCount;
	int 
	
}


// Program Functions
int main(int argc, char *argv[]);
static void reshape(int width, int height);
static void display(void);
static void drawObject(float column, float row, int object);
static void desenhaMapa(int gameMap[MapSizeY][MapSizeX]);
static void key(unsigned char key, int x, int y);

int main(int argc, char *argv[])
{
    glutInit(&argc, argv); // Init Glut
    glutInitWindowSize(WindowSizeX, WindowSizeY); // Define Window Size
    glutInitWindowPosition(10,10); // Window Started Position
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); // Display Mode 
    glutCreateWindow("PacMan 3D - Pumba Developer"); // Create Window With Name

	readArchiveMap();
	
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);

    glClearColor(0,0,0,1);

    glutMainLoop();

    return EXIT_SUCCESS;
}

static void readArchiveMap() {
	
}


/* GLUT callback Handlers */

static void reshape(int w, int h)
{    
    glMatrixMode (GL_PROJECTION);
	glLoadIdentity();

	glViewport (0, 0, w, h);

	glMatrixMode (GL_MODELVIEW);
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clean Collor Buffer
	glLoadIdentity();
	
    // Draw The Map
    desenhaMapa(gameMap);

    glutSwapBuffers();
}


static void drawObject(float column, float row, int object) {
	float color[3];
	
	switch(object) {
		case 0: // Cinza
			color[0] = 64.0;
			color[1] = 64.0;
			color[2] = 64.0;
			break;
		case 1: // Azul
			color[0] = 0;
			color[1] = 0;
			color[2] = 255;
			break;
		case 2: // Pastilha
			color[0] = 95.0;
			color[1] = 159.0;
			color[2] = 159.0;
			break;
		case 3:
			color[0] = 127.0;
			color[1] = 0.0;
			color[2] = 255.0;
			break;
		case 4:
			color[0] = 254.0;
			color[1] = 15.0;
			color[2] = 92.0;
			break;
		case 5:
			color[0] = 254.0;
			color[1] = 0.0;
			color[2] = 0.0;
			break;
		case 6:
			color[0] = 254.0;
			color[1] = 0.0;
			color[2] = 0.0;
			break;
		case 7:
			color[0] = 254.0;
			color[1] = 0.0;
			color[2] = 0.0;
			break;
		case 8:
			color[0] = 254.0;
			color[1] = 0.0;
			color[2] = 0.0;
			break;
		case 9:
			color[0] = 255.0;
			color[1] = 255.0;
			color[2] = 0.0;
			break;
	}
	
	
	glPushMatrix();
		glTranslatef (column + TAM, row + TAM, 0.0);
		glColor3f(color[0], color[1], color[2]);
		glutSolidCube (TAM);
	glPopMatrix();
	
	
}


static void desenhaMapa(int gameMap[MapSizeY][MapSizeX]){
	int i, j;
	for(i=0; i < MapSizeY; i++)
		for(j=0; j < MapSizeX; j++)
			drawObject(MAT2X(j), MAT2Y(i), gameMap[i][j]);
}


static void key(unsigned char key, int x, int y)
{
    switch (key) 
    {
        case 27 : 
        case 'q':
            exit(0);
            break;
    }

    glutPostRedisplay();
}
