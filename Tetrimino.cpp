#include "Tetrimino.h"

Tetrimino::Tetrimino() {}

Tetrimino::~Tetrimino() {}

void Tetrimino::addToMatrix() {
	if (m_matrix != nullptr) {

		auto& matrix = m_matrix->getMatrix();

		for (int i = 0; i < 4; i++) {
			matrix[m_minoPositions[i].y][m_minoPositions[i].x] = m_minoSign;
		}
	}
}

bool Tetrimino::moveLeft() {
	for (int i = 0; i < 4; i++) {
		if (!canMinoMoveLeft(m_minoPositions[i])) {
			return false;
		}
	}

	glm::ivec2 newMinoPos[4] = {
		glm::ivec2(m_minoPositions[0].x - 1, m_minoPositions[0].y),
		glm::ivec2(m_minoPositions[1].x - 1, m_minoPositions[1].y),
		glm::ivec2(m_minoPositions[2].x - 1, m_minoPositions[2].y),
		glm::ivec2(m_minoPositions[3].x - 1, m_minoPositions[3].y),

	};

	performTransformation(newMinoPos);
	return true;
}

bool Tetrimino::moveRight() {
	for (int i = 0; i < 4; i++) {
		if (!canMinoMoveRight(m_minoPositions[i])) {
			return false;
		}
	}

	glm::ivec2 newMinoPos[4] = {
		glm::ivec2(m_minoPositions[0].x + 1, m_minoPositions[0].y),
		glm::ivec2(m_minoPositions[1].x + 1, m_minoPositions[1].y),
		glm::ivec2(m_minoPositions[2].x + 1, m_minoPositions[2].y),
		glm::ivec2(m_minoPositions[3].x + 1, m_minoPositions[3].y),

	};

	performTransformation(newMinoPos);
	return true;
}

bool Tetrimino::moveDown() {
	for (int i = 0; i < 4; i++) {
		if (!canMinoMoveDown(m_minoPositions[i])) {
			return false;
		}
	}

	glm::ivec2 newMinoPos[4] = {
		glm::ivec2(m_minoPositions[0].x, m_minoPositions[0].y + 1),
		glm::ivec2(m_minoPositions[1].x, m_minoPositions[1].y + 1),
		glm::ivec2(m_minoPositions[2].x, m_minoPositions[2].y + 1),
		glm::ivec2(m_minoPositions[3].x, m_minoPositions[3].y + 1),

	};

	performTransformation(newMinoPos);
	return true;
}

glm::ivec2 Tetrimino::rotateMinoRight(const glm::ivec2& minoPostion) const {
	glm::ivec2 newPos(0);

	glm::ivec2 originMino = m_minoPositions[m_originMinoIndex];

	// if mino is on the left
	if (minoPostion.x < originMino.x && minoPostion.y == originMino.y) {
		newPos.x = originMino.x;
		newPos.y = originMino.y - (originMino.x - minoPostion.x);
	}

	// if mino is on the right
	else if (minoPostion.x > originMino.x && minoPostion.y == originMino.y) {
		newPos.x = originMino.x;
		newPos.y = originMino.y + (minoPostion.x - originMino.x);
	}

	// if mino is over origin
	else if (minoPostion.x == originMino.x && minoPostion.y < originMino.y) {
		newPos.x = originMino.x + (originMino.y - minoPostion.y);
		newPos.y = originMino.y;
	}

	// if mino is under origin
	else if (minoPostion.x == originMino.x && minoPostion.y > originMino.y) {
		newPos.x = originMino.x - (minoPostion.y - originMino.y);
		newPos.y = originMino.y;
	}

	// if mino is in the top left
	else if (minoPostion.x < originMino.x && minoPostion.y < originMino.y) {
		newPos.x = originMino.x + (originMino.y - minoPostion.y);
		newPos.y = originMino.y - (originMino.x - minoPostion.x);
	}

	// if mino is in the top right
	else if (minoPostion.x > originMino.x && minoPostion.y < originMino.y) {
		newPos.x = originMino.x + (originMino.y - minoPostion.y);
		newPos.y = originMino.y + (minoPostion.x - originMino.x);
	}

	// if mino is in the bottom right
	else if (minoPostion.x > originMino.x && minoPostion.y > originMino.y) {
		newPos.x = originMino.x - (minoPostion.y - originMino.y);
		newPos.y = originMino.y + (minoPostion.x - originMino.x);
	}

	// if mino is in the bottom left
	else if (minoPostion.x < originMino.x && minoPostion.y > originMino.y) {
		newPos.x = originMino.x - (minoPostion.y - originMino.y);
		newPos.y = originMino.y - (originMino.x - minoPostion.x);
	}

	return newPos;
}

