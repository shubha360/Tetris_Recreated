#include "Tetrimino.h"

Tetrimino::Tetrimino() {}

Tetrimino::~Tetrimino() {}

void Tetrimino::addToMatrix() {
	if (m_matrix != nullptr) {

		auto& matrix = m_matrix->getMatrix();

		matrix[m_originPosition.y][m_originPosition.x] = m_minoSign;

		for (auto& pos : m_otherMinoPos) {
			matrix[pos.y][pos.x] = m_minoSign;
		}
	}
}

bool Tetrimino::moveLeft() {

	if (!canMinoMoveLeft(m_originPosition)) {
		return false;
	}

	for (auto& minoPos : m_otherMinoPos) {
		if (!canMinoMoveLeft(minoPos)) {
			return false;
		}
	}

	glm::ivec2 newOriginPos(m_originPosition.x - 1, m_originPosition.y);

	glm::ivec2 newOtherMinoPos[3] = {
		glm::ivec2(m_otherMinoPos[0].x - 1, m_otherMinoPos[0].y),
		glm::ivec2(m_otherMinoPos[1].x - 1, m_otherMinoPos[1].y),
		glm::ivec2(m_otherMinoPos[2].x - 1, m_otherMinoPos[2].y)
	};

	performTransformation(newOriginPos, newOtherMinoPos);
	return true;
}

bool Tetrimino::moveRight() {
	if (!canMinoMoveRight(m_originPosition)) {
		return false;
	}

	for (auto& minoPos : m_otherMinoPos) {
		if (!canMinoMoveRight(minoPos)) {
			return false;
		}
	}

	glm::ivec2 newOriginPos(m_originPosition.x + 1, m_originPosition.y);

	glm::ivec2 newOtherMinoPos[3] = {
		glm::ivec2(m_otherMinoPos[0].x + 1, m_otherMinoPos[0].y),
		glm::ivec2(m_otherMinoPos[1].x + 1, m_otherMinoPos[1].y),
		glm::ivec2(m_otherMinoPos[2].x + 1, m_otherMinoPos[2].y)
	};

	performTransformation(newOriginPos, newOtherMinoPos);
	return true;
}

glm::ivec2 Tetrimino::rotateMinoRight(const glm::ivec2& minoPostion) const {
	glm::ivec2 newPos(0);

	// if mino is on the left
	if (minoPostion.x < m_originPosition.x && minoPostion.y == m_originPosition.y) {
		newPos.x = m_originPosition.x;
		newPos.y = m_originPosition.y - (m_originPosition.x - minoPostion.x);
	}

	// if mino is on the right
	else if (minoPostion.x > m_originPosition.x && minoPostion.y == m_originPosition.y) {
		newPos.x = m_originPosition.x;
		newPos.y = m_originPosition.y + (minoPostion.x - m_originPosition.x);
	}

	// if mino is over origin
	else if (minoPostion.x == m_originPosition.x && minoPostion.y < m_originPosition.y) {
		newPos.x = m_originPosition.x + (m_originPosition.y - minoPostion.y);
		newPos.y = m_originPosition.y;
	}

	// if mino is under origin
	else if (minoPostion.x == m_originPosition.x && minoPostion.y > m_originPosition.y) {
		newPos.x = m_originPosition.x - (minoPostion.y - m_originPosition.y);
		newPos.y = m_originPosition.y;
	}

	// if mino is in the top left
	else if (minoPostion.x < m_originPosition.x && minoPostion.y < m_originPosition.y) {
		newPos.x = m_originPosition.x + (m_originPosition.y - minoPostion.y);
		newPos.y = m_originPosition.y - (m_originPosition.x - minoPostion.x);
	}

	// if mino is in the top right
	else if (minoPostion.x > m_originPosition.x && minoPostion.y < m_originPosition.y) {
		newPos.x = m_originPosition.x + (m_originPosition.y - minoPostion.y);
		newPos.y = m_originPosition.y + (minoPostion.x - m_originPosition.x);
	}

	// if mino is in the bottom right
	else if (minoPostion.x > m_originPosition.x && minoPostion.y > m_originPosition.y) {
		newPos.x = m_originPosition.x - (minoPostion.y - m_originPosition.y);
		newPos.y = m_originPosition.y + (minoPostion.x - m_originPosition.x);
	}

	// if mino is in the bottom left
	else if (minoPostion.x < m_originPosition.x && minoPostion.y > m_originPosition.y) {
		newPos.x = m_originPosition.x - (minoPostion.y - m_originPosition.y);
		newPos.y = m_originPosition.y - (m_originPosition.x - minoPostion.x);
	}

	return newPos;
}

