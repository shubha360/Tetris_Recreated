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
	GameState m_gameState = GameState::MAIN_MENU;
	Evolve::TextureData m_minoTexture {};

	// game play variables
	
	long long m_score = 0;
	int m_linesCleared = 0;
	int m_currentLevel = 1;

	float m_timeSinceAutoDown = 0.0f;
	float m_autoDownDuration = 60.0f;

	bool m_canHold = true;
	bool m_respawn = true;
	bool m_checkLines = false;
	bool m_lastMoveMade = false;

	//

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

	int m_gui_QuicksandFont_Id = 0;
	
	int m_gui_StartButton_Id = -1;
	int m_gui_ExitButton_Id = -1;
	int m_gui_RestartButton_Id = -1;
	
	int m_gui_PauseBgText_Id = -1;
	int m_gui_LevelUpBlinkText_Id = -1;

	int m_gui_ScoreText_Id = -1;
	int m_gui_Legend_Id = -1;
	int m_gui_gameOverText_Id = -1;

	bool m_drawUpdateNeeded = true;

	bool initEngine();
	bool initGame();

	std::vector<Tetrimino*> initNexts();
	std::vector<Tetrimino*> initHold();

	void gameLoop();
	float runGameSimulations(float previousTicks);
	void processInput();
	
	void restart();

	/*
	This function is called in every simulation of a frame.
	Input should be processed at most once per frame.
	The second parameter takes a bool which specifies if input is already processed in this frame.
	If it is false, the function processes input and sets this to true. 
	So that the next simulation doesn't process input again.
	*/
	void updateGame(float deltaTime, bool& inputProcessed);
	
	// returns true if level upped
	bool updateScoreAndLevel();

	void draw();
	
	void printFps();
	void freeTetris();
};