glm::ivec2 Tetrimino::rotateMinoLeft(const glm::ivec2& minoPostion) const {
	glm::ivec2 newPos(0);

	glm::ivec2 originMino = m_minoPositions[m_originMinoIndex];

	// if mino is on the left
	if (minoPostion.x < originMino.x && minoPostion.y == originMino.y) {
		newPos.x = originMino.x;
		newPos.y = originMino.y + (originMino.x - minoPostion.x);
	}

	// if mino is on the right
	else if (minoPostion.x > originMino.x && minoPostion.y == originMino.y) {
		newPos.x = originMino.x;
		newPos.y = originMino.y - (minoPostion.x - originMino.x);
	}

	// if mino is over origin
	else if (minoPostion.x == originMino.x && minoPostion.y < originMino.y) {
		newPos.x = originMino.x - (originMino.y - minoPostion.y);
		newPos.y = originMino.y;
	}

	// if mino is under origin
	else if (minoPostion.x == originMino.x && minoPostion.y > originMino.y) {
		newPos.x = originMino.x + (minoPostion.y - originMino.y);
		newPos.y = originMino.y;
	}

	// if mino is in the top left
	else if (minoPostion.x < originMino.x && minoPostion.y < originMino.y) {
		newPos.x = originMino.x - (originMino.y - minoPostion.y);
		newPos.y = originMino.y + (originMino.x - minoPostion.x);
	}

	// if mino is in the top right
	else if (minoPostion.x > originMino.x && minoPostion.y < originMino.y) {
		newPos.x = originMino.x - (originMino.y - minoPostion.y);
		newPos.y = originMino.y - (minoPostion.x - originMino.x);
	}

	// if mino is in the bottom right
	else if (minoPostion.x > originMino.x && minoPostion.y > originMino.y) {
		newPos.x = originMino.x + (minoPostion.y - originMino.y);
		newPos.y = originMino.y - (minoPostion.x - originMino.x);
	}

	// if mino is in the bottom left
	else if (minoPostion.x < originMino.x && minoPostion.y > originMino.y) {
		newPos.x = originMino.x + (minoPostion.y - originMino.y);
		newPos.y = originMino.y + (originMino.x - minoPostion.x);
	}

	return newPos;
}

