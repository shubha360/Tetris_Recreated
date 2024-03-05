#include "../include/Tetris.h"

Tetris::Tetris() {}

Tetris::~Tetris() {
	freeTetris();
}

bool Tetris::init() {
	return initEngine() && initGame();
}

bool Tetris::initEngine() {
	return
		m_window.init(true, 1720, 980, CLEAR_COLOR) &&
		m_shaderProgram.compileAndLinkShaders("resources/shaders/mainShader.vert", "resources/shaders/mainShader.frag") &&
		m_fps.init(MAX_Fps) &&
		m_camera.init(m_window.getWindowWidth(), m_window.getWindowHeight()) &&
		m_font_coolvetica48.initFromFontFile("Coolvetica", "resources/fonts/coolvetica.ttf", 48, 1.0f, 1) &&
		m_font_ubuntu36.initFromFontFile("Ubuntu", "resources/fonts/Ubuntu-Regular.ttf", 36, 1.0f, 1) &&
		m_gui.init(m_font_coolvetica48) &&
		m_guiRenderer.init("../Evolve-Engine/engine-assets");
}

bool Tetris::initGame() {

	m_guiFont_ubuntu36 = m_gui.addFont(m_font_ubuntu36);

	m_windowDims = glm::ivec2 { m_window.getWindowWidth(), m_window.getWindowHeight() };

	m_randomEngine = std::mt19937(m_seed());
	m_getTetriminoIndex = std::uniform_int_distribution<int>(0, 6);

	Evolve::ImageLoader::LoadTextureFromImage("resources/images/minos.png", m_minoTexture, 4);
	Evolve::ImageLoader::BufferTextureData(m_minoTexture);
	Evolve::ImageLoader::FreeTexture(m_minoTexture);

	// matrix component variables

	const int mainMatrixWidth = 10 * 32;
	const int mainMatrxiHeight = 20 * 32;

	const int extraMatrixWidth = 8 * 16;

	const int horizontalMargin = 50;
		
	glm::ivec2 mainMatrixPos = { m_windowDims.x / 2 -  mainMatrixWidth / 2, m_windowDims.y / 2 + mainMatrxiHeight / 2 };
	glm::ivec2 nextMatrixPos = { mainMatrixPos.x - horizontalMargin - extraMatrixWidth, mainMatrixPos.y };
	glm::ivec2 holdMatrixPos = { nextMatrixPos.x, mainMatrixPos.y - 350 };

	initMatrices(mainMatrixPos, nextMatrixPos, holdMatrixPos);

	initGuiComponents(horizontalMargin);

	return true;
}

std::vector<Tetrimino*> Tetris::initNexts() {
	
	std::vector<Tetrimino*> nexts;
	nexts.resize(3);

	for (int i = 0; i < 3; i++) {
		//nexts[i] = m_tetriminoes[4];
		nexts[i] = m_tetriminoes[m_getTetriminoIndex(m_randomEngine)];
	}
	return nexts;
}

std::vector<Tetrimino*> Tetris::initHold() {

	std::vector<Tetrimino*> hold;
	hold.push_back(nullptr);
	
	return hold;
}

void Tetris::initMatrices(const glm::ivec2& mainMatrixPos, const glm::ivec2& nextMatrixPos, 
	const glm::ivec2& holdMatrixPos) {

	m_matrix.init(mainMatrixPos, m_minoTexture.id);

	m_holdMatrix.init(
		initHold(),
		"HOLD",
		m_font_coolvetica48,
		0.75f,
		Evolve::ColorRgba{ 255, 255, 255, 255 },
		holdMatrixPos,
		m_minoTexture.id
	);

	m_nextMatrix.init(
		initNexts(),
		"NEXT",
		m_font_coolvetica48,
		0.75f,
		Evolve::ColorRgba{ 255, 255, 255, 255 },
		nextMatrixPos,
		m_minoTexture.id
	);
}

