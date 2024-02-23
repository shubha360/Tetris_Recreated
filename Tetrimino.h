#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "Matrix.h"

class ExtraMatrix;

enum class Orientation {
	HORIZONTAL,
	VERTICAL
};

class Tetrimino {
public:
	friend class ExtraMatrix;

	Tetrimino();
	virtual ~Tetrimino();

	bool spawn();

	virtual bool rotateRight() = 0;
	virtual bool rotateLeft() = 0;

	bool moveLeft();
	bool moveRight();
	bool moveDown();

	void reset();

protected:
	glm::ivec2 m_spawnMinoPositions[4] = {};
	glm::ivec2 m_minoPositions[4] = {};
	glm::ivec2 m_ghostMinoPositions[4] = {};
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

	void updateGhost();
};

class Tetrimino_T : public Tetrimino {
public:
	Tetrimino_T(Matrix* matrix);
	~Tetrimino_T();

	bool rotateRight() override;
	bool rotateLeft() override;
};

class Tetrimino_L : public Tetrimino {
public:
	Tetrimino_L(Matrix* matrix);
	~Tetrimino_L();

	bool rotateRight() override;
	bool rotateLeft() override;
};

class Tetrimino_J : public Tetrimino {
public:
	Tetrimino_J(Matrix* matrix);
	~Tetrimino_J();

	bool rotateRight() override;
	bool rotateLeft() override;
};

class Tetrimino_O : public Tetrimino {
public:
	Tetrimino_O(Matrix* matrix);
	~Tetrimino_O();

	bool rotateRight() override;
	bool rotateLeft() override;
};

class Tetrimino_I : public Tetrimino {
public:
	Tetrimino_I(Matrix* matrix);
	~Tetrimino_I();

	bool rotateRight() override;
	bool rotateLeft() override;
};

class Tetrimino_Z : public Tetrimino {
public:
	Tetrimino_Z(Matrix* matrix);
	~Tetrimino_Z();

	bool rotateRight() override;
	bool rotateLeft() override;
};

class Tetrimino_S : public Tetrimino {
public:
	Tetrimino_S(Matrix* matrix);
	~Tetrimino_S();

	bool rotateRight() override;
	bool rotateLeft() override;
};