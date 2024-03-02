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
		m_window.init(false, 1720, 980, CLEAR_COLOR) &&
		m_shaderProgram.compileAndLinkShaders("resources/shaders/mainShader.vert", "resources/shaders/mainShader.frag") &&
		m_fps.init(MAX_Fps) &&
		m_camera.init(m_window.getWindowWidth(), m_window.getWindowHeight()) &&
		m_quicksandFont.initFromFontFile("Quicksand", "resources/fonts/Quicksand.otf") &&
		m_gui.init(m_quicksandFont) &&
		m_guiRenderer.init("../Evolve-Engine/engine-assets");
}

bool Tetris::initGame() {

	m_winowDims = glm::ivec2 { m_window.getWindowWidth(), m_window.getWindowHeight() };

	m_randomEngine = std::mt19937(m_seed());
	m_getTetriminoIndex = std::uniform_int_distribution<int>(0, 6);

	Evolve::ImageLoader::LoadTextureFromImage("resources/images/mino.png", m_minoTexture, 4);
	Evolve::ImageLoader::BufferTextureData(m_minoTexture);
	Evolve::ImageLoader::FreeTexture(m_minoTexture);

	//{ // Positioning the main game components

		//int top = m_winowDims.y / 10 * 9, left = m_winowDims.x / 20;

		glm::ivec2 mainMatrixPos { m_window.getWindowWidth() / 6, m_window.getWindowHeight() / 10 * 9 };

		int mainMatrixWidth = m_matrix.getColumns() * m_matrix.getMinoLength();

		glm::ivec2 nextMatrixPos { mainMatrixPos.x + mainMatrixWidth + 50, mainMatrixPos.y };

		glm::ivec2 holdMatrixPos { nextMatrixPos.x, mainMatrixPos.y - 500 };

		m_matrix.init(mainMatrixPos, m_minoTexture.id);
	//}

	std::vector<Tetrimino*> hold;
	hold.push_back(nullptr);
	m_holdMatrix.init(hold, holdMatrixPos, m_minoTexture.id);

	std::vector<Tetrimino*> nexts;
	nexts.resize(3);

	for (int i = 0; i < 3; i++) {
		//nexts[i] = m_tetriminoes[4];
		nexts[i] = m_tetriminoes[m_getTetriminoIndex(m_randomEngine)];
	}

	m_nextMatrix.init(nexts, nextMatrixPos, m_minoTexture.id);

	m_guiExitButtonId = m_gui.addTextButton(
		"Exit",
		m_guiQuicksandFontId,
		1.0f,
		Evolve::ColorRgba{ 255, 255, 255, 255 },
		Evolve::ColorRgba{ 0, 0, 0, 255 },
		Evolve::GlyphOrigin::TOP_RIGHT,
		Evolve::RectDimension{ (int) m_winowDims.x - 10, (int) m_winowDims.y - 10, 128, 64 },
		[&]() { m_gameState = GameState::QUIT; }
	);

	m_guiTextId = m_gui.addPlainText(
		"", 
		m_guiQuicksandFontId,
		1.0f, 
		Evolve::ColorRgba{ 255, 255, 255, 255 },
		glm::ivec2(0, (int)m_window.getWindowHeight()));

	return true;
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
		
		if (m_gameState == GameState::PLAYING) {
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

		if (m_gameState == GameState::PLAYING) {
			updateGame(deltaTime, inputProcessed);
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
		}
		else if (m_gameState == GameState::PAUSED) {
			m_gameState = GameState::PLAYING;
		}
	}
}

