#pragma once

#include "Font.h"

#include "Matrix.h"

#include "Tetrimino.h"

class ExtraMatrix : public Matrix {
public:
	ExtraMatrix();
	~ExtraMatrix();

	void init(std::vector<Tetrimino*> tetriminoes, const std::string& name, Evolve::Font& font,
		const float nameScale, const Evolve::ColorRgba nameColor,
		const glm::ivec2& topLeftPos, GLuint minoTextureId, const int minoLength = 16);

	Tetrimino* pushAndPop(Tetrimino* newTetrimino);

	void drawName(Evolve::TextureRenderer& textureRenderer);

private:
	std::vector<Tetrimino*> m_tetriminoes;

	std::string m_name = "";
	Evolve::Font m_font;
	float m_nameScale = 1.0f;
	Evolve::ColorRgba m_nameColor {};

	glm::ivec2 m_namePos {};
	glm::ivec2 m_matrixPos {};

	void addTetrimino(int index, Tetrimino* tetrimino);
	void emptyMatrix();

	int m_tetriminoMaxHeight = 5;
};