void Tetrimino::performTransformation(glm::ivec2 newMinoPos[4]) {

	auto& matrix = m_matrix->getMatrix();

	// empty the old cells in the matrix
	for (int i = 0; i < 4; i++) {
		matrix[m_minoPositions[i].y][m_minoPositions[i].x] = m_matrix->getEmptyCellSign();
	}

	// setting the new mino positions
	for (int i = 0; i < 4; i++) {
		m_minoPositions[i] = newMinoPos[i];
	}

	// fill up the new cells in the matrix
	for (int i = 0; i < 4; i++) {
		matrix[m_minoPositions[i].y][m_minoPositions[i].x] = m_minoSign;
	}
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

bool Tetrimino::canMinoMoveDown(const glm::ivec2& minoPos) {
	
	auto& matrix = m_matrix->getMatrix();

	return minoPos.y < m_matrix->getHeight() - 1 &&
		(matrix[minoPos.y + 1][minoPos.x] == m_matrix->getEmptyCellSign() ||
			isCellPartOfThis(glm::ivec2(minoPos.x, minoPos.y + 1)));;
}

bool Tetrimino::isCellPartOfThis(const glm::ivec2& cellPos) const {
	return
		cellPos == m_minoPositions[0] ||
		cellPos == m_minoPositions[1] ||
		cellPos == m_minoPositions[2] ||
		cellPos == m_minoPositions[3];
}

bool Tetrimino::isMinoInsideMatrix(const glm::ivec2& minoPos) {

	return (minoPos.x >= 0 && minoPos.x < m_matrix->getWidth()) && 
		(minoPos.y >= 0 && minoPos.y < m_matrix->getHeight());
}

void Tetrimino::changeOrientation() {
	if (m_orientation == Orientation::HORIZONTAL) {
		m_orientation = Orientation::VERTICAL;
	}
	else {
		m_orientation = Orientation::HORIZONTAL;
	}
}

Tetrimino_T::Tetrimino_T(int originPositionX, int originPositionY, Matrix* matrix) {
	m_originMinoIndex = 1;
	m_minoPositions[m_originMinoIndex] = glm::ivec2(originPositionX, originPositionY);

	m_minoPositions[0] = glm::ivec2(originPositionX - 1, originPositionY);
	m_minoPositions[2] = glm::ivec2(originPositionX, originPositionY - 1);
	m_minoPositions[3] = glm::ivec2(originPositionX + 1, originPositionY);

	m_minoSign = 'T';
	m_matrix = matrix;
	m_spawnPositionX = m_matrix->getWidth() % 2 == 0 ? m_matrix->getWidth() / 2 - 1 : m_matrix->getWidth() / 2;
	m_spawnPositionY = 1;
	m_orientation = Orientation::HORIZONTAL;
}

Tetrimino_T::~Tetrimino_T() {}

bool Tetrimino_T::rotateRight() {
	glm::ivec2 newMinoPos[4] = {};

	for (int i = 0; i < 4; i++) {
		if (i == m_originMinoIndex) {
			newMinoPos[i] = m_minoPositions[i];
		}
		else {
			newMinoPos[i] = rotateMinoRight(m_minoPositions[i]);
		}
	}

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

		performTransformation(newMinoPos);
		changeOrientation();
		return true;
	}
	return false;
}

bool Tetrimino_T::rotateLeft() {
	glm::ivec2 newMinoPos[4] = {};

	for (int i = 0; i < 4; i++) {
		if (i == m_originMinoIndex) {
			newMinoPos[i] = m_minoPositions[i];
		}
		else {
			newMinoPos[i] = rotateMinoLeft(m_minoPositions[i]);
		}
	}

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
		
		performTransformation(newMinoPos);
		changeOrientation();
		return true;
	}
	return false;
}

Tetrimino_L::Tetrimino_L(int originPositionX, int originPositionY, Matrix* matrix) {
	m_originMinoIndex = 1;
	m_minoPositions[m_originMinoIndex] = glm::ivec2(originPositionX, originPositionY);

	m_minoPositions[0] = glm::ivec2(originPositionX - 1, originPositionY);
	m_minoPositions[2] = glm::ivec2(originPositionX + 1, originPositionY);
	m_minoPositions[3] = glm::ivec2(originPositionX + 1, originPositionY - 1);

	m_minoSign = 'L';
	m_matrix = matrix;
	m_spawnPositionX = m_matrix->getWidth() % 2 == 0 ? m_matrix->getWidth() / 2 - 1 : m_matrix->getWidth() / 2;
	m_spawnPositionY = 1;
	m_orientation = Orientation::HORIZONTAL;
}

Tetrimino_L::~Tetrimino_L() {}

