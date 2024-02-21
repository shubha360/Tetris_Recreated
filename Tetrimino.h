#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "Matrix.h"

class Tetrimino {
public:
	Tetrimino();
	virtual ~Tetrimino();

	void addToMatrix();

	virtual bool rotateRight() = 0;
	virtual bool rotateLeft() = 0;

	bool moveLeft();
	bool moveRight();
	bool moveDown();

protected:
	glm::ivec2 m_originPosition = glm::ivec2(0);
	std::vector<glm::ivec2> m_otherMinoPos;

	char m_minoSign = ' ';
	int m_spawnPositionX = 0, m_spawnPositionY = 0;

	Matrix* m_matrix = nullptr;

	glm::ivec2 rotateMinoRight(const glm::ivec2& minoPostion) const;
	glm::ivec2 rotateMinoLeft(const glm::ivec2& minoPostion) const;

	void performTransformation(glm::ivec2 newOriginPos, glm::ivec2 newMinoPos[3]);

	bool canMinoMoveLeft(const glm::ivec2& minoPos);
	bool canMinoMoveRight(const glm::ivec2& minoPos);
	bool canMinoMoveDown(const glm::ivec2& minoPos);

	bool isCellPartOfThis(const glm::ivec2& cellPos);
	bool isMinoInsideMatrix(const glm::ivec2& minoPos);
};

class Tetrimino_T : public Tetrimino {
public:
	Tetrimino_T(int originPositionX, int originPositionY, Matrix* matrix);
	~Tetrimino_T();

	bool rotateRight();
	bool rotateLeft();
};

class Tetrimino_L : public Tetrimino {
public:
	Tetrimino_L(int originPositionX, int originPositionY, Matrix* matrix);
	~Tetrimino_L();

	bool rotateRight();
	bool rotateLeft();
};

class Tetrimino_J : public Tetrimino {
public:
	Tetrimino_J(int originPositionX, int originPositionY, Matrix* matrix);
	~Tetrimino_J();

	bool rotateRight();
	bool rotateLeft();
};

class Tetrimino_O : public Tetrimino {
public:
	Tetrimino_O(int originPositionX, int originPositionY, Matrix* matrix);
	~Tetrimino_O();

	bool rotateRight();
	bool rotateLeft();
};