void Tetris::initGuiComponents(const int horizontalMargin) {

	const Evolve::ColorRgba primaryColor	{	0,   0,  20, 255 };
	const Evolve::ColorRgba secondaryColor	{ 255, 255, 255, 255 };

	// gui component variables

	std::string gameOverText = "GAME OVER!";
	std::string levelUpText = "LEVEL UP!";
	std::string pauseText = "PAUSED";

	std::string digitText = "3";
	std::string goText = "GO!";

	Evolve::RectDimension preplayPanelDims{};
	Evolve::RectDimension hideNextPanelDims{};

	glm::ivec2 digitPos{};
	glm::ivec2 goPos{};

	glm::ivec2 scorePos{};
	glm::ivec2 legendPos{};
	glm::ivec2 gameOverTextPos{};
	glm::ivec2 levelUpTextPos{};
	glm::ivec2 pauseTextPos{};
	Evolve::RectDimension pausePanelDims{};

	Evolve::RectDimension startButtonDims{};
	Evolve::RectDimension exitButtonDims{};
	Evolve::RectDimension restartButtonDims{};

	auto mainMatrixDims = m_matrix.getDimension();
	auto nextMatrixDims = m_nextMatrix.getDimension();

	{ // positioning the gui components

		startButtonDims = { (int)m_windowDims.x / 2, (int)m_windowDims.y / 2, 512, 64 };
		exitButtonDims = { (int)m_windowDims.x - 10, (int)m_windowDims.y - 10, 100, 64 };
		restartButtonDims = { exitButtonDims.x - (int)exitButtonDims.width - 20, exitButtonDims.y, 140, 64 };

		scorePos = { mainMatrixDims.x + mainMatrixDims.width + horizontalMargin , mainMatrixDims.y };

		legendPos = { scorePos.x, mainMatrixDims.y - 200 };

		pauseTextPos = {
			m_windowDims.x / 2 - m_font_coolvetica48.getLineWidth(pauseText) / 2,
			m_windowDims.y / 2 + m_font_coolvetica48.getLineHeight() / 2
		};

		pausePanelDims.set(mainMatrixDims.x, mainMatrixDims.y, mainMatrixDims.width, mainMatrixDims.height);

		//m_font_coolvetica48.setFontScale(0.75f);

		gameOverTextPos = {
			mainMatrixDims.x + mainMatrixDims.width / 2 - m_font_coolvetica48.getLineWidth(gameOverText) / 2,
			mainMatrixDims.y - mainMatrixDims.height - 20
		};

		//m_font_coolvetica48.setFontScale(1.0f);

		levelUpTextPos = {
			m_windowDims.x / 2 - m_font_coolvetica48.getLineWidth(levelUpText) / 2,
			m_windowDims.y / 2 + m_font_coolvetica48.getLineHeight() / 2
		};

		preplayPanelDims = pausePanelDims;

		hideNextPanelDims.set(nextMatrixDims.x, nextMatrixDims.y,
			nextMatrixDims.width, nextMatrixDims.height);

		digitPos = {
			m_windowDims.x / 2 - m_font_coolvetica48.getLineWidth(digitText) / 2,
			m_windowDims.y / 2 + m_font_coolvetica48.getLineHeight() / 2
		};

		goPos = {
			m_windowDims.x / 2 - m_font_coolvetica48.getLineWidth(goText) / 2,
			m_windowDims.y / 2 + m_font_coolvetica48.getLineHeight() / 2
		};

	}

	// start button
	m_gui_StartButton = m_gui.addTextButton(
		"Start",
		m_guiFont_ubuntu36,
		1.0f,
		secondaryColor,
		primaryColor,
		Evolve::GlyphOrigin::CENTER,
		startButtonDims,
		[&]() {
			m_gameState = GameState::PRE_PLAY;

			m_gui.hideComponent(m_gui_StartButton);

			m_gui.showComponent(m_gui_ScoreText);
			m_gui.showComponent(m_gui_LegendText);
		}
	);

	// exit button
	m_gui_ExitButton = m_gui.addTextButton(
		"Exit",
		m_guiFont_ubuntu36,
		1.0f,
		secondaryColor,
		primaryColor,
		Evolve::GlyphOrigin::TOP_RIGHT,
		exitButtonDims,
		[&]() { m_gameState = GameState::QUIT; }
	);

	// restart button
	m_gui_RestartButton = m_gui.addTextButton(
		"Restart",
		m_guiFont_ubuntu36,
		1.0f,
		secondaryColor,
		primaryColor,
		Evolve::GlyphOrigin::TOP_RIGHT,
		restartButtonDims,
		[&]() {
			restart();
		}
	);

	m_gui.hideComponent(m_gui_RestartButton);

	// PRE PLAY
	m_gui_HideNextPanel = m_gui.addPanel(
		hideNextPanelDims,
		Evolve::GlyphOrigin::TOP_LEFT,
		primaryColor
	);

	m_gui.hideComponent(m_gui_HideNextPanel);

	m_gui_PreplayDigit = m_gui.addPlainText(
		digitText,
		m_guiFont_coolvetica48,
		1.0f,
		secondaryColor,
		digitPos
	);

	m_gui.hideComponent(m_gui_PreplayDigit);

	m_gui_PreplayGo = m_gui.addPlainText(
		goText,
		m_guiFont_coolvetica48,
		1.0f,
		secondaryColor,
		goPos
	);

	m_gui.hideComponent(m_gui_PreplayGo);

	// pause text
	m_gui_PauseText = m_gui.addPlainText(
		pauseText,
		m_guiFont_coolvetica48,
		1.0f,
		secondaryColor,
		pauseTextPos
	);

	m_gui.hideComponent(m_gui_PauseText);

	m_gui_PausePanel = m_gui.addPanel(
		pausePanelDims,
		Evolve::GlyphOrigin::TOP_LEFT,
		Evolve::ColorRgba{ primaryColor.red, primaryColor.green, primaryColor.blue, 200 }
	);

	m_gui.hideComponent(m_gui_PausePanel);

	// level up
	m_gui_LevelUpBlinkText = m_gui.addBlinkingText(
		levelUpText,
		m_guiFont_coolvetica48,
		1.0f,
		secondaryColor,
		levelUpTextPos,
		15.0f, 10.0f
	);

	m_gui.hideComponent(m_gui_LevelUpBlinkText);

	// score text
	m_gui_ScoreText = m_gui.addPlainText(
		"",
		m_guiFont_coolvetica48,
		0.75f,
		secondaryColor,
		scorePos
	);

	m_gui.hideComponent(m_gui_ScoreText);

	std::string legend =
		"A, D - Move left, right\n\n"
		"Q, E - Rotate left, right\n\n"
		"S - Soft drop\n\n"
		"Space - Hard drop\n\n"
		"W - Hold\n\n"
		"Esc - Pause / Resume";

	// legend text
	m_gui_LegendText = m_gui.addPlainText(
		legend,
		m_guiFont_coolvetica48,
		0.75f,
		secondaryColor,
		legendPos
	);

	m_gui.hideComponent(m_gui_LegendText);

	// game over text
	m_gui_gameOverText = m_gui.addPlainText(
		gameOverText,
		m_guiFont_coolvetica48,
		1.0f,
		secondaryColor,
		gameOverTextPos
	);

	m_gui.hideComponent(m_gui_gameOverText);
}

