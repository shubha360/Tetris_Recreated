#include "../include/ExtraMatrix.h"

ExtraMatrix::ExtraMatrix() {}

ExtraMatrix::~ExtraMatrix() {}

void ExtraMatrix::init(std::vector<Tetrimino*> tetriminoes, const std::string& name, Evolve::Font& font, 
	const float nameScale, const Evolve::ColorRgba nameColor,
	const glm::ivec2& topLeftPos, GLuint minoTextureId, const int minoLength /*= 16*/) {

	m_tetriminoes = tetriminoes;

	int numRows = m_tetriminoes.size() * m_tetriminoMaxHeight;
	int numColumns = 8;

	m_name = name;
	m_font = font;
	m_nameScale = nameScale;
	m_nameColor = nameColor;

	m_font.setFontScale(m_nameScale);

	// set name position, center the name horizontally
	unsigned int nameWidth = m_font.getLineWidth(m_name);
	m_namePos.x = topLeftPos.x + ((numColumns * minoLength) - nameWidth) / 2;
	m_namePos.y = topLeftPos.y;

	// set matrix position, just add the height of the font to Y
	unsigned int nameHeight = m_font.getLineHeight();
	m_matrixPos.x = topLeftPos.x;
	m_matrixPos.y = topLeftPos.y - nameHeight - 10;

	Matrix::init(m_matrixPos, minoTextureId, numRows, 8, minoLength);

	for (int i = 0; i < m_tetriminoes.size(); i++) {
		if (m_tetriminoes[i] != nullptr) {
			addTetrimino(i, m_tetriminoes[i]);
		}
	}
}

Tetrimino* ExtraMatrix::pushAndPop(Tetrimino* newTetrimino) {

	Tetrimino* pop = m_tetriminoes[0];

	for (int i = 0; i < m_tetriminoes.size() - 1; i++) {
		m_tetriminoes[i] = m_tetriminoes[i + 1];
	}

	m_tetriminoes[m_tetriminoes.size() - 1] = newTetrimino;

	/*m_tetriminoes[0] = m_tetriminoes[1];
	m_tetriminoes[1] = m_tetriminoes[2];
	m_tetriminoes[2] = newTetrimino;*/

	emptyMatrix();

	for (int i = 0; i < m_tetriminoes.size(); i++) {
		if (m_tetriminoes[i] != nullptr) {
			addTetrimino(i, m_tetriminoes[i]);
		}
	}

	return pop;
}

void ExtraMatrix::drawName(Evolve::TextureRenderer& textureRenderer) {
	m_font.setFontScale(m_nameScale);
	m_font.drawTextToRenderer(m_name, m_namePos.x, m_namePos.y, m_nameColor, textureRenderer);
}

void ExtraMatrix::addTetrimino(int index, Tetrimino* tetrimino) {
	int originX = m_numColumns / 2;
	int originY = index * m_tetriminoMaxHeight + 2;

	m_matrix[originY][originX] = tetrimino->m_minoSign;

	for (int i = 0; i < 4; i++) {
		if (i != tetrimino->m_originMinoIndex) {
			int minoX = 0, minoY = 0;

			minoX = originX + 
				(tetrimino->m_spawnMinoPositions[i].x - tetrimino->m_spawnMinoPositions[tetrimino->m_originMinoIndex].x);

			minoY = originY +
				(tetrimino->m_spawnMinoPositions[i].y - tetrimino->m_spawnMinoPositions[tetrimino->m_originMinoIndex].y);

			m_matrix[minoY][minoX] = tetrimino->m_minoSign;
		}
	}
}

void ExtraMatrix::emptyMatrix() {
	for (auto& row : m_matrix) {
		for (auto& cell : row) {
			cell = ' ';
		}
	}
}
