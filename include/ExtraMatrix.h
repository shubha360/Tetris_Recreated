#pragma once

#include <Evolve/Font.h>

#include "Matrix.h"

#include "Tetrimino.h"

class ExtraMatrix : public Matrix {
public:
	ExtraMatrix();
	~ExtraMatrix();

	void init(std::vector<Tetrimino*> tetriminoes, const char* name, Evolve::Font& font,
		const float nameScale, const Evolve::ColorRgba nameColor,
		const Evolve::Position2D& topLeftPos, GLuint minoTextureId, const int minoLength = 16);

	Tetrimino* pushAndPop(Tetrimino* newTetrimino);

	void reset(std::vector<Tetrimino*> tetriminoes);

	void drawName(Evolve::TextureRenderer& textureRenderer);

	int getTetriminoMaxHeight() const { return m_tetriminoMaxHeight; }

private:
	std::vector<Tetrimino*> m_tetriminoes;

	const char* m_name = nullptr;
	Evolve::Font m_font;
	float m_nameScale = 1.0f;
	Evolve::ColorRgba m_nameColor {};

	Evolve::Position2D m_namePos {};
	Evolve::Position2D m_matrixPos {};

	void addTetriminoes(std::vector<Tetrimino*> tetriminoes);
	void emptyMatrix();

	int m_tetriminoMaxHeight = 5;
};

