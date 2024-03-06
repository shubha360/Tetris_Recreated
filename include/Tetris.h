#pragma once


#include <SDL/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <random>

// Engine includes
#include <Evolve/Window.h>
#include <Evolve/GlslProgram.h>
#include <Evolve/Vertex2D.h>
#include <Evolve/Fps.h>
#include <Evolve/InputProcessor.h>
#include <Evolve/ImageLoader.h>
#include <Evolve/Camera.h>
#include <Evolve/TextureRenderer.h>
#include <Evolve/Font.h>
#include <Evolve/Gui.h>
#include <Evolve/GuiRenderer.h>

// Tetris includes
#include "Matrix.h"
#include "Tetrimino.h"
#include "ExtraMatrix.h"

enum class GameState {
	PRE_PLAY,
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
	glm::ivec2 m_windowDims { 0, 0 };

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

	bool m_drawUpdateNeeded = true;

	// matrices
	Matrix m_matrix;
	ExtraMatrix m_nextMatrix, m_holdMatrix;

	// tetrimino variables
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

	// random generation variables
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

	// fonts
	Evolve::Font m_font_ubuntu36;
	Evolve::Font m_font_amaranth48;
	Evolve::Font m_font_blox;

	// gui font ids
	int m_guiFont_amaranth48 = 0; // default gui font
	int m_guiFont_ubuntu36 = 0;
	int m_guiFont_blox = 0;
	
	// gui component ids
	int m_gui_TetrisText = -1;

	int m_gui_StartButton = -1;
	int m_gui_ExitButton = -1;
	int m_gui_RestartButton = -1;
	
	int m_gui_HideNextPanel = -1;
	int m_gui_PreplayDigit = -1;
	int m_gui_PreplayGo = -1;

	int m_gui_PauseText = -1;
	int m_gui_PausePanel = -1;

	int m_gui_LevelUpBlinkText = -1;

	int m_gui_ScoreText = -1;
	int m_gui_LegendText = -1;
	int m_gui_gameOverText = -1;

	bool initEngine();
	bool initGame();

	std::vector<Tetrimino*> initNexts();
	std::vector<Tetrimino*> initHold();

	void initMatrices(const glm::ivec2& mainMatrixPos, const glm::ivec2& nextMatrixPos, const glm::ivec2& holdMatrixPos);
	void initGuiComponents(const int horizontalMargin);

	void gameLoop();
	float runGameSimulations(float previousTicks);
	void processInput();
	
	void restart();

	void displayPreplay(float& preplayTime, const float preplayDuration, const float deltaTime);

	/*
	This function is called in every simulation of a frame.
	Input should be processed at most once per frame.
	The second parameter takes a bool which specifies if input is already processed in this frame.
	If it is false, the function processes input and sets this to true. 
	So that the next simulation doesn't process input again.
	*/
	void updateGame(float deltaTime, bool& inputProcessed);

	// returns true if leveled up
	bool updateScoreAndLevel();

	void draw();
	
	void printFps();
	void freeTetris();
};