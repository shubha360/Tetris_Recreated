#include "../include/ExtraMatrix.h"

ExtraMatrix::ExtraMatrix() {}

ExtraMatrix::~ExtraMatrix() {}

void ExtraMatrix::init(std::vector<Tetrimino*> tetriminoes, const glm::ivec2& topLeftPos, 
	GLuint minoTextureId, const int minoLength /*= 16*/) {

	m_tetriminoes = tetriminoes;
	int numRows = m_tetriminoes.size() * m_tetriminoMaxHeight;

	Matrix::init(topLeftPos, minoTextureId, numRows, 8, minoLength);

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