glm::ivec2 Tetrimino::rotateMinoLeft(const glm::ivec2& minoPostion) const {
	glm::ivec2 newPos(0);

	// if mino is on the left
	if (minoPostion.x < m_originPosition.x && minoPostion.y == m_originPosition.y) {
		newPos.x = m_originPosition.x;
		newPos.y = m_originPosition.y + (m_originPosition.x - minoPostion.x);
	}

	// if mino is on the right
	else if (minoPostion.x > m_originPosition.x && minoPostion.y == m_originPosition.y) {
		newPos.x = m_originPosition.x;
		newPos.y = m_originPosition.y - (minoPostion.x - m_originPosition.x);
	}

	// if mino is over origin
	else if (minoPostion.x == m_originPosition.x && minoPostion.y < m_originPosition.y) {
		newPos.x = m_originPosition.x - (m_originPosition.y - minoPostion.y);
		newPos.y = m_originPosition.y;
	}

	// if mino is under origin
	else if (minoPostion.x == m_originPosition.x && minoPostion.y > m_originPosition.y) {
		newPos.x = m_originPosition.x + (minoPostion.y - m_originPosition.y);
		newPos.y = m_originPosition.y;
	}

	// if mino is in the top left
	else if (minoPostion.x < m_originPosition.x && minoPostion.y < m_originPosition.y) {
		newPos.x = m_originPosition.x - (m_originPosition.y - minoPostion.y);
		newPos.y = m_originPosition.y + (m_originPosition.x - minoPostion.x);
	}

	// if mino is in the top right
	else if (minoPostion.x > m_originPosition.x && minoPostion.y < m_originPosition.y) {
		newPos.x = m_originPosition.x - (m_originPosition.y - minoPostion.y);
		newPos.y = m_originPosition.y - (minoPostion.x - m_originPosition.x);
	}

	// if mino is in the bottom right
	else if (minoPostion.x > m_originPosition.x && minoPostion.y > m_originPosition.y) {
		newPos.x = m_originPosition.x + (minoPostion.y - m_originPosition.y);
		newPos.y = m_originPosition.y - (minoPostion.x - m_originPosition.x);
	}

	// if mino is in the bottom left
	else if (minoPostion.x < m_originPosition.x && minoPostion.y > m_originPosition.y) {
		newPos.x = m_originPosition.x + (minoPostion.y - m_originPosition.y);
		newPos.y = m_originPosition.y + (m_originPosition.x - minoPostion.x);
	}

	return newPos;
}

Tetrimino_T::Tetrimino_T(int originPositionX, int originPositionY, Matrix* matrix) {
	m_originPosition = glm::vec2(originPositionX, originPositionY);

	m_otherMinoPos.resize(3);
	m_otherMinoPos[0] = glm::ivec2(m_originPosition.x - 1, m_originPosition.y);
	m_otherMinoPos[1] = glm::ivec2(m_originPosition.x, m_originPosition.y - 1);
	m_otherMinoPos[2] = glm::ivec2(m_originPosition.x + 1, m_originPosition.y);

	m_minoSign = 'T';
	m_matrix = matrix;
}

Tetrimino_T::~Tetrimino_T() {}

