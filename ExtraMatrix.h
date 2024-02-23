#pragma once

#include "Matrix.h"

#include "Tetrimino.h"

class ExtraMatrix : public Matrix {
public:
	ExtraMatrix();
	~ExtraMatrix();

	void initTetriminoes(std::vector<Tetrimino*> tetriminoes);

	Tetrimino* pushAndPop(Tetrimino* newTetrimino);

private:
	std::vector<Tetrimino*> m_tetriminoes;

	void addTetrimino(int index, Tetrimino* tetrimino);
	void emptyMatrix();

	int m_tetriminoMaxHeight = 5;
};

