#include "../include/ExtraMatrix.h"

ExtraMatrix::ExtraMatrix() {}

ExtraMatrix::~ExtraMatrix() {}

void ExtraMatrix::init(std::vector<Tetrimino*> tetriminoes, const std::string& name, Evolve::Font& font, 
	const float nameScale, const Evolve::ColorRgba nameColor,
	const glm::ivec2& topLeftPos, GLuint minoTextureId, const int minoLength /*= 16*/) {

	int numRows = tetriminoes.size() * m_tetriminoMaxHeight;
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

	addTetriminoes(tetriminoes);
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

	addTetriminoes(m_tetriminoes);

	return pop;
}

void ExtraMatrix::reset(std::vector<Tetrimino*> tetriminoes) {
	if (m_tetriminoes.size() != tetriminoes.size()) {
		EVOLVE_REPORT_ERROR("Invalid vector passed.", reset);
		return;
	}
	Matrix::reset();
	addTetriminoes(tetriminoes);
}

void ExtraMatrix::drawName(Evolve::TextureRenderer& textureRenderer) {
	m_font.setFontScale(m_nameScale);
	m_font.drawTextToRenderer(m_name, m_namePos.x, m_namePos.y, m_nameColor, textureRenderer);
}

void ExtraMatrix::addTetriminoes(std::vector<Tetrimino*> tetriminoes) {

	m_tetriminoes = tetriminoes;

	for (int i = 0; i < m_tetriminoes.size(); i++) {

		auto& current = m_tetriminoes[i];

		if (current != nullptr) {
			int originX = m_numColumns / 2;
			int originY = i * m_tetriminoMaxHeight + 2;

			m_matrix[originY][originX] = current->m_minoSign;

			for (int j = 0; j < 4; j++) {
				if (j != current->m_originMinoIndex) {
					int minoX = 0, minoY = 0;

					minoX = originX +
						(current->m_spawnMinoPositions[j].x - current->m_spawnMinoPositions[current->m_originMinoIndex].x);

					minoY = originY +
						(current->m_spawnMinoPositions[j].y - current->m_spawnMinoPositions[current->m_originMinoIndex].y);

					m_matrix[minoY][minoX] = current->m_minoSign;
				}
			}
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
