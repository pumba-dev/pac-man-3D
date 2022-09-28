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
#include <sstream>
using namespace std;
#include <conio.h>
#include <fstream>


// VARIABLES
// -- Window
#define WindowSizeX 720
#define WindowSizeY 720

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

#define MAPSIZE 20
#define TAM 0.1f
#define MAT2X(j) ((j)*0.1f-1)
#define MAT2Y(i) (0.9-(i)*0.1f)

// -- GamePlay
typedef struct {
	int gameMap[MAPSIZE][MAPSIZE];
	int foodCount;
	string gameMapName;
} pacmanGame;

pacmanGame gameEngine;


// Program Functions
int main(int argc, char * argv[]);
void clockFunction(int clock);
void reshape(int width, int height);
void display(void);
void drawObject(float column, float row, int object);
void desenhaMapa();
void key(unsigned char key, int x, int y);
void readArchiveMap(string mapName);
void showGameMap();

// Main
int main(int argc, char * argv[]) {
	glutInit( & argc, argv); // Init Glut
	glutInitWindowSize(WindowSizeX, WindowSizeY); // Define Window Size
	glutInitWindowPosition(10, 10); // Window Started Position
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // Display Mode
	glutCreateWindow("PacMan 3D - Pumba Developer"); // Create Window With Name
	glClearColor(0, 0, 0, 1); // Window Background Color

	gameEngine.gameMapName = "pacman-map-01.txt";
	readArchiveMap(gameEngine.gameMapName);
	showGameMap();
	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutTimerFunc(100, clockFunction, 1);

	glutMainLoop();

	return EXIT_SUCCESS;
}

void clockFunction(int clock) {
	// printf("## Clock %d ##\n", clock);

	glutTimerFunc(100, clockFunction, clock + 1);
}

void readArchiveMap(string mapName) {
	char charMapName[mapName.length() + 1]; // Create Array of Char For Map Name
	strcpy(charMapName, mapName.c_str()); // Copy String into Array of Char

	ifstream archiveMap; // Create Archive Stream Object
	archiveMap.open(charMapName); // Open Archive
	string archiveCurrentLine; // Variable To Save Current Line

	if(archiveMap.is_open()) {
		cout << "Arquivo de Mapa Aberto com Sucesso!\n" << endl;

		int row = 0;
		int column = 0;
		while(!archiveMap.eof()) {
			getline(archiveMap, archiveCurrentLine);
			// cout << archiveCurrentLine << endl;

			for(int i = 0; i < (int)archiveCurrentLine.length(); i++) { // add line to gamemap row;
				char str = archiveCurrentLine[i]; // convert to char
				int objectNumber = str - '0'; // convert chart ascii to integer

				if((int)str != 9) { // verify if is a empty value
					gameEngine.gameMap[row][column] = objectNumber;
					//cout << "index:: " << row << "|" <<  column << " || objectNumber:: " << objectNumber << " || gameMap:: " << gameEngine.gameMap[row][column] << endl;
					column += 1;
				}
			}
			
			// Protect Matriz Size
			if(row >= MAPSIZE -1 && column >= MAPSIZE -1) {
				cout << "CABO PORRA" << endl;
				break;
			} else {
				row += 1;
				column = 0;	
			}		
		}
		archiveMap.close();
		cout << "Arquivo de Salvo com Sucesso!\n" << endl;
	} else {
		cout << "Problemas na Abertura do Arquivo de Mapa!\n" << endl;
	}
}

void reshape(int w, int h) {
	// Evita a divisao por zero
	if(h == 0) h = 1;

	glViewport (0, 0, w, h); // Viewport Dimensions

	// Init Coord System
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


	gluPerspective(60, (float)w / (float)h, 1.0, 20.0);
	gluLookAt(0.0, -1.0, 2.5, 	// posição da câmera (olho)
			  0.0, 0.0, 0.0, 	// centro da cena
			  0.0, 1.0, 0.0); // direção de cima

	glMatrixMode (GL_MODELVIEW);
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clean Collor Buffer
	glLoadIdentity();

	// Draw The Map
	desenhaMapa();

	glutSwapBuffers();
}


void drawObject(float column, float row, int object) {
	float color[3];

	glPushMatrix();
	glTranslatef (column + TAM, row + TAM, 0.0);
	
	switch(object) {
	case 0: // Cinza
		glColor3f(112, 112, 112);
		glutSolidCube(TAM);
		break;
	case 1: // Azul
		glColor3f(0.0, 0.0, 255.0);
		glutSolidCube(TAM);
		break;
	case 2: // Pastilha
		glColor3f(95, 159, 159);
		glutSolidSphere(TAM*0.2, 30, 30);
		break;
	case 3: // Super Power
		glColor3f(130, 130, 150);
		glutSolidSphere(TAM*0.4, 30, 30); 
		break;
	case 4: // Door
		glColor3f(205, 127, 50); 
		glutSolidCube(TAM);
		break;
	case 5:
		glColor3f(230, 0, 0); 
		glutSolidSphere(TAM*0.75, 30, 30);
		break;
	case 6:
		glColor3f(230, 0, 0); 
		glutSolidSphere(TAM*0.75, 30, 30);
		break;
	case 7:
		glColor3f(230, 0, 0); 
		glutSolidSphere(TAM*0.75, 30, 30);
		break;
	case 8:
		glColor3f(230, 0, 0); 
		glutSolidSphere(TAM*0.75, 30, 30);
		break;
	case 9:
		glColor3f(230, 230, 0); 
		glutSolidSphere(TAM*0.75, 30, 30);
		break;
	}
	
	glPopMatrix();
}


void desenhaMapa() {
	for(int i = 0; i < MAPSIZE; i++)
		for(int j = 0; j < MAPSIZE; j++)
			drawObject(MAT2X(j), MAT2Y(i), gameEngine.gameMap[i][j]);
}


void key(unsigned char key, int x, int y) {
	switch (key) {
	case 27 :
	case 'q':
		exit(0);
		break;
	}

	glutPostRedisplay();
}

void showGameMap() {
	cout << "########## GAME MAP ###########" << endl;
	for(int i = 0; i < MAPSIZE; i++) {
		for(int j = 0; j < MAPSIZE; j++) {
			cout << gameEngine.gameMap[i][j] << " ";
		}
		cout << endl;
	}
	cout << "###############################" << endl;
}


