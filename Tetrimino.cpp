#include "Tetrimino.h"

Tetrimino::Tetrimino() {}

Tetrimino::~Tetrimino() {}

void Tetrimino::addToMatrix(std::vector<std::string>& matrix) {
	matrix[m_originPosition.y][m_originPosition.x] = m_minoSign;

	for (auto& pos : m_otherMinoPos) {
		matrix[pos.y][pos.x] = m_minoSign;
	}
}

bool Tetrimino::moveLeft(std::vector<std::string>& matrix, char emptyCellSign) {
	if (!canMinoMoveLeft(m_originPosition, matrix, emptyCellSign)) {
		return false;
	}

	for (auto& minoPos : m_otherMinoPos) {
		if (!canMinoMoveLeft(minoPos, matrix, emptyCellSign)) {
			return false;
		}
	}

	glm::ivec2 newOriginPos(m_originPosition.x - 1, m_originPosition.y);

	glm::ivec2 newOtherMinoPos[3] = {
		glm::ivec2(m_otherMinoPos[0].x - 1, m_otherMinoPos[0].y),
		glm::ivec2(m_otherMinoPos[1].x - 1, m_otherMinoPos[1].y),
		glm::ivec2(m_otherMinoPos[2].x - 1, m_otherMinoPos[2].y)
	};

	performTransformation(matrix, newOriginPos, newOtherMinoPos, emptyCellSign, m_minoSign);
	return true;
}

