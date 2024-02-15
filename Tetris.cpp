#include "Tetris.h"

Tetris::Tetris() {}

Tetris::~Tetris() {
	freeTetris();
}

bool Tetris::init() {
	return
		m_window.init ( SCREEN_WIDTH, SCREEN_HEIGHT, CLEAR_COLOR ) &&
		m_shaderProgram.compileAndLinkShaders ( "resources/shaders/mainShader.vert", "resources/shaders/mainShader.frag" ) &&
		m_fps.init ( MAX_FPS ) &&
		m_camera.init ( m_window.getWindowWidth(), m_window.getWindowHeight() ) &&
		m_lazyFont.initFromFontFile ( "resources/fonts/Quicksand.otf", 32);
}

void Tetris::run() {

	ImageLoader::LoadTextureFromImage("resources/images/square.png", m_textureOne, 4);
	ImageLoader::BufferTextureData(m_textureOne);
	ImageLoader::FreeTexture(m_textureOne);

	ImageLoader::LoadTextureFromImage("resources/images/non-square.png", m_textureTwo, 4);
	ImageLoader::BufferTextureData(m_textureTwo);
	ImageLoader::FreeTexture(m_textureTwo);

	ImageLoader::LoadTextureFromImage("resources/images/color-test.png", m_textureTest, 4);
	ImageLoader::BufferTextureData(m_textureTest);
	ImageLoader::FreeTexture(m_textureTest);

	gameLoop();
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

	glClear(GL_COLOR_BUFFER_BIT);

	m_shaderProgram.useProgram();

	glActiveTexture(GL_TEXTURE0);
	GLint samplerLoc = m_shaderProgram.getUniformLocation("u_imageSampler");
	glUniform1i(samplerLoc, 0);

	m_camera.sendMatrixDataToShader(m_shaderProgram);

	{
		static UVDimension uvRect;
		uvRect.set(0.0f, 0.0f, 1.0f, 1.0f);

		static RectDimension destRect1;
		destRect1.set(100, 100, 256, 256);

		static RectDimension destRect2;
		destRect2.set(100, 600, 270, 180);

		static RectDimension destRect3;
		destRect3.set(m_window.getWindowWidth() / 2, m_window.getWindowHeight() / 2, 512, 256);

		static ColorRGBA whiteColor;
		whiteColor.set(255, 255, 255, 255);

		static ColorRGBA yellowColor;
		yellowColor.set(255, 255, 0, 255);

		static ColorRGBA magentaColor;
		magentaColor.set(255, 0, 255, 255);

		static ColorRGBA blackColor;
		blackColor.set(0, 0, 0, 255);

		if (m_drawUpdateNeeded) {
			m_textureRenderer.begin();

			m_textureRenderer.draw(
				GlyphOrigin::BOTTOM_LEFT,
				destRect1,
				uvRect,
				m_textureOne.id,
				whiteColor
			);

			m_textureRenderer.draw(
				GlyphOrigin::BOTTOM_LEFT,
				destRect2,
				uvRect,
				m_textureTwo.id,
				whiteColor
			);

			m_textureRenderer.draw(
				GlyphOrigin::CENTER,
				destRect3,
				uvRect,
				m_textureTest.id,
				yellowColor
			);

			m_lazyFont.renderText(
				"The name is Thomas Shelby.\n"
				"Always carrying guns.\n\n"
				"Here: +880123456789@@duck.$$$\n"
				"Drugs, smuggling, rapes",
				0, m_window.getWindowHeight(), magentaColor, m_textureRenderer
			);

			m_textureRenderer.end();

			m_drawUpdateNeeded = false;
		}

		m_textureRenderer.renderTextures();
	}

	m_shaderProgram.unuseProgram();

	m_window.swapBuffer();
}

void Tetris::freeTetris() {

	ImageLoader::DeleteTexture(m_textureOne);
	ImageLoader::DeleteTexture(m_textureTwo);
	ImageLoader::DeleteTexture(m_textureTest);

	m_lazyFont.deleteFont();

	m_window.deleteWindow();

	SDL_Quit();
}