void Tetris::updateGame(float deltaTime, bool& inputProcessed) {
	static float timeSinceAutoDown = 0.0f;
	static float autoDownDuration = 60.0f;

	static float moveDelayDuration = 5.0f;
	static float currentMoveDelay = moveDelayDuration;

	// can hold only once before respawing
	static bool canHold = true;
	static bool respawn = true;
	static bool checkLines = false;

	if (m_gameState == GameState::PLAYING) {
		if (respawn) {

			if (checkLines) {
				updateScoreAndLevel(autoDownDuration);
				checkLines = false;
			}

			//m_current = m_nextMatrix.pushAndPop(m_tetriminoes[4]);
			m_current = m_nextMatrix.pushAndPop(m_tetriminoes[m_getTetriminoIndex(m_randomEngine)]);

			m_current->reset();

			if (!m_current->spawn()) {
				// game ended
				m_gameState = GameState::ENDED;
			}

			m_drawUpdateNeeded = true;
			currentMoveDelay = moveDelayDuration;
			timeSinceAutoDown = 0.0f;

			respawn = false;
		}
		else {

			timeSinceAutoDown += deltaTime;
			currentMoveDelay += deltaTime;

			if (timeSinceAutoDown >= autoDownDuration) {

				// moveDown returns false if can't move down anymore
				// that's why respawn is true
				if (!m_current->moveDown()) {
					respawn = true;
					canHold = true;
					checkLines = true;
				}

				timeSinceAutoDown = 0.0f;
				m_drawUpdateNeeded = true;
			}

			// move left
			if (m_inputProcessor.isKeyDown(SDLK_a) && !inputProcessed) {

				if (currentMoveDelay >= moveDelayDuration && m_current->moveLeft()) {
					m_drawUpdateNeeded = true;
					inputProcessed = true;
					currentMoveDelay = 0.0f;

					// can only make one move if can't move down
					if (!m_current->canMoveDown()) {
						respawn = true;
						canHold = true;
						checkLines = true;
					}
				}
			}

			// move right
			else if (m_inputProcessor.isKeyDown(SDLK_d) && !inputProcessed) {

				if (currentMoveDelay >= moveDelayDuration && m_current->moveRight()) {
					m_drawUpdateNeeded = true;
					inputProcessed = true;
					currentMoveDelay = 0.0f;

					// can only make one move if can't move down
					if (!m_current->canMoveDown()) {
						respawn = true;
						canHold = true;
						checkLines = true;
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
						timeSinceAutoDown = 0.0f;

						m_score += (long long) (1 * m_currentLevel);
					}
					else {
						respawn = true;
						canHold = true;
						checkLines = true;
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
			else if (m_inputProcessor.isKeyPressed(SDLK_q) && !inputProcessed) {
				if (m_current->rotateLeft()) {
					m_drawUpdateNeeded = true;
					inputProcessed = true;
					
					// can only make one move if can't move down
					if (!m_current->canMoveDown()) {
						respawn = true;
						canHold = true;
						checkLines = true;
					}
				}
			}
				
			// rotate right
			else if (m_inputProcessor.isKeyPressed(SDLK_e) && !inputProcessed) {
				if (m_current->rotateRight()) {
					m_drawUpdateNeeded = true;
					inputProcessed = true;

					// can only make one move if can't move down
					if (!m_current->canMoveDown()) {
						respawn = true;
						canHold = true;
						checkLines = true;
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
				respawn = true;
				canHold = true;
				checkLines = true;
			}

			// hold
			else if (m_inputProcessor.isKeyPressed(SDLK_w) && !inputProcessed && canHold) {
				m_current->removeFromMatrix();
				m_current = m_holdMatrix.pushAndPop(m_current);

				if (m_current == nullptr) {
					respawn = true;
				}
				else {
					m_current->reset();
					m_current->spawn();
				}

				m_drawUpdateNeeded = true;
				inputProcessed = true;
				canHold = false;
			}
		}
	}
}

void Tetris::updateScoreAndLevel(float& autoDownDuration) {
	
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
	if (autoDownDuration > lowestAutoDownDuration) {

		// if, linesCleared = 57,
		// then 57 / 10 + 1 = 5 + 1 = 6
		int newLevel = m_linesCleared / lineClearsForLevelUp + 1;

		// level up
		if (newLevel > m_currentLevel) {
			m_currentLevel = newLevel;
			autoDownDuration -= eachLevelSpeedIncrease;

			if (autoDownDuration < lowestAutoDownDuration) {
				autoDownDuration = lowestAutoDownDuration;
			}
		}
	}
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

			m_holdMatrix.drawMatrix(m_textureRenderer);

			m_textureRenderer.end();

			m_drawUpdateNeeded = false;
		}

		m_textureRenderer.renderTextures();
	}

	m_shaderProgram.unuseProgram();

	std::string scoreText = "Score: " + std::to_string(m_score) + "\n" +
		"Lines cleared: " + std::to_string(m_linesCleared) + "\n" +
		"Level: " + std::to_string(m_currentLevel) + "\n";

	m_gui.setComponentLabel(m_guiTextId, scoreText);

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

	m_quicksandFont.deleteFont();

	m_gui.freeGui();
	m_guiRenderer.freeGuiRenderer();

	m_textureRenderer.freeTextureRenderer();

	m_shaderProgram.freeProgram();

	m_window.deleteWindow();

	SDL_Quit();
}