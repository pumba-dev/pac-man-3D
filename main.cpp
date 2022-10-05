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
#include <time.h>
using namespace std;
#include <conio.h>
#include <fstream>
#include "Textura.h"
#include "GameObjectDesigner.h"


// VARIABLES
// -- Window
#define WindowSizeX 720
#define WindowSizeY 720

// -- Game Dificulty [1..10]
#define GAMELEVEL 9

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
	int doorX, doorY;
	bool gameOver;

} PacmanGame;

typedef struct {
	int x, y;
	int points;
	bool invencible;
	bool dead;
	int objectBelowPlayer;
	bool hasStarted;
} Player;

PacmanGame gameEngine;
Player pacmanPlayer;
Player ghostPlayer[4];

// -- Textures

Textura gameTex[9];

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
void moveGhostRandomly(int ghostID);
void initLighting();
void initCullFace();
int randNumber(int min, int max);
void checkWinCondition();
void checkPacmanDead();
void knockDownGameDoor();
void initGame();

// Main
int main(int argc, char * argv[]) {
	glutInit( & argc, argv); // Init Glut
	glutInitWindowSize(WindowSizeX, WindowSizeY); // Define Window Size
	glutInitWindowPosition(10, 10); // Window Started Position
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); // Display Mode
	glutCreateWindow("PacMan 3D - Pumba Developer"); // Create Window With Name
	glClearColor(0, 0, 0, 1); // Window Background Color

	initCullFace();
	initLighting();
	initGame();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutTimerFunc(100, clockFunction, 1);

	glutMainLoop();

	return EXIT_SUCCESS;
}

void initLighting() {
	// -- GL_LIGHTING
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	float globalAmb[] = { 0.1f, 0.1f, 0.1f, 1.f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmb);

	float light0[4][4] = {
		{0.1f, 0.1f, 0.1f, 1.f}, // ambient
		{0.8f, 0.8f, 0.8f, 1.f}, // diffuse
		{ 1.f,  1.f,  1.f, 1.f }, // speculargi
		{0.f, 0.f, 1.f, 1.f}    // position
	};

	glLightfv(GL_LIGHT0, GL_AMBIENT,  & light0[0][0]);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  & light0[1][0]);
	glLightfv(GL_LIGHT0, GL_SPECULAR, & light0[2][0]);
	glLightfv(GL_LIGHT0, GL_POSITION, & light0[3][0]);
}

void initCullFace() {
	// -- Face Culling
	glEnable(GL_DEPTH_TEST); // Enable Z-buffer Algorithm
	glEnable(GL_CULL_FACE); // Enable Face Culling
	glCullFace(GL_BACK); // Cull Face = Clock Wise (Back)
}

void initGame() {
	// Reset Game Data
	gameEngine.foodCount = 0;
	pacmanPlayer.points = 0;
	pacmanPlayer.invencible = false;
	pacmanPlayer.dead = false;

	// Set and Load Game Map
	gameEngine.gameMapName = "pacman-map-01.txt";
	readArchiveMap(gameEngine.gameMapName);
	showGameMap();

	// Open The Doors
	knockDownGameDoor();
}


void clockFunction(int clock) {
	printf("## Clock %d ##\n", clock);

	// Move Ghost
	int sortedGhost = randNumber(0, 3);
	moveGhostRandomly(sortedGhost);

	int clockTime = (1000.0 / GAMELEVEL);
	glutTimerFunc(clockTime, clockFunction, clock + 1);
}

