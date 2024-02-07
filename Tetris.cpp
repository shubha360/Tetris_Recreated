#include "Tetris.h"

Tetris::Tetris() { 

	/*glm::vec3 traslationMatrix(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);

	m_projectionMatrix = glm::translate(m_projectionMatrix, traslationMatrix);*/
}

Tetris::~Tetris() {
	freeTetris();
}

bool Tetris::init() {
	return 
		initSdlWindow() &&
		initGL() &&
		m_shaderProgram.compileAndLinkShaders("Assets/Shaders/mainShader.vert", "Assets/Shaders/mainShader.frag") &&
		fps.init(MAX_FPS);
}

void Tetris::run() {

	m_indicies[0] = 0;
	m_indicies[1] = 1;
	m_indicies[2] = 2;
	m_indicies[3] = 0;
	m_indicies[4] = 3;
	m_indicies[5] = 2;

	ColorRGBA vertexColor = { 141, 229, 0, 255 };

	m_verticies[0].setPosition(EXIT_BUTTON_X, EXIT_BUTTON_Y);
	m_verticies[0].setColor(vertexColor);

	m_verticies[1].setPosition(EXIT_BUTTON_X + EXIT_BUTTON_WIDTH, EXIT_BUTTON_Y);
	m_verticies[1].setColor(vertexColor);

	m_verticies[2].setPosition(EXIT_BUTTON_X + EXIT_BUTTON_WIDTH, EXIT_BUTTON_Y + EXIT_BUTTON_HEIGHT);
	m_verticies[2].setColor(vertexColor);
	
	m_verticies[3].setPosition(EXIT_BUTTON_X, EXIT_BUTTON_Y + EXIT_BUTTON_HEIGHT);
	m_verticies[3].setColor(vertexColor);

	glGenVertexArrays(1, &m_vaoID);
	glGenBuffers(1, &m_vboID);
	glGenBuffers(1, &m_iboID);

	glBindVertexArray(m_vaoID);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	//glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 2, GL_INT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, position));
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, color));
	/*glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, textureCoords));*/

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(Vertex2D), m_verticies, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12 * sizeof(GLuint), m_indicies, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	gameLoop();
}

bool Tetris::initSdlWindow() {

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("Failed to initialize SDL! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	int attribResponse = 0;

	attribResponse += SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	attribResponse += SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	attribResponse += SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	attribResponse += SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	if (attribResponse < 0) {
		printf("Failed to set an SDL_GL attribute! Error: %s\n", SDL_GetError());
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
		printf("Failed to create window! Error: %s\n", SDL_GetError());
		return false;
	}

	return true;
}

bool Tetris::initGL() {

	SDL_GLContext glContext = SDL_GL_CreateContext(m_window);

	if (glContext == nullptr) {
		printf("Failed to create GL context! Error: %s\n", SDL_GetError());
		return false;
	}

	GLenum response = glewInit();

	if (response != GLEW_OK) {
		printf("Failed to initialize GLEW!\n");
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
	SDL_Event event;
	bool gameLoop = true;

	while (gameLoop) {

		fps.beginFrame();

		gameLoop = processInput(&event);

		draw();

		float currentFps = fps.calculateFps();

		static int frameCount = 0;
		frameCount++;

		if (frameCount == 100) {
			printf("%f\n", currentFps);
			frameCount = 0;
		}

		fps.endFrame();
	}
}

bool Tetris::processInput(SDL_Event* event) {
	
	while (SDL_PollEvent(event)) {
		if (event->type == SDL_QUIT) {
			return false;
		}
		if (event->type == SDL_MOUSEBUTTONDOWN) {
			if (event->button.button == SDL_BUTTON_LEFT) {
				if ((event->motion.x >= EXIT_BUTTON_X && event->motion.x <= EXIT_BUTTON_X + EXIT_BUTTON_WIDTH)
					&&
					(event->motion.y >= EXIT_BUTTON_Y && event->motion.y <= EXIT_BUTTON_Y + EXIT_BUTTON_HEIGHT)) 
				{
					return false;
				}
			}
		}
	}
	return true;
}

void Tetris::draw() {
	glClear(GL_COLOR_BUFFER_BIT);

	m_shaderProgram.useProgram();

	GLint mvpLoc = m_shaderProgram.getUniformLocation("mvpMatrix");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &m_mvp[0][0]);

	glBindVertexArray(m_vaoID);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	m_shaderProgram.unuseProgram();

	SDL_GL_SwapWindow(m_window);
}

void Tetris::freeTetris() {
	if (m_window) {
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
	}
	if (m_iboID) {
		glDeleteBuffers(1, &m_iboID);
	}

	if (m_vboID) {
		glDeleteBuffers(1, &m_vboID);
	}

	if (m_vaoID) {
		glDeleteVertexArrays(1, &m_vaoID);
	}

	SDL_Quit();
}