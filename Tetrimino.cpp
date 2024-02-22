#include "Tetrimino.h"

Tetrimino::Tetrimino() {}

Tetrimino::~Tetrimino() {}

void Tetrimino::spawn() {
	auto& matrix = m_matrix->getMatrix();

	for (int i = 0; i < 4; i++) {
		matrix[m_minoPositions[i].y][m_minoPositions[i].x] = m_minoSign;
	}
}

bool Tetrimino::moveLeft() {

	glm::ivec2 newMinoPos[4] = {
		glm::ivec2(m_minoPositions[0].x - 1, m_minoPositions[0].y),
		glm::ivec2(m_minoPositions[1].x - 1, m_minoPositions[1].y),
		glm::ivec2(m_minoPositions[2].x - 1, m_minoPositions[2].y),
		glm::ivec2(m_minoPositions[3].x - 1, m_minoPositions[3].y),

	};

	if (!canPerformTransformation(newMinoPos)) {
		return false;
	}
	
	performTransformation(newMinoPos);
	return true;
}

bool Tetrimino::moveRight() {
	glm::ivec2 newMinoPos[4] = {
		glm::ivec2(m_minoPositions[0].x + 1, m_minoPositions[0].y),
		glm::ivec2(m_minoPositions[1].x + 1, m_minoPositions[1].y),
		glm::ivec2(m_minoPositions[2].x + 1, m_minoPositions[2].y),
		glm::ivec2(m_minoPositions[3].x + 1, m_minoPositions[3].y),

	};

	if (!canPerformTransformation(newMinoPos)) {
		return false;
	}

	performTransformation(newMinoPos);
	return true;
}

bool Tetrimino::moveDown() {
	glm::ivec2 newMinoPos[4] = {
		glm::ivec2(m_minoPositions[0].x, m_minoPositions[0].y + 1),
		glm::ivec2(m_minoPositions[1].x, m_minoPositions[1].y + 1),
		glm::ivec2(m_minoPositions[2].x, m_minoPositions[2].y + 1),
		glm::ivec2(m_minoPositions[3].x, m_minoPositions[3].y + 1),

	};

	if (!canPerformTransformation(newMinoPos)) {
		return false;
	}

	performTransformation(newMinoPos);
	return true;
}