void knockDownGameDoor() {
	int doorRow = gameEngine.doorY;
	int doorColumn = gameEngine.doorX;
	int doorPositionObject = gameEngine.gameMap[doorRow][doorColumn];
	if(doorPositionObject == DOOR) {
		gameEngine.gameMap[doorRow][doorColumn] = FLOOR;
	} else {
		cout << "Erro ao derrubar as portas para iniciar o jogo!!" << endl;
	}
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

					// verify Objects and Set Params
					if(objectNumber == FOOD) {
						gameEngine.foodCount += 1;
					} else if(objectNumber == PACMAN) {
						pacmanPlayer.x = column;
						pacmanPlayer.y = row;
					} else if (objectNumber == DOOR) {
						gameEngine.doorX = column;
						gameEngine.doorY = row;
					} else if (objectNumber == REDGHOST) {
						ghostPlayer[0].x = column;
						ghostPlayer[0].y = row;
						ghostPlayer[0].hasStarted = true;
						ghostPlayer[0].objectBelowPlayer = 0;
					} else if (objectNumber == BLUEGHOST) {
						ghostPlayer[1].x = column;
						ghostPlayer[1].y = row;
						ghostPlayer[1].hasStarted = true;
						ghostPlayer[1].objectBelowPlayer = 0;
					} else if (objectNumber == PURPLEGHOST) {
						ghostPlayer[2].x = column;
						ghostPlayer[2].y = row;
						ghostPlayer[2].hasStarted = true;
						ghostPlayer[2].objectBelowPlayer = 0;
					} else if (objectNumber == ORANGEGHOST) {
						ghostPlayer[3].x = column;
						ghostPlayer[3].y = row;
						ghostPlayer[3].hasStarted = true;
						ghostPlayer[3].objectBelowPlayer = 0;
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
	// Evitar Divis�o por Zero
	if(h == 0) h = 1;

	glViewport (0, 0, w, h); // Viewport Dimensions

	// Init Coord System
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float aspect = (float)w / (float)h;
	gluPerspective(60, aspect, 1.0, 20.0);

	gluLookAt(0.0, -1.0, 2.5, 	// posição da câmera (olho)
			  0.0, 0.0, 0.0, 	// centro da cena
			  0.0, 1.0, 0.0); // direção de cima

	glMatrixMode (GL_MODELVIEW);
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clean Collor Buffer
	glLoadIdentity(); // Load Identity Matriz

	// -- Draw The Map And Objects
	desenhaMapa();

	checkPacmanDead();
	checkWinCondition();

	glutSwapBuffers();
}

void checkWinCondition() {
	if(pacmanPlayer.points == gameEngine.foodCount) {
		gameEngine.gameOver = true;
	}
}

void checkPacmanDead() {
	for(int i = 0 ; i < 4 ; i++) {
		if(ghostPlayer[i].objectBelowPlayer == PACMAN) {
			gameEngine.gameOver = true;
		}
	}
}

void drawObject(float column, float row, int object) {
	glPushMatrix();
	glTranslatef (column + TAM, row + TAM, 0.0);

	switch(object) {
	case FLOOR: {
		// Nothing
		break;
	}
	case WALL: {
		float color[3] = {0.0, 0.0, 220.0};
		desenhaCubo( & gameTex[WALL], TAM, color);
		break;
	}
	case FOOD:
		glColor3f(0.8, 0.8, 0.8);
		glutSolidSphere(TAM * 0.2, 30, 30);
		break;
	case POWER:
		glColor3f(0.8, 0.8, 0.8);
		glutSolidSphere(TAM * 0.4, 30, 30);
		break;
	case DOOR: {
		float color[3] = {205, 127, 50};
		desenhaCubo( & gameTex[DOOR], TAM, color);
		break;
	}
	case REDGHOST:
		glColor3f(0.8, 0, 0);
		glutSolidSphere(TAM * 0.6, 30, 30);
		break;
	case BLUEGHOST:
		glColor3f(0.8, 0.2, 0.2);
		glutSolidSphere(TAM * 0.6, 30, 30);
		break;
	case ORANGEGHOST:
		glColor3f(0.8, 0.2, 0);
		glutSolidSphere(TAM * 0.6, 30, 30);
		break;
	case PURPLEGHOST:
		glColor3f(0.8, 0, 0.2);
		glutSolidSphere(TAM * 0.6, 30, 30);
		break;
	case PACMAN:
		glColor3f(1, 1, 0);
		glutSolidSphere(TAM * 0.75, 30, 30);
		break;
	}

	glPopMatrix();
}


void desenhaMapa() {
	// Set and Load Textures
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	gameTex[WALL].load("textures/brick.png");
	gameTex[DOOR].load("textures/door.png");

	for(int i = 0; i < MAPSIZE; i++)
		for(int j = 0; j < MAPSIZE; j++) {
			// drawObject(MAT2X(j), MAT2Y(i), gameEngine.gameMap[i][j]);
			drawObject(MAT2X(j), MAT2Y(i), gameEngine.gameMap[i][j]);
		}
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

int randNumber(int min, int max) {
	static bool first = true;
	if (first) {
		srand( time(NULL) ); //seeding for the first time only!
		first = false;
	}
	return min + rand() % (( max + 1 ) - min);
}

void moveGhostRandomly(int ghostID) {
	// Check if Game Over
	if(gameEngine.gameOver) return;

	// Check if Ghost Exist
	if(!ghostPlayer[ghostID].hasStarted) return;

	// Variables
	int ghostObjectId = ghostID + 5;
	Coord lastGhostCoord = {ghostPlayer[ghostID].x, ghostPlayer[ghostID].y};

	Coord nextMove[4] = {
		{ghostPlayer[ghostID].x, ghostPlayer[ghostID].y - 1},
		{ghostPlayer[ghostID].x - 1, ghostPlayer[ghostID].y},
		{ghostPlayer[ghostID].x, ghostPlayer[ghostID].y + 1},
		{ghostPlayer[ghostID].x + 1, ghostPlayer[ghostID].y}
	}; // 0 - North | 1 - Left | 2 - South | 3 - Left

	int nextObject[4] = {
		gameEngine.gameMap[nextMove[0].y][nextMove[0].x],
		gameEngine.gameMap[nextMove[1].y][nextMove[1].x],
		gameEngine.gameMap[nextMove[2].y][nextMove[2].x],
		gameEngine.gameMap[nextMove[3].y][nextMove[3].x]
	};  // 0 - North | 1 - Left | 2 - South | 3 - Left

	// Check If Is Prisoned
	int possibleMoves = 0;
	for(int i = 0; i < 4 ; i++) {
		if(nextObject[i] == FLOOR || nextObject[i] == FOOD || nextObject[i] == POWER || nextObject[i] == PACMAN)
			possibleMoves += 1;
	}
	if(possibleMoves == 0) {
		cout << "Ghost " << ghostID << " is Prisoned!!" << endl;
		return;
	}

	// If Freedom, Generate Valid Random Move
	int randMove;
	do {
		randMove = randNumber(0, 3);

	} while (
		nextObject[randMove] == WALL ||
		nextObject[randMove] == REDGHOST ||
		nextObject[randMove] == ORANGEGHOST ||
		nextObject[randMove] == PURPLEGHOST ||
		nextObject[randMove] == BLUEGHOST
	);

	cout << "Ghost " << ghostObjectId << " Rand Move = " << randMove << " Next Object = " << nextObject[randMove] << endl;
	cout << "Ghost Object Below = " << ghostPlayer[ghostID].objectBelowPlayer << " Next Object Below Ghost = " << gameEngine.gameMap[nextMove[randMove].y][nextMove[randMove].x] << endl;
	cout << "Ghost Position: (" << lastGhostCoord.x << ", " << lastGhostCoord.y << ") New Ghost Position = " << nextMove[randMove].x << "-" << nextMove[randMove].y << endl;


	// Update Next Ghost Location on Map and Ghost Coord Params
	gameEngine.gameMap[nextMove[randMove].y][nextMove[randMove].x] = ghostObjectId;

	gameEngine.gameMap[lastGhostCoord.y][lastGhostCoord.x] = ghostPlayer[ghostID].objectBelowPlayer;

	ghostPlayer[ghostID].objectBelowPlayer = nextObject[randMove];
	ghostPlayer[ghostID].x = nextMove[randMove].x;
	ghostPlayer[ghostID].y = nextMove[randMove].y;


	// Redisplay Objects In New Position
	glutPostRedisplay();
}

void movePacman(int moveCode) {
	// Check if Game Over
	if(gameEngine.gameOver) return;

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


