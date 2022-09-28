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
	int x, y;
} Coord;

typedef struct {
	int gameMap[MAPSIZE][MAPSIZE];
	int foodCount;
	string gameMapName;

} PacmanGame;

typedef struct {
	int x, y;
	int points;
	bool invencible;
} Player;

PacmanGame gameEngine;
Player pacmanPlayer;

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
void movePacman(int moveCode);
void initGame();

// Main
int main(int argc, char * argv[]) {
	glutInit( & argc, argv); // Init Glut
	glutInitWindowSize(WindowSizeX, WindowSizeY); // Define Window Size
	glutInitWindowPosition(10, 10); // Window Started Position
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // Display Mode
	glutCreateWindow("PacMan 3D - Pumba Developer"); // Create Window With Name
	glClearColor(0, 0, 0, 1); // Window Background Color

	initGame();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutTimerFunc(100, clockFunction, 1);

	glutMainLoop();

	return EXIT_SUCCESS;
}

void initGame() {
	gameEngine.foodCount = 0;
	pacmanPlayer.points = 0;
	gameEngine.gameMapName = "pacman-map-01.txt";
	readArchiveMap(gameEngine.gameMapName);
	showGameMap();
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
					// cout << "index:: " << row << "|" <<  column << " || objectNumber:: " << objectNumber << " || gameMap:: " << gameEngine.gameMap[row][column] << endl;

					// verify foods and count
					if(objectNumber == FOOD) {
						gameEngine.foodCount += 1;
					}

					// verify and set pacman position
					if(objectNumber == PACMAN) {
						pacmanPlayer.x = column;
						pacmanPlayer.y = row;
					}

					column += 1; // Incremente Column
				}
			}

			// Protect Matriz Size
			if(row >= MAPSIZE - 1 && column >= MAPSIZE - 1) {
				// cout << "CABO PORRA" << endl;
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
	glPushMatrix();
	glTranslatef (column + TAM, row + TAM, 0.0);

	switch(object) {
	case 0:
		//glColor3f(0, 0, 0);
		//glutSolidCube(TAM);
		break;
	case 1:
		glColor3f(0.0, 0.0, 255.0);
		glutSolidCube(TAM);
		break;
	case 2:
		glColor3f(95, 159, 159);
		glutSolidSphere(TAM * 0.2, 30, 30);
		break;
	case 3:
		glColor3f(130, 130, 150);
		glutSolidSphere(TAM * 0.4, 30, 30);
		break;
	case 4:
		glColor3f(205, 127, 50);
		glutSolidCube(TAM);
		break;
	case 5:
		glColor3f(230, 0, 0);
		glutSolidSphere(TAM * 0.75, 30, 30);
		break;
	case 6:
		glColor3f(230, 0, 0);
		glutSolidSphere(TAM * 0.75, 30, 30);
		break;
	case 7:
		glColor3f(230, 0, 0);
		glutSolidSphere(TAM * 0.75, 30, 30);
		break;
	case 8:
		glColor3f(230, 0, 0);
		glutSolidSphere(TAM * 0.75, 30, 30);
		break;
	case 9:
		glColor3f(230, 230, 0);
		glutSolidSphere(TAM * 0.75, 30, 30);
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
		exit(0);
		break;
	case 'w':
		movePacman(0); // up
		break;
	case 'a':
		movePacman(1); // left
		break;
	case 's':
		movePacman(2); // down
		break;
	case 'd':
		movePacman(3); // right
		break;

	}

	glutPostRedisplay();
}


