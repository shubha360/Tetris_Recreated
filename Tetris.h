#pragma once


#include <SDL/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "GLSLProgram.h"
#include "Vertex2D.h"
#include "FPS.h"

class Tetris {
public:
	Tetris();
	~Tetris();

	bool init();
	void run();	

private:
	const int SCREEN_WIDTH = 1730; // 1920
	const int SCREEN_HEIGHT = 980; // 1080
	const float MAX_FPS = 60.0f;

	const GLint EXIT_BUTTON_WIDTH = SCREEN_WIDTH / 50;
	const GLint EXIT_BUTTON_HEIGHT = SCREEN_WIDTH / 50;
	const GLint EXIT_BUTTON_MARGIN = SCREEN_HEIGHT / 50;

	const GLint EXIT_BUTTON_X = SCREEN_WIDTH - EXIT_BUTTON_WIDTH - EXIT_BUTTON_MARGIN;
	const GLint EXIT_BUTTON_Y = EXIT_BUTTON_MARGIN;

	glm::mat4 m_projectionMatrix = glm::ortho(
		0.0f,
		(float)SCREEN_WIDTH,
		(float)SCREEN_HEIGHT,
		0.0f,
		-1.0f,
		1.0f
	);

	glm::mat4 m_viewMatrix = glm::mat4(1.0f);
	glm::mat4 m_modelMatrix = glm::mat4(1.0f);

	glm::mat4 m_mvp = m_projectionMatrix * m_viewMatrix * m_modelMatrix;

	GLuint m_vaoID = 0;
	GLuint m_vboID = 0;
	GLuint m_iboID = 0;

	Vertex2D m_verticies[4] = {};
	GLuint m_indicies[6] = {};

	SDL_Window* m_window = nullptr;

	GLSLProgram m_shaderProgram;
	FPS fps;

	bool initSdlWindow();
	bool initGL();

	void gameLoop();
	bool processInput(SDL_Event* event);
	void draw();
	void freeTetris();
};