bool Tetrimino::moveRight(std::vector<std::string>& matrix, char emptyCellSign) {
	if (!canMinoMoveRight(m_originPosition, matrix, emptyCellSign)) {
		return false;
	}

	for (auto& minoPos : m_otherMinoPos) {
		if (!canMinoMoveRight(minoPos, matrix, emptyCellSign)) {
			return false;
		}
	}

	glm::ivec2 newOriginPos(m_originPosition.x + 1, m_originPosition.y);

	glm::ivec2 newOtherMinoPos[3] = {
		glm::ivec2(m_otherMinoPos[0].x + 1, m_otherMinoPos[0].y),
		glm::ivec2(m_otherMinoPos[1].x + 1, m_otherMinoPos[1].y),
		glm::ivec2(m_otherMinoPos[2].x + 1, m_otherMinoPos[2].y)
	};

	performTransformation(matrix, newOriginPos, newOtherMinoPos, emptyCellSign, m_minoSign);
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

Tetrimino_T::Tetrimino_T(int originPositionX, int originPositionY, int matrixWidth, int matrixHeight) {
	m_originPosition = glm::vec2(originPositionX, originPositionY);

	m_otherMinoPos.resize(3);
	m_otherMinoPos[0] = glm::ivec2(m_originPosition.x - 1, m_originPosition.y);
	m_otherMinoPos[1] = glm::ivec2(m_originPosition.x, m_originPosition.y - 1);
	m_otherMinoPos[2] = glm::ivec2(m_originPosition.x + 1, m_originPosition.y);

	m_minoSign = 'T';
	m_matrixWidth = matrixWidth;
	m_matrixHeight = matrixHeight;
}

Tetrimino_T::~Tetrimino_T() {}

bool Tetrimino_T::rotateRight(std::vector<std::string>& matrix, char emptyCellSign) {
	glm::ivec2 newMinoPos[3] = {
		rotateMinoRight(m_otherMinoPos[0]),
		rotateMinoRight(m_otherMinoPos[1]),
		rotateMinoRight(m_otherMinoPos[2])
	};

	// if all the cells after the rotation are empty or part of this tetrimino
	if (
		isMinoInsideMatrix(newMinoPos[0]) &&
		(matrix[newMinoPos[0].y][newMinoPos[0].x] == emptyCellSign || isCellPartOfThis(newMinoPos[0])) &&
		isMinoInsideMatrix(newMinoPos[1]) &&
		(matrix[newMinoPos[1].y][newMinoPos[1].x] == emptyCellSign || isCellPartOfThis(newMinoPos[1])) &&
		isMinoInsideMatrix(newMinoPos[2]) &&
		(matrix[newMinoPos[2].y][newMinoPos[2].x] == emptyCellSign || isCellPartOfThis(newMinoPos[2]))
		) {

		performTransformation(matrix, m_originPosition, newMinoPos, emptyCellSign, m_minoSign);
		return true;
	}
	return false;
}

bool Tetrimino_T::rotateLeft(std::vector<std::string>& matrix, char emptyCellSign) {
	glm::ivec2 newMinoPos[3] = {
		rotateMinoLeft(m_otherMinoPos[0]),
		rotateMinoLeft(m_otherMinoPos[1]),
		rotateMinoLeft(m_otherMinoPos[2])
	};

	// if all the cells after the rotation are empty
	if (
		isMinoInsideMatrix(newMinoPos[0]) &&
		(matrix[newMinoPos[0].y][newMinoPos[0].x] == emptyCellSign || isCellPartOfThis(newMinoPos[0])) &&
		isMinoInsideMatrix(newMinoPos[1]) &&
		(matrix[newMinoPos[1].y][newMinoPos[1].x] == emptyCellSign || isCellPartOfThis(newMinoPos[1])) &&
		isMinoInsideMatrix(newMinoPos[2]) &&
		(matrix[newMinoPos[2].y][newMinoPos[2].x] == emptyCellSign || isCellPartOfThis(newMinoPos[2]))
		) {
		performTransformation(matrix, m_originPosition, newMinoPos, emptyCellSign, m_minoSign);
		return true;
		// UPDATE COORDINATES
	}
	return false;
}

Tetrimino_L::Tetrimino_L(int originPositionX, int originPositionY, int matrixWidth, int matrixHeight) {
	m_originPosition = glm::vec2(originPositionX, originPositionY);

	m_otherMinoPos.resize(3);
	m_otherMinoPos[0] = glm::ivec2(m_originPosition.x - 1, m_originPosition.y);
	m_otherMinoPos[1] = glm::ivec2(m_originPosition.x + 1, m_originPosition.y);
	m_otherMinoPos[2] = glm::ivec2(m_originPosition.x + 1, m_originPosition.y - 1);

	m_minoSign = 'L';
	m_matrixWidth = matrixWidth;
	m_matrixHeight = matrixHeight;
}

Tetrimino_L::~Tetrimino_L() {}

bool Tetrimino_L::rotateRight(std::vector<std::string>& matrix, char emptyCellSign) {
	glm::ivec2 newMinoPos[3] = {
		rotateMinoRight(m_otherMinoPos[0]),
		rotateMinoRight(m_otherMinoPos[1]),
		rotateMinoRight(m_otherMinoPos[2])
	};

	// if all the cells after the rotation are empty
	if (
		isMinoInsideMatrix(newMinoPos[0]) &&
		(matrix[newMinoPos[0].y][newMinoPos[0].x] == emptyCellSign || isCellPartOfThis(newMinoPos[0])) &&
		isMinoInsideMatrix(newMinoPos[1]) &&
		(matrix[newMinoPos[1].y][newMinoPos[1].x] == emptyCellSign || isCellPartOfThis(newMinoPos[1])) &&
		isMinoInsideMatrix(newMinoPos[2]) &&
		(matrix[newMinoPos[2].y][newMinoPos[2].x] == emptyCellSign || isCellPartOfThis(newMinoPos[2]))
		) {

		performTransformation(matrix, m_originPosition, newMinoPos, emptyCellSign, m_minoSign);
		return true;
	}
	return false;
}

bool Tetrimino_L::rotateLeft(std::vector<std::string>& matrix, char emptyCellSign) {
	glm::ivec2 newMinoPos[3] = {
		rotateMinoLeft(m_otherMinoPos[0]),
		rotateMinoLeft(m_otherMinoPos[1]),
		rotateMinoLeft(m_otherMinoPos[2])
	};

	// if all the cells after the rotation are empty
	if (
		isMinoInsideMatrix(newMinoPos[0]) && 
		(matrix[newMinoPos[0].y][newMinoPos[0].x] == emptyCellSign || isCellPartOfThis(newMinoPos[0])) &&
		isMinoInsideMatrix(newMinoPos[1]) && 
		(matrix[newMinoPos[1].y][newMinoPos[1].x] == emptyCellSign || isCellPartOfThis(newMinoPos[1])) &&
		isMinoInsideMatrix(newMinoPos[2]) && 
		(matrix[newMinoPos[2].y][newMinoPos[2].x] == emptyCellSign || isCellPartOfThis(newMinoPos[2]))
		) {
		
		performTransformation(matrix, m_originPosition, newMinoPos, emptyCellSign, m_minoSign);
		return true;
	}
	return false;
}

void Tetrimino::performTransformation(std::vector<std::string>& matrix, 
	glm::ivec2 newOriginPos, glm::ivec2 newMinoPos[3],
	char emptyCellSign, char minoSign) {

	matrix[m_originPosition.y][m_originPosition.x] = emptyCellSign;
	matrix[m_otherMinoPos[0].y][m_otherMinoPos[0].x] = emptyCellSign;
	matrix[m_otherMinoPos[1].y][m_otherMinoPos[1].x] = emptyCellSign;
	matrix[m_otherMinoPos[2].y][m_otherMinoPos[2].x] = emptyCellSign;

	m_originPosition = newOriginPos;
	m_otherMinoPos[0] = newMinoPos[0];
	m_otherMinoPos[1] = newMinoPos[1];
	m_otherMinoPos[2] = newMinoPos[2];

	matrix[m_originPosition.y][m_originPosition.x] = minoSign;
	matrix[m_otherMinoPos[0].y][m_otherMinoPos[0].x] = minoSign;
	matrix[m_otherMinoPos[1].y][m_otherMinoPos[1].x] = minoSign;
	matrix[m_otherMinoPos[2].y][m_otherMinoPos[2].x] = minoSign;
}

bool Tetrimino::canMinoMoveLeft(const glm::ivec2& minoPos, std::vector<std::string>& matrix, 
								char emptyCellSign) {

	return minoPos.x > 0 && 
		(matrix[minoPos.y][minoPos.x - 1] == emptyCellSign || 
			isCellPartOfThis(glm::ivec2(minoPos.x - 1, minoPos.y)));
}

bool Tetrimino::canMinoMoveRight(const glm::ivec2& minoPos, std::vector<std::string>& matrix, 
	char emptyCellSign) {
	
	return minoPos.x < m_matrixWidth - 1 &&
		(matrix[minoPos.y][minoPos.x + 1] == emptyCellSign || 
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
	
	return (minoPos.x >= 0 && minoPos.x < m_matrixWidth) && (minoPos.y >= 0 && minoPos.y < m_matrixHeight);
}
