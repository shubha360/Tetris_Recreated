#pragma once


#include <SDL/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Window.h"
#include "GLSLProgram.h"
#include "Vertex2D.h"
#include "FPS.h"
#include "InputProcessor.h"
#include "ImageLoader.h"
#include "Camera.h"
#include "TextureRenderer.h"
#include "Font.h"

enum class GameState {
	PLAYING,
	QUIT
};

class Tetris {
public:
	Tetris();
	~Tetris();

	bool init();
	void run();	

private:
	const unsigned int SCREEN_WIDTH = 1730; // 1920
	const unsigned int SCREEN_HEIGHT = 980; // 1080
	const ColorRGBA CLEAR_COLOR { 0,50,120,255 };
	const float MAX_FPS = 60.0f;
	GameState m_gameState = GameState::PLAYING;

	const GLint EXIT_BUTTON_WIDTH = SCREEN_WIDTH / 50;
	const GLint EXIT_BUTTON_HEIGHT = SCREEN_WIDTH / 50;
	const GLint EXIT_BUTTON_MARGIN = SCREEN_HEIGHT / 50;

	const GLint EXIT_BUTTON_X = SCREEN_WIDTH - EXIT_BUTTON_WIDTH - EXIT_BUTTON_MARGIN;
	const GLint EXIT_BUTTON_Y = EXIT_BUTTON_MARGIN;


	Window m_window;
	GLSLProgram m_shaderProgram;
	FPS m_fps;
	InputProcessor m_inputProcessor;
	Camera m_camera;
	TextureRenderer m_textureRenderer;

	Font m_lazyFont;
	TextureData m_textureOne, m_textureTwo, m_textureTest;

	bool m_drawUpdateNeeded = true;

	void gameLoop();
	void processInput();
	void updateGame();
	void draw();
	void freeTetris();
};