void Tetrimino::reset() {
	for (int i = 0; i < 4; i++) {
		m_minoPositions[i] = m_spawnMinoPositions[i];
	}
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

void Tetrimino::performTransformation(const glm::ivec2 newMinoPos[4]) {

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

bool Tetrimino::canPerformTransformation(const glm::ivec2 newMinoPos[4]) const {

	auto& matrix = m_matrix->getMatrix();

	for (int i = 0; i < 4; i++) {
		if ( !isMinoInsideMatrix(newMinoPos[i]) ||
			!( matrix[newMinoPos[i].y][newMinoPos[i].x] == m_matrix->getEmptyCellSign() || 
				isCellPartOfThis(newMinoPos[i]) ) )
		{
			return false;
		}
	}

	return true;
}

bool Tetrimino::isCellPartOfThis(const glm::ivec2& cellPos) const {
	return
		cellPos == m_minoPositions[0] ||
		cellPos == m_minoPositions[1] ||
		cellPos == m_minoPositions[2] ||
		cellPos == m_minoPositions[3];
}

bool Tetrimino::isMinoInsideMatrix(const glm::ivec2& minoPos) const {

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

Tetrimino_T::Tetrimino_T(Matrix* matrix) {
	m_minoSign = 'T';
	m_matrix = matrix;
	m_spawnPositionX = m_matrix->getWidth() % 2 == 0 ? m_matrix->getWidth() / 2 - 1 : m_matrix->getWidth() / 2;
	m_spawnPositionY = 1;
	m_orientation = Orientation::HORIZONTAL;

	m_originMinoIndex = 1;
	m_spawnMinoPositions[m_originMinoIndex] = glm::ivec2(m_spawnPositionX, m_spawnPositionY);

	m_spawnMinoPositions[0] = glm::ivec2(m_spawnPositionX - 1, m_spawnPositionY);
	m_spawnMinoPositions[2] = glm::ivec2(m_spawnPositionX, m_spawnPositionY - 1);
	m_spawnMinoPositions[3] = glm::ivec2(m_spawnPositionX + 1, m_spawnPositionY);
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

	// if all the cells after the rotation are empty or part of this tetrimino
	if (!canPerformTransformation(newMinoPos)) {
		return false;
	}
	performTransformation(newMinoPos);
	changeOrientation();
	return true;
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

	// if all the cells after the rotation are empty or part of this tetrimino
	if (!canPerformTransformation(newMinoPos)) {
		return false;
	}
	performTransformation(newMinoPos);
	changeOrientation();
	return true;
}

Tetrimino_L::Tetrimino_L(Matrix* matrix) {
	m_minoSign = 'L';
	m_matrix = matrix;
	m_spawnPositionX = m_matrix->getWidth() % 2 == 0 ? m_matrix->getWidth() / 2 - 1 : m_matrix->getWidth() / 2;
	m_spawnPositionY = 1;
	m_orientation = Orientation::HORIZONTAL;

	m_originMinoIndex = 1;
	m_spawnMinoPositions[m_originMinoIndex] = glm::ivec2(m_spawnPositionX, m_spawnPositionY);

	m_spawnMinoPositions[0] = glm::ivec2(m_spawnPositionX - 1, m_spawnPositionY);
	m_spawnMinoPositions[2] = glm::ivec2(m_spawnPositionX + 1, m_spawnPositionY);
	m_spawnMinoPositions[3] = glm::ivec2(m_spawnPositionX + 1, m_spawnPositionY - 1);
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

	// if all the cells after the rotation are empty or part of this tetrimino
	if (!canPerformTransformation(newMinoPos)) {
		return false;
	}
	performTransformation(newMinoPos);
	changeOrientation();
	return true;
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

	// if all the cells after the rotation are empty or part of this tetrimino
	if (!canPerformTransformation(newMinoPos)) {
		return false;
	}
	performTransformation(newMinoPos);
	changeOrientation();
	return true;
}

Tetrimino_J::Tetrimino_J(Matrix* matrix) {
	m_minoSign = 'J';
	m_matrix = matrix;
	m_spawnPositionX = m_matrix->getWidth() % 2 == 0 ? m_matrix->getWidth() / 2 - 1 : m_matrix->getWidth() / 2;
	m_spawnPositionY = 1;
	m_orientation = Orientation::HORIZONTAL;

	m_originMinoIndex = 2;
	m_spawnMinoPositions[m_originMinoIndex] = glm::ivec2(m_spawnPositionX, m_spawnPositionY);

	m_spawnMinoPositions[0] = glm::ivec2(m_spawnPositionX - 1, m_spawnPositionY - 1);
	m_spawnMinoPositions[1] = glm::ivec2(m_spawnPositionX - 1, m_spawnPositionY);
	m_spawnMinoPositions[3] = glm::ivec2(m_spawnPositionX + 1, m_spawnPositionY);
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

	// if all the cells after the rotation are empty or part of this tetrimino
	if (!canPerformTransformation(newMinoPos)) {
		return false;
	}
	performTransformation(newMinoPos);
	changeOrientation();
	return true;
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

	// if all the cells after the rotation are empty or part of this tetrimino
	if (!canPerformTransformation(newMinoPos)) {
		return false;
	}
	performTransformation(newMinoPos);
	changeOrientation();
	return true;
}

Tetrimino_O::Tetrimino_O(Matrix* matrix) {
	m_minoSign = 'O';
	m_matrix = matrix;
	m_spawnPositionX = m_matrix->getWidth() % 2 == 0 ? m_matrix->getWidth() / 2 : m_matrix->getWidth() / 2 - 1;
	m_spawnPositionY = 1;
	m_orientation = Orientation::HORIZONTAL;

	m_originMinoIndex = 3;
	m_spawnMinoPositions[m_originMinoIndex] = glm::ivec2(m_spawnPositionX, m_spawnPositionY);

	m_spawnMinoPositions[0] = glm::ivec2(m_spawnPositionX - 1, m_spawnPositionY - 1);
	m_spawnMinoPositions[1] = glm::ivec2(m_spawnPositionX, m_spawnPositionY - 1);
	m_spawnMinoPositions[2] = glm::ivec2(m_spawnPositionX - 1, m_spawnPositionY);
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

Tetrimino_I::Tetrimino_I(Matrix* matrix) {
	m_minoSign = 'I';
	m_matrix = matrix;
	m_spawnPositionX = m_matrix->getWidth() % 2 == 0 ? m_matrix->getWidth() / 2 : m_matrix->getWidth() / 2 - 1;
	m_spawnPositionY = 0;
	m_orientation = Orientation::HORIZONTAL;

	m_originMinoIndex = 2;
	m_spawnMinoPositions[m_originMinoIndex] = glm::ivec2(m_spawnPositionX, m_spawnPositionY);

	m_spawnMinoPositions[0] = glm::ivec2(m_spawnPositionX - 2, m_spawnPositionY);
	m_spawnMinoPositions[1] = glm::ivec2(m_spawnPositionX - 1, m_spawnPositionY);
	m_spawnMinoPositions[3] = glm::ivec2(m_spawnPositionX + 1, m_spawnPositionY);
}

Tetrimino_I::~Tetrimino_I() {}

bool Tetrimino_I::rotateRight() {

	// origin will be 1 for right rotation
	m_originMinoIndex = 1;

	glm::ivec2 newMinoPos[4] = {};

	for (int i = 0; i < 4; i++) {
		if (i == m_originMinoIndex) {
			newMinoPos[i] = m_minoPositions[i];
		}
		else {

			// rotate right if in horizontal orientation
			if (m_orientation == Orientation::HORIZONTAL) {
				newMinoPos[i] = rotateMinoRight(m_minoPositions[i]);
			}

			// rotate left if in vertical orientation
			else if (m_orientation == Orientation::VERTICAL) {
				newMinoPos[i] = rotateMinoLeft(m_minoPositions[i]);
			}
		}
	}	

	// if all the cells after the rotation are empty or part of this tetrimino
	if (!canPerformTransformation(newMinoPos)) {
		return false;
	}
	performTransformation(newMinoPos);
	changeOrientation();
	return true;
}

bool Tetrimino_I::rotateLeft() {
	
	// origin will be 1 for right rotation
	m_originMinoIndex = 2;

	glm::ivec2 newMinoPos[4] = {};

	for (int i = 0; i < 4; i++) {
		if (i == m_originMinoIndex) {
			newMinoPos[i] = m_minoPositions[i];
		}
		else {
			// rotate left if in horizontal orientation
			if (m_orientation == Orientation::HORIZONTAL) {
				newMinoPos[i] = rotateMinoLeft(m_minoPositions[i]);
			}

			// rotate right if in vertical orientation
			else if (m_orientation == Orientation::VERTICAL) {
				newMinoPos[i] = rotateMinoRight(m_minoPositions[i]);
			}
		}
	}

	// if all the cells after the rotation are empty or part of this tetrimino
	if (!canPerformTransformation(newMinoPos)) {
		return false;
	}
	performTransformation(newMinoPos);
	changeOrientation();
	return true;
}

Tetrimino_Z::Tetrimino_Z(Matrix* matrix) {
	m_minoSign = 'Z';
	m_matrix = matrix;
	m_spawnPositionX = m_matrix->getWidth() % 2 == 0 ? m_matrix->getWidth() / 2 - 1 : m_matrix->getWidth() / 2;
	m_spawnPositionY = 0;
	m_orientation = Orientation::HORIZONTAL;

	m_originMinoIndex = 2;
	m_spawnMinoPositions[m_originMinoIndex] = glm::ivec2(m_spawnPositionX, m_spawnPositionY);

	m_spawnMinoPositions[0] = glm::ivec2(m_spawnPositionX - 1, m_spawnPositionY + 1);
	m_spawnMinoPositions[1] = glm::ivec2(m_spawnPositionX, m_spawnPositionY + 1);
	m_spawnMinoPositions[3] = glm::ivec2(m_spawnPositionX + 1, m_spawnPositionY);
}

Tetrimino_Z::~Tetrimino_Z() {}

bool Tetrimino_Z::rotateRight() {
	glm::ivec2 newMinoPos[4] = {};

	for (int i = 0; i < 4; i++) {
		if (i == m_originMinoIndex) {
			newMinoPos[i] = m_minoPositions[i];
		}
		else {
			newMinoPos[i] = rotateMinoRight(m_minoPositions[i]);
		}
	}

	// if all the cells after the rotation are empty or part of this tetrimino
	if (!canPerformTransformation(newMinoPos)) {
		return false;
	}
	performTransformation(newMinoPos);
	changeOrientation();
	return true;
}

bool Tetrimino_Z::rotateLeft() {
	glm::ivec2 newMinoPos[4] = {};

	for (int i = 0; i < 4; i++) {
		if (i == m_originMinoIndex) {
			newMinoPos[i] = m_minoPositions[i];
		}
		else {
			newMinoPos[i] = rotateMinoLeft(m_minoPositions[i]);
		}
	}

	// if all the cells after the rotation are empty or part of this tetrimino
	if (!canPerformTransformation(newMinoPos)) {
		return false;
	}
	performTransformation(newMinoPos);
	changeOrientation();
	return true;
}

Tetrimino_S::Tetrimino_S(Matrix* matrix) {
	m_minoSign = 'S';
	m_matrix = matrix;
	m_spawnPositionX = m_matrix->getWidth() % 2 == 0 ? m_matrix->getWidth() / 2 - 1 : m_matrix->getWidth() / 2;
	m_spawnPositionY = 0;
	m_orientation = Orientation::HORIZONTAL;

	m_originMinoIndex = 1;
	m_spawnMinoPositions[m_originMinoIndex] = glm::ivec2(m_spawnPositionX, m_spawnPositionY);

	m_spawnMinoPositions[0] = glm::ivec2(m_spawnPositionX - 1, m_spawnPositionY);
	m_spawnMinoPositions[2] = glm::ivec2(m_spawnPositionX, m_spawnPositionY + 1);
	m_spawnMinoPositions[3] = glm::ivec2(m_spawnPositionX + 1, m_spawnPositionY + 1);
}

Tetrimino_S::~Tetrimino_S() {}

bool Tetrimino_S::rotateRight() {
	glm::ivec2 newMinoPos[4] = {};

	for (int i = 0; i < 4; i++) {
		if (i == m_originMinoIndex) {
			newMinoPos[i] = m_minoPositions[i];
		}
		else {
			newMinoPos[i] = rotateMinoRight(m_minoPositions[i]);
		}
	}

	// if all the cells after the rotation are empty or part of this tetrimino
	if (!canPerformTransformation(newMinoPos)) {
		return false;
	}
	performTransformation(newMinoPos);
	changeOrientation();
	return true;
}

bool Tetrimino_S::rotateLeft() {
	glm::ivec2 newMinoPos[4] = {};

	for (int i = 0; i < 4; i++) {
		if (i == m_originMinoIndex) {
			newMinoPos[i] = m_minoPositions[i];
		}
		else {
			newMinoPos[i] = rotateMinoLeft(m_minoPositions[i]);
		}
	}

	// if all the cells after the rotation are empty or part of this tetrimino
	if (!canPerformTransformation(newMinoPos)) {
		return false;
	}
	performTransformation(newMinoPos);
	changeOrientation();
	return true;
}