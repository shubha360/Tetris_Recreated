#pragma once

//#include <glm/glm.hpp>
#include <vector>
#include <string>

#include <Evolve/TextureData.h>
#include <Evolve/TextureRenderer.h>
#include <Evolve/ColorRgba.h>
#include <Evolve/RectDimension.h>
#include <Evolve/UVDimension.h>
#include <Evolve/ImageLoader.h>

class Matrix {
public:
	Matrix();
	virtual ~Matrix();

	bool init(const glm::ivec2& topLeftPos, GLuint minoTextureId, const int numRows = 20, 
		const int numColumns = 10, const int minoLength = 32);
	
	int checkLineClears();

	void drawMatrix(Evolve::TextureRenderer& renderer) const;

	void reset();

	std::vector<std::string>& getMatrix() { return m_matrix; }
	const int getColumns() const { return m_numColumns; }
	const int getRows() const { return m_numRows; }
	const int getMinoLength() const { return m_minoLength; }
	const char getEmptyCellSign() const { return EMPTY_CELL_SIGN; }
	const char getGhostCellSign() const { return GHOST_CELL_SIGN; }

	Evolve::RectDimension getDimension() const {
		return Evolve::RectDimension(
			Evolve::Origin::TOP_LEFT,
			m_matrixPosTopLeft.x, m_matrixPosTopLeft.y,
			(unsigned int) m_numColumns * m_minoLength,
			(unsigned int) m_numRows * m_minoLength
		);
	}

protected:
	const Evolve::UvDimension MINO_UV_BLACK		{ 0.0f, 0.0f, 0.1f, 1.0f };
	const Evolve::UvDimension MINO_UV_RED		{ 0.1f, 0.0f, 0.1f, 1.0f };
	const Evolve::UvDimension MINO_UV_GREEN		{ 0.2f, 0.0f, 0.1f, 1.0f };
	const Evolve::UvDimension MINO_UV_BLUE		{ 0.3f, 0.0f, 0.1f, 1.0f };
	const Evolve::UvDimension MINO_UV_YELLOW	{ 0.4f, 0.0f, 0.1f, 1.0f };
	const Evolve::UvDimension MINO_UV_CYAN		{ 0.5f, 0.0f, 0.1f, 1.0f };
	const Evolve::UvDimension MINO_UV_MAGENTA	{ 0.6f, 0.0f, 0.1f, 1.0f };
	const Evolve::UvDimension MINO_UV_ORANGE	{ 0.7f, 0.0f, 0.1f, 1.0f };
	const Evolve::UvDimension MINO_UV_GHOST		{ 0.8f, 0.0f, 0.1f, 1.0f };

	const Evolve::ColorRgba MINO_COLOR { 255, 255, 255, 255 };

	int m_numRows = 20;
	int m_numColumns = 10;
	int m_minoLength = 32;
	
	const char EMPTY_CELL_SIGN = ' ';
	const char GHOST_CELL_SIGN = 'G';
	const std::string EMPTY_LINE = std::string(m_numColumns, EMPTY_CELL_SIGN);

	glm::ivec2 m_matrixPosTopLeft = glm::ivec2(0);

	std::vector<std::string> m_matrix;
	GLuint m_minoTextureId = 0;
};

