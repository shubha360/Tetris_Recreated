#include "Tetrimino.h"

Tetrimino::Tetrimino() {}

Tetrimino::~Tetrimino() {}

void Tetrimino::addToMatrix(std::vector<std::string>& matrix) {
	matrix[m_originPosition.y][m_originPosition.x] = m_minoChar;

	for (auto& pos : m_otherMinoPos) {
		matrix[pos.y][pos.x] = m_minoChar;
	}
}

void Tetrimino::moveLeft()
{
}

void Tetrimino::moveRight()
{
}

void Tetrimino::updateCoordinates(int left, int right, int top, int bottom) {
	m_leftX = m_originPosition.x + left;
	m_rightX = m_originPosition.x + right;
	m_topY = m_originPosition.y + top;
	m_bottomY = m_originPosition.y + bottom;
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

Tetrimino_T::Tetrimino_T(int originPositionX, int originPositionY) {
	m_originPosition = glm::vec2(originPositionX, originPositionY);

	m_otherMinoPos.resize(3);
	m_otherMinoPos[0] = glm::ivec2(m_originPosition.x - 1, m_originPosition.y);
	m_otherMinoPos[1] = glm::ivec2(m_originPosition.x, m_originPosition.y - 1);
	m_otherMinoPos[2] = glm::ivec2(m_originPosition.x + 1, m_originPosition.y);

	updateCoordinates(-1, 1, -1, 0);

	m_minoChar = 'T';
}

Tetrimino_T::~Tetrimino_T() {}

void Tetrimino_T::rotateRight(std::vector<std::string>& matrix, char emptyCellSign) {
	glm::ivec2 newMinoPos[3] = {
		rotateMinoRight(m_otherMinoPos[0]),
		rotateMinoRight(m_otherMinoPos[1]),
		rotateMinoRight(m_otherMinoPos[2])
	};

	// if all the cells after the rotation are empty
	if (
		matrix[newMinoPos[0].y][newMinoPos[0].x] == emptyCellSign &&
		matrix[newMinoPos[1].y][newMinoPos[1].x] == emptyCellSign &&
		matrix[newMinoPos[2].y][newMinoPos[2].x] == emptyCellSign
		) {

		performRotation(matrix, newMinoPos, emptyCellSign, m_minoChar);

		// UPDATE COORDINATES
	}
}

void Tetrimino_T::rotateLeft(std::vector<std::string>& matrix, char emptyCellSign) {
	glm::ivec2 newMinoPos[3] = {
		rotateMinoLeft(m_otherMinoPos[0]),
		rotateMinoLeft(m_otherMinoPos[1]),
		rotateMinoLeft(m_otherMinoPos[2])
	};

	// if all the cells after the rotation are empty
	if (
		matrix[newMinoPos[0].y][newMinoPos[0].x] == emptyCellSign &&
		matrix[newMinoPos[1].y][newMinoPos[1].x] == emptyCellSign &&
		matrix[newMinoPos[2].y][newMinoPos[2].x] == emptyCellSign
		) {
		performRotation(matrix, newMinoPos, emptyCellSign, m_minoChar);

		// UPDATE COORDINATES
	}
}

Tetrimino_L::Tetrimino_L(int originPositionX, int originPositionY) {
	m_originPosition = glm::vec2(originPositionX, originPositionY);

	m_otherMinoPos.resize(3);
	m_otherMinoPos[0] = glm::ivec2(m_originPosition.x - 1, m_originPosition.y);
	m_otherMinoPos[1] = glm::ivec2(m_originPosition.x + 1, m_originPosition.y);
	m_otherMinoPos[2] = glm::ivec2(m_originPosition.x + 1, m_originPosition.y - 1);

	updateCoordinates(-1, 1, -1, 0);

	m_minoChar = 'L';
}

Tetrimino_L::~Tetrimino_L() {}

void Tetrimino_L::rotateRight(std::vector<std::string>& matrix, char emptyCellSign) {
	glm::ivec2 newMinoPos[3] = {
		rotateMinoRight(m_otherMinoPos[0]),
		rotateMinoRight(m_otherMinoPos[1]),
		rotateMinoRight(m_otherMinoPos[2])
	};

	// if all the cells after the rotation are empty
	if (
		matrix[newMinoPos[0].y][newMinoPos[0].x] == emptyCellSign &&
		matrix[newMinoPos[1].y][newMinoPos[1].x] == emptyCellSign &&
		matrix[newMinoPos[2].y][newMinoPos[2].x] == emptyCellSign
		) {

		performRotation(matrix, newMinoPos, emptyCellSign, m_minoChar);
		updateCoordinates(0, 1, -1, 1);
	}
}

void Tetrimino_L::rotateLeft(std::vector<std::string>& matrix, char emptyCellSign) {
	glm::ivec2 newMinoPos[3] = {
		rotateMinoLeft(m_otherMinoPos[0]),
		rotateMinoLeft(m_otherMinoPos[1]),
		rotateMinoLeft(m_otherMinoPos[2])
	};

	// if all the cells after the rotation are empty
	if (
		matrix[newMinoPos[0].y][newMinoPos[0].x] == emptyCellSign &&
		matrix[newMinoPos[1].y][newMinoPos[1].x] == emptyCellSign &&
		matrix[newMinoPos[2].y][newMinoPos[2].x] == emptyCellSign
		) {
		
		performRotation(matrix, newMinoPos, emptyCellSign, m_minoChar);

		// UPDATE COORDINATES
	}
}

void Tetrimino::performRotation(std::vector<std::string>& matrix, glm::ivec2 newMinoPos[3], 
	char emptyCellSign, char minoSign) {

	matrix[m_otherMinoPos[0].y][m_otherMinoPos[0].x] = emptyCellSign;
	matrix[m_otherMinoPos[1].y][m_otherMinoPos[1].x] = emptyCellSign;
	matrix[m_otherMinoPos[2].y][m_otherMinoPos[2].x] = emptyCellSign;

	m_otherMinoPos[0] = newMinoPos[0];
	m_otherMinoPos[1] = newMinoPos[1];
	m_otherMinoPos[2] = newMinoPos[2];

	matrix[m_otherMinoPos[0].y][m_otherMinoPos[0].x] = minoSign;
	matrix[m_otherMinoPos[1].y][m_otherMinoPos[1].x] = minoSign;
	matrix[m_otherMinoPos[2].y][m_otherMinoPos[2].x] = minoSign;
}