bool Tetrimino_T::rotateRight() {
	glm::ivec2 newMinoPos[3] = {
		rotateMinoRight(m_otherMinoPos[0]),
		rotateMinoRight(m_otherMinoPos[1]),
		rotateMinoRight(m_otherMinoPos[2])
	};

	auto& matrix = m_matrix->getMatrix();

	// if all the cells after the rotation are empty or part of this tetrimino
	if (
		isMinoInsideMatrix(newMinoPos[0]) &&
		(matrix[newMinoPos[0].y][newMinoPos[0].x] == m_matrix->getEmptyCellSign() || isCellPartOfThis(newMinoPos[0])) &&
		isMinoInsideMatrix(newMinoPos[1]) &&
		(matrix[newMinoPos[1].y][newMinoPos[1].x] == m_matrix->getEmptyCellSign() || isCellPartOfThis(newMinoPos[1])) &&
		isMinoInsideMatrix(newMinoPos[2]) &&
		(matrix[newMinoPos[2].y][newMinoPos[2].x] == m_matrix->getEmptyCellSign() || isCellPartOfThis(newMinoPos[2]))
		) {

		performTransformation(m_originPosition, newMinoPos);
		return true;
	}
	return false;
}

bool Tetrimino_T::rotateLeft() {
	glm::ivec2 newMinoPos[3] = {
		rotateMinoLeft(m_otherMinoPos[0]),
		rotateMinoLeft(m_otherMinoPos[1]),
		rotateMinoLeft(m_otherMinoPos[2])
	};

	auto& matrix = m_matrix->getMatrix();

	// if all the cells after the rotation are empty
	if (
		isMinoInsideMatrix(newMinoPos[0]) &&
		(matrix[newMinoPos[0].y][newMinoPos[0].x] == m_matrix->getEmptyCellSign() || isCellPartOfThis(newMinoPos[0])) &&
		isMinoInsideMatrix(newMinoPos[1]) &&
		(matrix[newMinoPos[1].y][newMinoPos[1].x] == m_matrix->getEmptyCellSign() || isCellPartOfThis(newMinoPos[1])) &&
		isMinoInsideMatrix(newMinoPos[2]) &&
		(matrix[newMinoPos[2].y][newMinoPos[2].x] == m_matrix->getEmptyCellSign() || isCellPartOfThis(newMinoPos[2]))
		) {
		performTransformation(m_originPosition, newMinoPos);
		return true;
		// UPDATE COORDINATES
	}
	return false;
}

Tetrimino_L::Tetrimino_L(int originPositionX, int originPositionY, Matrix* matrix) {
	m_originPosition = glm::vec2(originPositionX, originPositionY);

	m_otherMinoPos.resize(3);
	m_otherMinoPos[0] = glm::ivec2(m_originPosition.x - 1, m_originPosition.y);
	m_otherMinoPos[1] = glm::ivec2(m_originPosition.x + 1, m_originPosition.y);
	m_otherMinoPos[2] = glm::ivec2(m_originPosition.x + 1, m_originPosition.y - 1);

	m_minoSign = 'L';
	m_matrix = matrix;
}

Tetrimino_L::~Tetrimino_L() {}

bool Tetrimino_L::rotateRight() {
	glm::ivec2 newMinoPos[3] = {
		rotateMinoRight(m_otherMinoPos[0]),
		rotateMinoRight(m_otherMinoPos[1]),
		rotateMinoRight(m_otherMinoPos[2])
	};

	auto& matrix = m_matrix->getMatrix();

	// if all the cells after the rotation are empty
	if (
		isMinoInsideMatrix(newMinoPos[0]) &&
		(matrix[newMinoPos[0].y][newMinoPos[0].x] == m_matrix->getEmptyCellSign() || isCellPartOfThis(newMinoPos[0])) &&
		isMinoInsideMatrix(newMinoPos[1]) &&
		(matrix[newMinoPos[1].y][newMinoPos[1].x] == m_matrix->getEmptyCellSign() || isCellPartOfThis(newMinoPos[1])) &&
		isMinoInsideMatrix(newMinoPos[2]) &&
		(matrix[newMinoPos[2].y][newMinoPos[2].x] == m_matrix->getEmptyCellSign() || isCellPartOfThis(newMinoPos[2]))
		) {

		performTransformation(m_originPosition, newMinoPos);
		return true;
	}
	return false;
}

