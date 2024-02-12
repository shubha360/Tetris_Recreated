#include "Tetris.h"

Tetris::Tetris() {}

Tetris::~Tetris() {
	freeTetris();
}

bool Tetris::init() {
	return
		initSdlWindow() &&
		initGL() &&
		m_shaderProgram.compileAndLinkShaders("resources/shaders/mainShader.vert", "resources/shaders/mainShader.frag") &&
		m_fps.init(MAX_FPS) &&
		m_camera.init(
			glm::ortho(0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f, -1.0f, 1.0f),
			glm::mat4(1.0f),
			glm::mat4(1.0f));
}

void Tetris::run() {

	m_imageLodaer.loadTextureFromImage32("resources/images/square.png", m_textureOne);
	m_imageLodaer.bufferTextureData(m_textureOne);

	m_imageLodaer.loadTextureFromImage32("resources/images/non-square.png", m_textureTwo);
	m_imageLodaer.bufferTextureData(m_textureTwo);

	m_imageLodaer.loadTextureFromImage32("resources/images/color-test.png", m_textureTest);
	m_imageLodaer.bufferTextureData(m_textureTest);

	gameLoop();
}

bool Tetris::initSdlWindow() {

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("Error at initSdlWindow() in Tetris.cpp\n"
			"Failed to initialize SDL!SDL_Error: % s\n", SDL_GetError());
		return false;
	}

	int attribResponse = 0;

	attribResponse += SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	attribResponse += SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	attribResponse += SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	attribResponse += SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	if (attribResponse < 0) {
		printf("Error at initSdlWindow() in Tetris.cpp\n"
			"Failed to set an SDL_GL attribute! Error: %s\n", SDL_GetError());
		return false;
	}

	m_window = SDL_CreateWindow(
		"Tetris",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_OPENGL /*| SDL_WINDOW_FULLSCREEN*/
	);

	if (m_window == nullptr) {
		printf("Error at initSdlWindow() in Tetris.cpp\n"
			"Failed to create window! Error: %s\n", SDL_GetError());
		return false;
	}

	return true;
}

bool Tetris::initGL() {

	SDL_GLContext glContext = SDL_GL_CreateContext(m_window);

	if (glContext == nullptr) {
		printf("Error at initGL() in Tetris.cpp\n"
			"Failed to create GL context! Error: %s\n", SDL_GetError());
		return false;
	}

	GLenum response = glewInit();

	if (response != GLEW_OK) {
		printf("Error at initGL() in Tetris.cpp\n"
			"Failed to initialize GLEW!\n");
		return false;
	}

	printf("OpenGL Version: %s\n", glGetString(GL_VERSION));

	glClearColor(0.0f, 0.05f, 0.25f, 1.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	// SDL_GL_SetSwapInterval(1);

	return true;
}

void Tetris::gameLoop() {
	while (m_gameState == GameState::PLAYING) {

		m_fps.beginFrame();

		processInput();
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
		}
	}
}

void Tetris::updateGame() {
	if (m_inputProcessor.isKeyDown(SDL_BUTTON_LEFT)) {

		glm::ivec2 mouseCoords = m_inputProcessor.getMouseCoords();

		if ((mouseCoords.x >= EXIT_BUTTON_X && mouseCoords.x <= EXIT_BUTTON_X + EXIT_BUTTON_WIDTH)
			&&
			(mouseCoords.y >= EXIT_BUTTON_Y && mouseCoords.y <= EXIT_BUTTON_Y + EXIT_BUTTON_HEIGHT))
		{
			m_gameState = GameState::QUIT;
		}
	}
}

void Tetris::draw() {
	static UVDimension uvRect;
	uvRect.set(0.0f, 0.0f, 1.0f, 1.0f);

	static RectDimension destRect1;
	destRect1.set(100, 100, 256, 256);

	static RectDimension destRect2;
	destRect2.set(100, 600, 270, 180);

	static RectDimension destRect3;
	destRect3.set(500, 500, 512, 256);

	static ColorRGBA whiteColor;
	whiteColor.set(255, 255, 255, 255);

	static ColorRGBA yellowColor;
	yellowColor.set(255, 255, 0, 255);

	glClear(GL_COLOR_BUFFER_BIT);

	m_shaderProgram.useProgram();

	glActiveTexture(GL_TEXTURE0);
	GLint samplerLoc = m_shaderProgram.getUniformLocation("imageSampler");
	glUniform1i(samplerLoc, 0);

	m_camera.sendMatrixDataToShader(m_shaderProgram);

	m_textureRenderer.begin();

	m_textureRenderer.draw(
		destRect1,
		uvRect,
		m_textureOne.id,
		whiteColor
	);

	m_textureRenderer.draw(
		destRect2,
		uvRect,
		m_textureTwo.id,
		whiteColor
	);

	m_textureRenderer.draw(
		destRect3,
		uvRect,
		m_textureTest.id,
		yellowColor
	);

	m_textureRenderer.end();

	m_textureRenderer.renderTextures();

	m_shaderProgram.unuseProgram();

	SDL_GL_SwapWindow(m_window);
}

void Tetris::freeTetris() {
	m_imageLodaer.freeTexture(m_textureOne);
	m_imageLodaer.deleteTexture(m_textureOne);

	m_imageLodaer.freeTexture(m_textureTwo);
	m_imageLodaer.deleteTexture(m_textureTwo);

	m_imageLodaer.freeTexture(m_textureTest);
	m_imageLodaer.deleteTexture(m_textureTest);

	if (m_window) {
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
	}

	SDL_Quit();
}