bool Tetrimino_L::rotateRight() {
	glm::ivec2 newMinoPos[4] = {};

	for (int i = 0; i < 4; i++) {
		if (i == m_originMinoIndex) {
			newMinoPos[i] = m_minoPositions[i];
		}
		else {
			newMinoPos[i] = rotateMinoRight(m_minoPositions[i]);
		}
	}

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

		performTransformation(newMinoPos);
		changeOrientation();
		return true;
	}
	return false;
}

bool Tetrimino_L::rotateLeft() {
	glm::ivec2 newMinoPos[4] = {};

	for (int i = 0; i < 4; i++) {
		if (i == m_originMinoIndex) {
			newMinoPos[i] = m_minoPositions[i];
		}
		else {
			newMinoPos[i] = rotateMinoLeft(m_minoPositions[i]);
		}
	}

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

		performTransformation(newMinoPos);
		changeOrientation();
		return true;
	}
	return false;
}

Tetrimino_J::Tetrimino_J(int originPositionX, int originPositionY, Matrix* matrix) {
	m_originMinoIndex = 2;
	m_minoPositions[m_originMinoIndex] = glm::ivec2(originPositionX, originPositionY);

	m_minoPositions[0] = glm::ivec2(originPositionX - 1, originPositionY - 1);
	m_minoPositions[1] = glm::ivec2(originPositionX - 1, originPositionY);
	m_minoPositions[3] = glm::ivec2(originPositionX + 1, originPositionY);

	m_minoSign = 'J';
	m_matrix = matrix;
	m_spawnPositionX = m_matrix->getWidth() % 2 == 0 ? m_matrix->getWidth() / 2 - 1 : m_matrix->getWidth() / 2;
	m_spawnPositionY = 1;
	m_orientation = Orientation::HORIZONTAL;
}

Tetrimino_J::~Tetrimino_J() {}

bool Tetrimino_J::rotateRight() {
	glm::ivec2 newMinoPos[4] = {};

	for (int i = 0; i < 4; i++) {
		if (i == m_originMinoIndex) {
			newMinoPos[i] = m_minoPositions[i];
		}
		else {
			newMinoPos[i] = rotateMinoRight(m_minoPositions[i]);
		}
	}

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

		performTransformation(newMinoPos);
		changeOrientation();
		return true;
	}
	return false;
}

bool Tetrimino_J::rotateLeft() {
	glm::ivec2 newMinoPos[4] = {};

	for (int i = 0; i < 4; i++) {
		if (i == m_originMinoIndex) {
			newMinoPos[i] = m_minoPositions[i];
		}
		else {
			newMinoPos[i] = rotateMinoLeft(m_minoPositions[i]);
		}
	}

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

		performTransformation(newMinoPos);
		changeOrientation();
		return true;
	}
	return false;
}

Tetrimino_O::Tetrimino_O(int originPositionX, int originPositionY, Matrix* matrix) {
	m_originMinoIndex = 3;
	m_minoPositions[m_originMinoIndex] = glm::ivec2(originPositionX, originPositionY);

	m_minoPositions[0] = glm::ivec2(originPositionX - 1, originPositionY - 1);
	m_minoPositions[1] = glm::ivec2(originPositionX, originPositionY - 1);
	m_minoPositions[2] = glm::ivec2(originPositionX - 1, originPositionY);

	m_minoSign = 'O';
	m_matrix = matrix;
	m_spawnPositionX = m_matrix->getWidth() % 2 == 0 ? m_matrix->getWidth() / 2 : m_matrix->getWidth() / 2 - 1;
	m_spawnPositionY = 1;
	m_orientation = Orientation::HORIZONTAL;
}

Tetrimino_O::~Tetrimino_O() {}

bool Tetrimino_O::rotateRight() {
	// returning false will not trigger a draw update
	return false;
}

bool Tetrimino_O::rotateLeft() {
	// returning false will not trigger a draw update
	return false;
}