bool Tetrimino_L::rotateLeft() {
	glm::ivec2 newMinoPos[3] = {
		rotateMinoLeft(m_otherMinoPos[0]),
		rotateMinoLeft(m_otherMinoPos[1]),
		rotateMinoLeft(m_otherMinoPos[2])
	};

	auto& matrix = m_matrix->getMatrix();

	// if all the cells after the rotation are empty
	if (
		isMinoInsideMatrix(newMinoPos[0]) && 
		(matrix[newMinoPos[0].y][newMinoPos[0].x] == m_matrix->getEmptyCellSign() || isCellPartOfThis(newMinoPos[0])) &&
		isMinoInsideMatrix(newMinoPos[1]) && 
		(matrix[newMinoPos[1].y][newMinoPos[1].x] == m_matrix->getEmptyCellSign() || isCellPartOfThis(newMinoPos[1])) &&
		isMinoInsideMatrix(newMinoPos[2]) && 
		(matrix[newMinoPos[2].y][newMinoPos[2].x] == m_matrix->getEmptyCellSign() || isCellPartOfThis(newMinoPos[2]))
		) {
		
		performTransformation(m_originPosition, newMinoPos);
		return true;
	}
	return false;
}

void Tetrimino::performTransformation(glm::ivec2 newOriginPos, glm::ivec2 newMinoPos[3]) {

	auto& matrix = m_matrix->getMatrix();

	matrix[m_originPosition.y][m_originPosition.x] = m_matrix->getEmptyCellSign();
	matrix[m_otherMinoPos[0].y][m_otherMinoPos[0].x] = m_matrix->getEmptyCellSign();
	matrix[m_otherMinoPos[1].y][m_otherMinoPos[1].x] = m_matrix->getEmptyCellSign();
	matrix[m_otherMinoPos[2].y][m_otherMinoPos[2].x] = m_matrix->getEmptyCellSign();

	m_originPosition = newOriginPos;
	m_otherMinoPos[0] = newMinoPos[0];
	m_otherMinoPos[1] = newMinoPos[1];
	m_otherMinoPos[2] = newMinoPos[2];

	matrix[m_originPosition.y][m_originPosition.x] = m_minoSign;
	matrix[m_otherMinoPos[0].y][m_otherMinoPos[0].x] = m_minoSign;
	matrix[m_otherMinoPos[1].y][m_otherMinoPos[1].x] = m_minoSign;
	matrix[m_otherMinoPos[2].y][m_otherMinoPos[2].x] = m_minoSign;
}

bool Tetrimino::canMinoMoveLeft(const glm::ivec2& minoPos) {

	auto& matrix = m_matrix->getMatrix();

	return minoPos.x > 0 && 
		(matrix[minoPos.y][minoPos.x - 1] == m_matrix->getEmptyCellSign() ||
			isCellPartOfThis(glm::ivec2(minoPos.x - 1, minoPos.y)));
}

bool Tetrimino::canMinoMoveRight(const glm::ivec2& minoPos) {
	
	auto& matrix = m_matrix->getMatrix();

	return minoPos.x < m_matrix->getWidth() - 1 &&
		(matrix[minoPos.y][minoPos.x + 1] == m_matrix->getEmptyCellSign() ||
			isCellPartOfThis(glm::ivec2(minoPos.x + 1, minoPos.y)));;
}

bool Tetrimino::isCellPartOfThis(const glm::ivec2& cellPos) {
	return
		cellPos == m_originPosition ||
		cellPos == m_otherMinoPos[0] ||
		cellPos == m_otherMinoPos[1] ||
		cellPos == m_otherMinoPos[2];
}

bool Tetrimino::isMinoInsideMatrix(const glm::ivec2& minoPos) {

	return (minoPos.x >= 0 && minoPos.x < m_matrix->getWidth()) && 
		(minoPos.y >= 0 && minoPos.y < m_matrix->getHeight());
}