void movePacman(int moveCode) {
	Coord north = {pacmanPlayer.x, pacmanPlayer.y - 1};
	Coord left = {pacmanPlayer.x - 1, pacmanPlayer.y};
	Coord down = {pacmanPlayer.x, pacmanPlayer.y + 1};
	Coord right = {pacmanPlayer.x + 1, pacmanPlayer.y};

	switch(moveCode) {
	case 0:
		if(gameEngine.gameMap[north.y][north.x] == WALL) {
			cout << "Walking Upper Forbidden || WALL" << endl;
		} else if(gameEngine.gameMap[north.y][north.x] == FLOOR) {
			cout << "Walking Upper || FLOOR" << endl;
			// Change Map Object
			gameEngine.gameMap[pacmanPlayer.y][pacmanPlayer.x] = FLOOR; // current position = floor
			gameEngine.gameMap[north.y][north.x] = PACMAN; // next position = pacman
			// Set New Pacman Position
			pacmanPlayer.x = north.x;
			pacmanPlayer.y = north.y;
		} else if (gameEngine.gameMap[north.y][north.x] == FOOD) {
			cout << "Walking Upper || FOOD" << endl;
			// Change Map Object
			gameEngine.gameMap[pacmanPlayer.y][pacmanPlayer.x] = FLOOR; // current position = floor
			gameEngine.gameMap[north.y][north.x] = PACMAN; // next position = pacman
			// Set New Pacman Position
			pacmanPlayer.x = north.x;
			pacmanPlayer.y = north.y;
			// Add Point to Pacman
			pacmanPlayer.points += 1;
		} else if (gameEngine.gameMap[north.y][north.x] == POWER) {
			cout << "Walking Upper || POWER" << endl;
			// Change Map Object
			gameEngine.gameMap[pacmanPlayer.y][pacmanPlayer.x] = FLOOR; // current position = floor
			gameEngine.gameMap[north.y][north.x] = PACMAN; // next position = pacman
			// Set New Pacman Position
			pacmanPlayer.x = north.x;
			pacmanPlayer.y = north.y;
			// Set PacMan Invensible
			pacmanPlayer.invencible = true;
		}
		break;
	case 1:
		if(gameEngine.gameMap[left.y][left.x] == WALL) {
			cout << "Walking Left Forbidden || WALL" << endl;
		} else if(gameEngine.gameMap[left.y][left.x] == FLOOR) {
			cout << "Walking Left || FLOOR" << endl;
			// Change Map Object
			gameEngine.gameMap[pacmanPlayer.y][pacmanPlayer.x] = FLOOR; // current position = floor
			gameEngine.gameMap[left.y][left.x] = PACMAN; // next position = pacman
			// Set New Pacman Position
			pacmanPlayer.x = left.x;
			pacmanPlayer.y = left.y;
		} else if (gameEngine.gameMap[left.y][left.x] == FOOD) {
			cout << "Walking Left || FOOD" << endl;
			// Change Map Object
			gameEngine.gameMap[pacmanPlayer.y][pacmanPlayer.x] = FLOOR; // current position = floor
			gameEngine.gameMap[left.y][left.x] = PACMAN; // next position = pacman
			// Set New Pacman Position
			pacmanPlayer.x = left.x;
			pacmanPlayer.y = left.y;
			// Add Point to Pacman
			pacmanPlayer.points += 1;
		} else if (gameEngine.gameMap[left.y][left.x] == POWER) {
			cout << "Walking Left || POWER" << endl;
			// Change Map Object
			gameEngine.gameMap[pacmanPlayer.y][pacmanPlayer.x] = FLOOR; // current position = floor
			gameEngine.gameMap[left.y][left.x] = PACMAN; // next position = pacman
			// Set New Pacman Position
			pacmanPlayer.x = left.x;
			pacmanPlayer.y = left.y;
			// Set PacMan Invensible
			pacmanPlayer.invencible = true;
		}
		break;
	case 2:
		if(gameEngine.gameMap[down.y][down.x] == WALL) {
			cout << "Walking Down Forbidden || WALL" << endl;
		} else if(gameEngine.gameMap[down.y][down.x] == FLOOR) {
			cout << "Walking Down || FLOOR" << endl;
			// Change Map Object
			gameEngine.gameMap[pacmanPlayer.y][pacmanPlayer.x] = FLOOR; // current position = floor
			gameEngine.gameMap[down.y][down.x] = PACMAN; // next position = pacman
			// Set New Pacman Position
			pacmanPlayer.x = down.x;
			pacmanPlayer.y = down.y;
		} else if (gameEngine.gameMap[down.y][down.x] == FOOD) {
			cout << "Walking Down || FOOD" << endl;
			// Change Map Object
			gameEngine.gameMap[pacmanPlayer.y][pacmanPlayer.x] = FLOOR; // current position = floor
			gameEngine.gameMap[down.y][down.x] = PACMAN; // next position = pacman
			// Set New Pacman Position
			pacmanPlayer.x = down.x;
			pacmanPlayer.y = down.y;
			// Add Point to Pacman
			pacmanPlayer.points += 1;
		} else if (gameEngine.gameMap[down.y][down.x] == POWER) {
			cout << "Walking Down || POWER" << endl;
			// Change Map Object
			gameEngine.gameMap[pacmanPlayer.y][pacmanPlayer.x] = FLOOR; // current position = floor
			gameEngine.gameMap[down.y][down.x] = PACMAN; // next position = pacman
			// Set New Pacman Position
			pacmanPlayer.x = down.x;
			pacmanPlayer.y = down.y;
			// Set PacMan Invensible
			pacmanPlayer.invencible = true;
		}
		break;
	case 3:
		if(gameEngine.gameMap[right.y][right.x] == WALL) {
			cout << "Walking Right Forbidden || WALL" << endl;
		} else if(gameEngine.gameMap[right.y][right.x] == FLOOR) {
			cout << "Walking Right || FLOOR" << endl;
			// Change Map Object
			gameEngine.gameMap[pacmanPlayer.y][pacmanPlayer.x] = FLOOR; // current position = floor
			gameEngine.gameMap[right.y][right.x] = PACMAN; // next position = pacman
			// Set New Pacman Position
			pacmanPlayer.x = right.x;
			pacmanPlayer.y = right.y;
		} else if (gameEngine.gameMap[right.y][right.x] == FOOD) {
			cout << "Walking Right || FOOD" << endl;
			// Change Map Object
			gameEngine.gameMap[pacmanPlayer.y][pacmanPlayer.x] = FLOOR; // current position = floor
			gameEngine.gameMap[right.y][right.x] = PACMAN; // next position = pacman
			// Set New Pacman Position
			pacmanPlayer.x = right.x;
			pacmanPlayer.y = right.y;
			// Add Point to Pacman
			pacmanPlayer.points += 1;
		} else if (gameEngine.gameMap[right.y][right.x] == POWER) {
			cout << "Walking Right || POWER" << endl;
			// Change Map Object
			gameEngine.gameMap[pacmanPlayer.y][pacmanPlayer.x] = FLOOR; // current position = floor
			gameEngine.gameMap[right.y][right.x] = PACMAN; // next position = pacman
			// Set New Pacman Position
			pacmanPlayer.x = right.x;
			pacmanPlayer.y = right.y;
			// Set PacMan Invensible
			pacmanPlayer.invencible = true;
		}
		break;
	}
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


