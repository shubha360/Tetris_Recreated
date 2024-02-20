#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>

class Tetrimino {
public:
	Tetrimino();
	virtual ~Tetrimino();

	void addToMatrix(std::vector<std::string>& matrix);

	virtual bool rotateRight(std::vector<std::string>& matrix, char emptyCellSign) = 0;
	virtual bool rotateLeft(std::vector<std::string>& matrix, char emptyCellSign) = 0;

	bool moveLeft(std::vector<std::string>& matrix, char emptyCellSign);
	bool moveRight(std::vector<std::string>& matrix, char emptyCellSign);

protected:
	glm::ivec2 m_originPosition = glm::ivec2(0);
	std::vector<glm::ivec2> m_otherMinoPos;

	char m_minoSign = ' ';

	int m_matrixWidth = 0, m_matrixHeight = 0;

	glm::ivec2 rotateMinoRight(const glm::ivec2& minoPostion) const;
	glm::ivec2 rotateMinoLeft(const glm::ivec2& minoPostion) const;

	void performTransformation(std::vector<std::string>& matrix, 
		glm::ivec2 newOriginPos, glm::ivec2 newMinoPos[3],
		char emptyCellSign, char minoSign);

	bool canMinoMoveLeft(const glm::ivec2& minoPos, std::vector<std::string>& matrix, char emptyCellSign);
	bool canMinoMoveRight(const glm::ivec2& minoPos, std::vector<std::string>& matrix, char emptyCellSign);

	bool isCellPartOfThis(const glm::ivec2& cellPos);
	bool isMinoInsideMatrix(const glm::ivec2& minoPos);
};

class Tetrimino_T : public Tetrimino {
public:
	Tetrimino_T(int originPositionX, int originPositionY, int matrixWidth, int matrixHeight);
	~Tetrimino_T();

	bool rotateRight(std::vector<std::string>& matrix, char emptyCellSign);
	bool rotateLeft(std::vector<std::string>& matrix, char emptyCellSign);
};

class Tetrimino_L : public Tetrimino {
public:
	Tetrimino_L(int originPositionX, int originPositionY, int matrixWidth, int matrixHeight);
	~Tetrimino_L();

	bool rotateRight(std::vector<std::string>& matrix, char emptyCellSign);
	bool rotateLeft(std::vector<std::string>& matrix, char emptyCellSign);
};