void Tetris::run() {
	gameLoop();
}

void Tetris::gameLoop() {

	float previousTicks = (float)SDL_GetTicks();

	while (m_gameState != GameState::QUIT) {

		m_fps.beginFrame();

		m_inputProcessor.update();

		processInput();

		m_gui.updateGui(m_inputProcessor, m_camera);
		
		if (m_gameState == GameState::PRE_PLAY || m_gameState == GameState::PLAYING) {
			previousTicks = runGameSimulations(previousTicks);
		}

		draw();

		m_fps.endFrame();

		printFps();
	}
}

float Tetris::runGameSimulations(float previousTicks) {
	static const float MS_PER_SECOND = 1000.0f;
	static const float DESIRED_Fps = 60.0f;

	static const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_Fps;

	static const int MAX_PHYSICS_SIMS = 6;
	static const float MAX_DELTA_TIME = 1.0f;

	float newTicks = (float)SDL_GetTicks();
	float frameTime = newTicks - previousTicks;

	float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

	bool inputProcessed = false;

	int i = 0;
	while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_SIMS) {
		float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);

		if (m_gameState == GameState::PRE_PLAY || m_gameState == GameState::PLAYING) {
			updateGame(deltaTime, inputProcessed);
			m_gui.updateTime(deltaTime);
			totalDeltaTime -= deltaTime;
			i++;
		}
		else if (m_gameState == GameState::ENDED) {
			totalDeltaTime = 0.0f;
			i = MAX_PHYSICS_SIMS;
		}
	}

	return newTicks;
}

