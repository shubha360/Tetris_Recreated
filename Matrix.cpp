#include "Matrix.h"

Matrix::Matrix() {
}

Matrix::~Matrix() {
	ImageLoader::DeleteTexture(m_minoTexture);
}

bool Matrix::init(unsigned int screenWidth, unsigned int screenHeight) {

	m_screenDimension.x = screenWidth;
	m_screenDimension.y = screenHeight;

	m_matrixPosTopLeft.x = 200;
	m_matrixPosTopLeft.y = screenHeight - 200;

	ImageLoader::LoadTextureFromImage("resources/images/mino.png", m_minoTexture, 4);
	ImageLoader::BufferTextureData(m_minoTexture);
	ImageLoader::FreeTexture(m_minoTexture);
	
	m_matrix.resize(NUM_ROWS);

	for (int i = 0; i < m_matrix.size(); i++) {
		m_matrix[i] = std::string(NUM_COLUMNS, EMPTY_CELL_SIGN);
	}

	return true;
}

void Matrix::addTetrimino(Tetrimino* terimino) {
	m_matrix[terimino->m_originPosition.y][terimino->m_originPosition.x] = terimino->m_minoChar;

	for (auto& pos : terimino->m_otherMinoPos) {
		m_matrix[pos.y][pos.x] = terimino->m_minoChar;
	}
}

void Matrix::drawMatrix(TextureRenderer& renderer) const {

	static UVDimension minoUV { 0.0f, 0.0f, 1.0f, 1.0f };
	
	ColorRGBA minoColor {};

	for (int row = 0; row < NUM_ROWS; row++) {
		for (int column = 0; column < NUM_COLUMNS; column++) {
			
			// determine color
			switch (m_matrix[row][column]) {
			
			case ' ':
				minoColor = MINO_COLOR_BLACK;
				break;

			case 'T':
				minoColor = MINO_COLOR_MAGENTA;
				break;

			case 'L':
				minoColor = MINO_COLOR_ORANGE;
				break;
			}

			RectDimension minoDimension{ 
				m_matrixPosTopLeft.x + column * MINO_LENGTH,
				m_matrixPosTopLeft.y - row * MINO_LENGTH, 
				MINO_LENGTH, MINO_LENGTH };


			renderer.draw(
				GlyphOrigin::TOP_LEFT,
				minoDimension,
				minoUV,
				m_minoTexture.id,
				minoColor
			);
		}
	}
}
