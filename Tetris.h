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
#include "GUI.h"
#include "GUIRenderer.h"

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
	const ColorRGBA CLEAR_COLOR { 0,50,120,255 };
	const float MAX_FPS = 60.0f;
	GameState m_gameState = GameState::PLAYING;

	Window m_window;
	GLSLProgram m_shaderProgram;
	FPS m_fps;
	InputProcessor m_inputProcessor;
	Camera m_camera;
	TextureRenderer m_textureRenderer;
	
	GUI m_gui;
	GUIRenderer m_guiRenderer;

	Font m_lazyFont;
	TextureData m_textureOne, m_textureTwo, m_textureTest;

	SDL_Cursor* m_indexPointerCursor = nullptr;

	bool m_drawUpdateNeeded = true;

	void gameLoop();
	void processInput();
	void updateGame();
	void draw();
	void freeTetris();
};