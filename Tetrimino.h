#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "Matrix.h"

enum class Orientation {
	HORIZONTAL,
	VERTICAL
};

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
	glm::ivec2 m_minoPositions[4] = {};
	int m_originMinoIndex = 0;

	char m_minoSign = ' ';
	int m_spawnPositionX = 0, m_spawnPositionY = 0;
	Orientation m_orientation = Orientation::HORIZONTAL;

	Matrix* m_matrix = nullptr;

	glm::ivec2 rotateMinoRight(const glm::ivec2& minoPostion) const;
	glm::ivec2 rotateMinoLeft(const glm::ivec2& minoPostion) const;	

	bool canPerformTransformation(const glm::ivec2 newMinoPos[4]) const;

	bool isCellPartOfThis(const glm::ivec2& cellPos) const;
	bool isMinoInsideMatrix(const glm::ivec2& minoPos) const;

	void performTransformation(const glm::ivec2 newMinoPos[4]);
	void changeOrientation();
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

class Tetrimino_I : public Tetrimino {
public:
	Tetrimino_I(int originPositionX, int originPositionY, Matrix* matrix);
	~Tetrimino_I();

	bool rotateRight();
	bool rotateLeft();
};

class Tetrimino_Z : public Tetrimino {
public:
	Tetrimino_Z(int originPositionX, int originPositionY, Matrix* matrix);
	~Tetrimino_Z();

	bool rotateRight();
	bool rotateLeft();
};

class Tetrimino_S : public Tetrimino {
public:
	Tetrimino_S(int originPositionX, int originPositionY, Matrix* matrix);
	~Tetrimino_S();

	bool rotateRight();
	bool rotateLeft();
};