void Tetris::processInput() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {

		switch (event.type) {
		case SDL_QUIT:
			m_gameState = GameState::QUIT;
			
		case SDL_MOUSEMOTION:
			m_inputProcessor.setMouseCoords(event.motion.x, event.motion.y);
			break;

		case SDL_MOUSEBUTTONDOWN:
			m_inputProcessor.pressKey(event.button.button);
			break;

		case SDL_MOUSEBUTTONUP:
			m_inputProcessor.releaseKey(event.button.button);
			break;

		case SDL_KEYDOWN:
			m_inputProcessor.pressKey(event.key.keysym.sym);
			break;

		case SDL_KEYUP:
			m_inputProcessor.releaseKey(event.key.keysym.sym);
			break;
		}
	}

	if (m_inputProcessor.isKeyPressed(SDLK_ESCAPE)) {
		if (m_gameState == GameState::PLAYING) {
			m_gameState = GameState::PAUSED;
			
			m_gui.showComponent(m_gui_PauseText);
			m_gui.showComponent(m_gui_PausePanel);

			m_gui.hideComponent(m_gui_LevelUpBlinkText);
		}
		else if (m_gameState == GameState::PAUSED) {
			m_gameState = GameState::PLAYING;
			
			m_gui.hideComponent(m_gui_PauseText);
			m_gui.hideComponent(m_gui_PausePanel);
		}
	}
}

void Tetris::restart() {
	m_score = 0;
	m_linesCleared = 0;
	m_currentLevel = 1;

	m_timeSinceAutoDown = 0.0f;
	m_autoDownDuration = 60.0f;

	m_canHold = true;
	m_respawn = true;
	m_checkLines = false;
	m_lastMoveMade = false;

	m_matrix.reset();
	
	m_nextMatrix.reset(initNexts());

	m_holdMatrix.reset(initHold());

	m_gameState = GameState::PRE_PLAY;

	m_gui.hideComponent(m_gui_RestartButton);
	m_gui.hideComponent(m_gui_PauseText);
	m_gui.hideComponent(m_gui_PausePanel);

	m_drawUpdateNeeded = true;
}

void Tetris::displayPreplay(float& preplayTime, const float preplayDuration, const float deltaTime) {
	static bool set3 = false, set2 = false, set1 = false, setGo = false;
	static bool textVisible = false;

	if (preplayTime > preplayDuration) {

		//m_gui.hideComponent(m_gui_PreplayPanel_Id);
		m_gui.hideComponent(m_gui_HideNextPanel);
		m_gui.hideComponent(m_gui_PreplayGo);

		m_gui.showComponent(m_gui_RestartButton);

		preplayTime = 0.0f;
		set3 = set2 = set1 = setGo = textVisible = false;

		m_gameState = GameState::PLAYING;
	}
	else {

		// show 1
		if (preplayTime < 60.0f) {

			if (!textVisible) {
				m_gui.showComponent(m_gui_HideNextPanel);
				m_gui.showComponent(m_gui_PreplayDigit);

				textVisible = true;
			}

			if (!set3) {
				m_gui.setComponentLabel(m_gui_PreplayDigit, "3");
				set3 = true;
			}
		}

		// show 2
		else if (preplayTime >= 60.0f && preplayTime < 120.0f) {

			if (!set2) {
				m_gui.setComponentLabel(m_gui_PreplayDigit, "2");
				set2 = true;
			}
		}

		// show 1
		else if (preplayTime >= 120.0f && preplayTime < 180.0f) {

			if (!set1) {
				m_gui.setComponentLabel(m_gui_PreplayDigit, "1");
				set1 = true;
			}
		}

		// show go
		else {
			m_gui.hideComponent(m_gui_PreplayDigit);
			m_gui.showComponent(m_gui_PreplayGo);
		}

		preplayTime += deltaTime;
	}
}

