#pragma once
#include<vector>
#include<cmath>
#include "Textura.h"

struct vector3d {
	float x, y, z;
	vector3d() : x(0.0), y(0.0), z(0.0) {}
	vector3d(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
};

void draw(vector3d p1, vector3d p2, vector3d p3, vector3d p4, float cor[3]) {
	glColor3f(cor[0], cor[1], cor[2]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3fv( & p1.x);
	glTexCoord2f(1.0, 0.0);
	glVertex3fv( & p2.x);
	glTexCoord2f(1.0, 1.0);
	glVertex3fv( & p3.x);
	glTexCoord2f(0.0, 1.0);
	glVertex3fv( & p4.x);
	glEnd();
}

void desenhaCubo(Textura * textura, float tam, float cor[3]) {
	float d = tam / 2.0;

	vector3d v1(-d, d, d);
	vector3d v2(-d, -d, d);
	vector3d v3(d, -d, d);
	vector3d v4(d, d, d);

	vector3d v5(d, d, -d);
	vector3d v6(d, -d, -d);
	vector3d v7(-d, -d, -d);
	vector3d v8(-d, d, -d);


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// Use Texture
	textura->addText();
	
	// Apply Specular Light
	float matSpecular[] = { 1.f, 1.f, 1.f, 1.f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, 128);
	
	// Front
	glNormal3f(0.f, 0.f, 1.f);
	draw(v1, v2, v3, v4, cor);

	// Right
	glNormal3f(1.f, 0.f, 0.f);
	draw(v4, v3, v6, v5, cor);

	// Back
	glNormal3f(0.f, 0.f, -1.f);
	draw(v5, v8, v7, v6, cor);

	// Left
	glNormal3f(-1.f, 0.f, 0.f);
	draw(v1, v8, v7, v2, cor);

	// Top
	glNormal3f(0.f, 1.f, 0.f);
	draw(v1, v4, v5, v8, cor);

	// Bot
	glNormal3f(0.f, -1.f, 0.f);
	draw(v2, v7, v6, v3, cor);

	textura->removeText();
	glEndList();
}
