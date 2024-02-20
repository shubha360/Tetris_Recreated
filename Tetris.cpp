#include "Tetris.h"

Tetris::Tetris() {}

Tetris::~Tetris() {
	freeTetris();
}

bool Tetris::init() {
	return initEngine() && initGame();
}

bool Tetris::initGame() {
	m_matrix.init(m_window.getWindowWidth(), m_window.getWindowHeight());
	
	m_tempMino.addToMatrix(m_matrix.getMatrix());
	m_tempMino2.addToMatrix(m_matrix.getMatrix());
	return true;
}

bool Tetris::initEngine() {
	return
		m_window.init(false, 1720, 980, CLEAR_COLOR) &&
		m_shaderProgram.compileAndLinkShaders("resources/shaders/mainShader.vert", "resources/shaders/mainShader.frag") &&
		m_fps.init(MAX_FPS) &&
		m_camera.init(m_window.getWindowWidth(), m_window.getWindowHeight()) &&
		m_gui.init() &&
		m_guiRenderer.init();
}

void Tetris::run() {
	m_gui.addTextButton(
		"Exit",
		0.5f,
		ColorRGBA{ 255, 255, 255, 255 },
		ColorRGBA{ 0, 0, 0, 255 },
		GlyphOrigin::TOP_RIGHT,
		RectDimension{ (int)m_window.getWindowWidth() - 10, (int)m_window.getWindowHeight() - 10, 80, 40},
		[&]() { m_gameState = GameState::QUIT; }
		);

	gameLoop();
}

void Tetris::gameLoop() {

	while (m_gameState == GameState::PLAYING) {

		m_fps.beginFrame();

		m_inputProcessor.update();

		processInput();

		m_gui.updateGUI(m_inputProcessor, m_camera);
		updateGame();

		draw();

		float currentFps = m_fps.calculateFps();

		static int frameCount = 0;
		frameCount++;

		if (frameCount == 100) {
			printf("%f\n", currentFps);
			frameCount = 0;
		}

		m_fps.endFrame();
	}
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

	// move left or right
	if (m_inputProcessor.isKeyPressed(SDLK_a)) {
		m_tempMino.moveLeft(m_matrix.getMatrix(), m_matrix.getEmptyCellSign());
		m_drawUpdateNeeded = true;
	}
	else if (m_inputProcessor.isKeyPressed(SDLK_d)) {
		m_tempMino.moveRight(m_matrix.getMatrix(), m_matrix.getEmptyCellSign());
		m_drawUpdateNeeded = true;
	}

	// rotate left or right
	else if (m_inputProcessor.isKeyPressed(SDLK_q)) {
		m_tempMino.rotateLeft(m_matrix.getMatrix(), m_matrix.getEmptyCellSign());
		m_drawUpdateNeeded = true;
	}
	else if (m_inputProcessor.isKeyPressed(SDLK_e)) {
		m_tempMino.rotateRight(m_matrix.getMatrix(), m_matrix.getEmptyCellSign());
		m_drawUpdateNeeded = true;
	}
}

void Tetris::updateGame() {}

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

			m_textureRenderer.end();

			m_drawUpdateNeeded = false;
		}

		m_textureRenderer.renderTextures();
	}

	m_shaderProgram.unuseProgram();

	m_guiRenderer.renderGUI(m_gui, m_camera);

	m_window.swapBuffer();
}

void Tetris::freeTetris() {
	m_gui.freeGUI();
	m_guiRenderer.freeGUIRenderer();

	m_textureRenderer.freeTextureRenderer();

	m_shaderProgram.freeProgram();

	m_window.deleteWindow();

	SDL_Quit();
}