void Tetris::updateGame(float deltaTime, bool& inputProcessed) {
	
	static float moveDelayDuration = 5.0f;
	static float currentMoveDelay = moveDelayDuration;

	static float levelUpDurauion = 100.0f;
	static float levelUpTime = 0.0f;
	static bool levelingUp = false;

	if (levelingUp) {
		if (levelUpTime > levelUpDurauion) {
			levelingUp = false;
			levelUpTime = 0.0f;
			m_gui.hideComponent(m_gui_LevelUpBlinkText);
		}
		else {
			levelUpTime += deltaTime;
		}
	}

	static float preplayDuration = 240.0f;
	static float preplayTime = 0.0f;

	if (m_gameState == GameState::PRE_PLAY) {
		displayPreplay(preplayTime, preplayDuration, deltaTime);
	}

	else if (m_gameState == GameState::PLAYING) {
		if (m_respawn) {

			if (m_checkLines) {
				if (updateScoreAndLevel()) {
					levelingUp = true;
					m_gui.showComponent(m_gui_LevelUpBlinkText);
				}
				m_checkLines = false;
			}

			//m_current = m_nextMatrix.pushAndPop(m_tetriminoes[4]);
			m_current = m_nextMatrix.pushAndPop(m_tetriminoes[m_getTetriminoIndex(m_randomEngine)]);

			m_current->reset();

			if (!m_current->spawn()) {
				// game ended
				m_gameState = GameState::ENDED;

				m_gui.showComponent(m_gui_gameOverText);
			}

			m_drawUpdateNeeded = true;
			currentMoveDelay = moveDelayDuration;
			m_timeSinceAutoDown = 0.0f;

			m_respawn = false;
			m_lastMoveMade = false;
		}
		else {

			m_timeSinceAutoDown += deltaTime;
			currentMoveDelay += deltaTime;

			if (m_timeSinceAutoDown >= m_autoDownDuration) {

				// moveDown returns false if can't move down anymore
				// that's why m_respawn is true
				if (!m_current->moveDown()) {
					m_respawn = true;
					m_canHold = true;
					m_checkLines = true;
				}

				m_timeSinceAutoDown = 0.0f;
				m_drawUpdateNeeded = true;
			}

			// move left
			if (m_inputProcessor.isKeyDown(SDLK_a) && !inputProcessed && !m_lastMoveMade) {

				if (currentMoveDelay >= moveDelayDuration && m_current->moveLeft()) {
					m_drawUpdateNeeded = true;
					inputProcessed = true;
					currentMoveDelay = 0.0f;

					// can only make one move if can't move down
					if (!m_current->canMoveDown()) {
						m_lastMoveMade = true;
					}
				}
			}

			// move right
			else if (m_inputProcessor.isKeyDown(SDLK_d) && !inputProcessed && !m_lastMoveMade) {

				if (currentMoveDelay >= moveDelayDuration && m_current->moveRight()) {
					m_drawUpdateNeeded = true;
					inputProcessed = true;
					currentMoveDelay = 0.0f;

					// can only make one move if can't move down
					if (!m_current->canMoveDown()) {
						m_lastMoveMade = true;
					}
				}
			}
				
			// soft drop
			else if (m_inputProcessor.isKeyDown(SDLK_s) && !inputProcessed) {

				if (currentMoveDelay >= moveDelayDuration) {
					if (m_current->moveDown()) {
						m_drawUpdateNeeded = true;
						inputProcessed = true;
						currentMoveDelay = 0.0f;
						m_timeSinceAutoDown = 0.0f;

						m_score += (long long) (1 * m_currentLevel);
					}
					else {
						m_respawn = true;
						m_canHold = true;
						m_checkLines = true;
						inputProcessed = true;
						m_drawUpdateNeeded = true;
					}
				}
			}

			// if any moving key is released, next key press should not delay
			else if (
				m_inputProcessor.isKeyReleased(SDLK_a) ||
				m_inputProcessor.isKeyReleased(SDLK_d) ||
				m_inputProcessor.isKeyReleased(SDLK_s)
				) {

				currentMoveDelay = moveDelayDuration;
			}

			// rotate left
			else if (m_inputProcessor.isKeyPressed(SDLK_q) && !inputProcessed && !m_lastMoveMade) {
				if (m_current->rotateLeft()) {
					m_drawUpdateNeeded = true;
					inputProcessed = true;
					
					// can only make one move if can't move down
					if (!m_current->canMoveDown()) {
						m_lastMoveMade = true;
					}
				}
			}
				
			// rotate right
			else if (m_inputProcessor.isKeyPressed(SDLK_e) && !inputProcessed && !m_lastMoveMade) {
				if (m_current->rotateRight()) {
					m_drawUpdateNeeded = true;
					inputProcessed = true;

					// can only make one move if can't move down
					if (!m_current->canMoveDown()) {
						m_lastMoveMade = true;
					}
				}
			}
				
			// hard drop
			else if (m_inputProcessor.isKeyPressed(SDLK_SPACE) && !inputProcessed) {
				while (true) {
					if (!m_current->moveDown()) {
						break;
					}
					m_score += (long long) (2 * m_currentLevel);
				}
				m_respawn = true;
				m_canHold = true;
				m_checkLines = true;
				inputProcessed = true;
				m_drawUpdateNeeded = true;
			}

			// hold
			else if (m_inputProcessor.isKeyPressed(SDLK_w) && !inputProcessed && m_canHold) {
				m_current->removeFromMatrix(true);
				m_current = m_holdMatrix.pushAndPop(m_current);

				if (m_current == nullptr) {
					m_respawn = true;
				}
				else {
					m_current->reset();
					m_current->spawn();
				}

				m_drawUpdateNeeded = true;
				inputProcessed = true;
				m_canHold = false;
			}
		}
	}
}

