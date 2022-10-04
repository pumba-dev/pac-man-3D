#include "Textura.h"
#include<iostream>
#include<Windows.h>
#include<gl/GL.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Textura::Textura():
	m_id(0),m_largura(0),
	m_altura(0),m_canais(0)
{
}

Textura::~Textura() {
	glDeleteTextures(1, & m_id);
}

void Textura::load(std::string filePath) {
	unsigned char * imgData; // Variable to Save Image

	stbi_set_flip_vertically_on_load(true); // Flip To Fix Position
	imgData = stbi_load(filePath.c_str(), & m_largura, & m_altura, & m_canais, 4); // Load Archive with yout width, height and
	if (imgData) {
		glGenTextures(1, & m_id); // Create Texture ID Space in Memory
		glBindTexture(GL_TEXTURE_2D, m_id); // Bind texture with texture ID
		// Config Texture Params
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_largura, m_altura, 0,
					 GL_RGBA, GL_UNSIGNED_BYTE, imgData);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Unbind
		glBindTexture(GL_TEXTURE_2D, 0);
		// Free Image
		stbi_image_free(imgData);
	} else {
		std::cout << "ERRO:: Nao foi possivel carregar a textura!" << filePath.c_str() << std::endl;
	}
}

void Textura::addText() {
	glBindTexture(GL_TEXTURE_2D, m_id); // Bind Texture for Use
}

void Textura::removeText() {
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind Texture
}
