#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>

class Matrix;

class Tetrimino {
public:
	friend class Matrix;

	Tetrimino();
	virtual ~Tetrimino();

	void addToMatrix(std::vector<std::string>& matrix);

	virtual void rotateRight(std::vector<std::string>& matrix, char emptyCellSign) = 0;
	virtual void rotateLeft(std::vector<std::string>& matrix, char emptyCellSign) = 0;

	void moveLeft();
	void moveRight();

protected:
	glm::ivec2 m_originPosition = glm::ivec2(0);
	std::vector<glm::ivec2> m_otherMinoPos;

	// these will change upon rotating and moving
	int m_topY = 0, m_bottomY = 0;
	int m_leftX = 0, m_rightX = 0;

	char m_minoChar = ' ';

	// updates the left, right, top, bottom coordinates
	void updateCoordinates(int left, int right, int top, int bottom);

	glm::ivec2 rotateMinoRight(const glm::ivec2& minoPostion) const;
	glm::ivec2 rotateMinoLeft(const glm::ivec2& minoPostion) const;

	void performRotation(std::vector<std::string>& matrix, glm::ivec2 newMinoPos[3], 
		char emptyCellSign, char minoSign);
};

class Tetrimino_T : public Tetrimino {
public:
	Tetrimino_T(int originPositionX, int originPositionY);
	~Tetrimino_T();

	void rotateRight(std::vector<std::string>& matrix, char emptyCellSign);
	void rotateLeft(std::vector<std::string>& matrix, char emptyCellSign);
};

class Tetrimino_L : public Tetrimino {
public:
	Tetrimino_L(int originPositionX, int originPositionY);
	~Tetrimino_L();

	void rotateRight(std::vector<std::string>& matrix, char emptyCellSign);
	void rotateLeft(std::vector<std::string>& matrix, char emptyCellSign);
};