#pragma once


#include <SDL/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <random>

// Engine includes
#include "Window.h"
#include "GlslProgram.h"
#include "Vertex2D.h"
#include "Fps.h"
#include "InputProcessor.h"
#include "ImageLoader.h"
#include "Camera.h"
#include "TextureRenderer.h"
#include "Font.h"
#include "Gui.h"
#include "GuiRenderer.h"

// Tetris includes
#include "Matrix.h"
#include "Tetrimino.h"
#include "ExtraMatrix.h"

enum class GameState {
	PLAYING,
	MAIN_MENU,
	PAUSED,
	ENDED,
	QUIT
};

class Tetris {
public:
	Tetris();
	~Tetris();

	bool init();
	void run();	

private:
	const Evolve::ColorRgba CLEAR_COLOR { 28,36,56,255 };
	const float MAX_Fps = 60.0f;
	GameState m_gameState = GameState::PLAYING;
	Evolve::TextureData m_minoTexture {};

	std::string m_pauseText = "PAUSED", m_endText = "GAME ENDED!";
	
	// these will be defined after font is initialized
	int m_pauseTextX = 0, m_endTextX = 0;
	glm::ivec2 m_gameStateTextPos {};

	long long m_score = 0;
	int m_linesCleared = 0;
	int m_currentLevel = 1;

	Matrix m_matrix;
	ExtraMatrix m_nextMatrix, m_holdMatrix;

	Tetrimino_T m_tempMinoT = Tetrimino_T(&m_matrix);
	Tetrimino_L m_tempMinoL = Tetrimino_L(&m_matrix);
	Tetrimino_J m_tempMinoJ = Tetrimino_J(&m_matrix);
	Tetrimino_O m_tempMinoO = Tetrimino_O(&m_matrix);
	Tetrimino_I m_tempMinoI = Tetrimino_I(&m_matrix);
	Tetrimino_Z m_tempMinoZ = Tetrimino_Z(&m_matrix);
	Tetrimino_S m_tempMinoS = Tetrimino_S(&m_matrix);
	
	Tetrimino* m_tetriminoes[7] {
		&m_tempMinoT,
		&m_tempMinoL,
		&m_tempMinoJ,
		&m_tempMinoO,
		&m_tempMinoI,
		&m_tempMinoZ,
		&m_tempMinoS
	};

	Tetrimino* m_current = nullptr;

	std::random_device m_seed;
	std::mt19937 m_randomEngine;
	std::uniform_int_distribution<int> m_getTetriminoIndex;

	// Engine Tools
	Evolve::Window m_window;
	Evolve::GlslProgram m_shaderProgram;
	Evolve::Fps m_fps;
	Evolve::InputProcessor m_inputProcessor;
	Evolve::Camera m_camera;
	Evolve::TextureRenderer m_textureRenderer;
	Evolve::Gui m_gui;
	Evolve::GuiRenderer m_guiRenderer;
	Evolve::Font m_quicksandFont;

	glm::ivec2 m_windowDims { 0, 0 };

	int m_guiQuicksandFontId = 0;
	int m_guiExitButtonId = -1;
	int m_guiTextId = -1;
	int m_guiLegendId = -1;
	int m_guiGameStateTextId = -1;

	bool m_drawUpdateNeeded = true;

	bool initEngine();
	bool initGame();

	void gameLoop();
	float runGameSimulations(float previousTicks);
	void processInput();
	
	/*
	This function is called in every simulation of a frame.
	Input should be processed at most once per frame.
	The second parameter takes a bool which specifies if input is already processed in this frame.
	If it is false, the function processes input and sets this to true. 
	So that the next simulation doesn't process input again.
	*/
	void updateGame(float deltaTime, bool& inputProcessed);
	

	void updateScoreAndLevel(float& autoDownDuration);

	void draw();
	
	void printFps();
	void freeTetris();
};