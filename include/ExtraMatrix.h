#pragma once

#include "Matrix.h"

#include "Tetrimino.h"

class ExtraMatrix : public Matrix {
public:
	ExtraMatrix();
	~ExtraMatrix();

	void init(std::vector<Tetrimino*> tetriminoes, const glm::ivec2& topLeftPos, 
		GLuint minoTextureId, const int minoLength = 16);

	Tetrimino* pushAndPop(Tetrimino* newTetrimino);

private:
	std::vector<Tetrimino*> m_tetriminoes;

	void addTetrimino(int index, Tetrimino* tetrimino);
	void emptyMatrix();

	int m_tetriminoMaxHeight = 5;
};