// returns true if leveled up
bool Tetris::updateScoreAndLevel() {
	
	bool levelUp = false;

	static float lowestAutoDownDuration = 10.0f;

	static float eachLevelSpeedIncrease = 10.0f;
	static int lineClearsForLevelUp = 10;

	int currentLinesCleared = m_matrix.checkLineClears();

	switch (currentLinesCleared) {

	case 1:
		m_score += (long long)(100 * m_currentLevel);
		break;

	case 2:
		m_score += (long long)(300 * m_currentLevel);
		break;

	case 3:
		m_score += (long long)(600 * m_currentLevel);
		break;

	case 4:
		m_score += (long long)(1000 * m_currentLevel);
		break;
	}

	m_linesCleared += currentLinesCleared;

	// check if level up possible
	if (m_autoDownDuration > lowestAutoDownDuration) {

		// if, linesCleared = 57,
		// then 57 / 10 + 1 = 5 + 1 = 6
		int newLevel = m_linesCleared / lineClearsForLevelUp + 1;

		// level up
		if (newLevel > m_currentLevel) {
			levelUp = true;
			m_currentLevel = newLevel;
			m_autoDownDuration -= eachLevelSpeedIncrease;

			if (m_autoDownDuration < lowestAutoDownDuration) {
				m_autoDownDuration = lowestAutoDownDuration;
			}
		}
	}
	return levelUp;
}

void Tetris::draw() {

	glClear(GL_COLOR_BUFFER_BIT);

	m_shaderProgram.useProgram();

	glActiveTexture(GL_TEXTURE0);
	GLint samplerLoc = m_shaderProgram.getUniformLocation("u_imageSampler");
	glUniform1i(samplerLoc, 0);

	m_camera.sendMatrixDataToShader(m_shaderProgram);

	{
		if (m_drawUpdateNeeded) {
			m_textureRenderer.begin();

			// DRAW HERE
			m_matrix.drawMatrix(m_textureRenderer);

			m_nextMatrix.drawMatrix(m_textureRenderer);

			m_nextMatrix.drawName(m_textureRenderer);

			m_holdMatrix.drawMatrix(m_textureRenderer);

			m_holdMatrix.drawName(m_textureRenderer);

			m_textureRenderer.end();

			m_drawUpdateNeeded = false;
		}

		if (m_gameState != GameState::MAIN_MENU && m_gameState != GameState::QUIT) {
			m_textureRenderer.renderTextures();
		}
	}

	m_shaderProgram.unuseProgram();

	std::string scoreText = "Score: " + std::to_string(m_score) + "\n" +
		"Lines cleared: " + std::to_string(m_linesCleared) + "\n" +
		"Level: " + std::to_string(m_currentLevel) + "\n";

	m_gui.setComponentLabel(m_gui_ScoreText, scoreText);

	m_guiRenderer.renderGui(m_gui, m_camera);

	m_window.swapBuffer();
}

void Tetris::printFps() {
	float currentFps = m_fps.calculateFps();

	static int frameCount = 0;
	frameCount++;

	if (frameCount == 100) {
		printf("%f\n", currentFps);
		frameCount = 0;
	}
}

void Tetris::freeTetris() {
	Evolve::ImageLoader::DeleteTexture(m_minoTexture);

	m_font_coolvetica48.deleteFont();

	m_gui.freeGui();
	m_guiRenderer.freeGuiRenderer();

	m_textureRenderer.freeTextureRenderer();

	m_shaderProgram.freeProgram();

	m_window.deleteWindow();

	SDL_Quit();
}