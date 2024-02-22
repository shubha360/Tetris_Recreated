#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "TextureData.h"
#include "TextureRenderer.h"
#include "ColorRGBA.h"
#include "RectDimension.h"
#include "UVDimension.h"
#include "ImageLoader.h"

class Matrix {
public:
	Matrix();
	~Matrix();

	bool init(unsigned int screenWidth, unsigned int screenHeight);
	
	void checkLineClears();

	void drawMatrix(TextureRenderer& renderer) const;

	std::vector<std::string>& getMatrix() { return m_matrix; }
	const int getWidth() const { return NUM_COLUMNS; }
	const int getHeight() const { return NUM_ROWS; }
	const char getEmptyCellSign() const { return EMPTY_CELL_SIGN; }

private:
	const ColorRGBA MINO_COLOR_BLACK {0, 0, 0, 255};
	const ColorRGBA MINO_COLOR_RED { 255, 0, 0, 255 };
	const ColorRGBA MINO_COLOR_GREEN { 0, 255, 0, 255 };
	const ColorRGBA MINO_COLOR_BLUE { 0, 0, 255, 255 };
	const ColorRGBA MINO_COLOR_YELLOW { 255, 255, 0, 255 };
	const ColorRGBA MINO_COLOR_CYAN { 0, 255, 255, 255 };
	const ColorRGBA MINO_COLOR_MAGENTA { 255, 0, 255, 255 };
	const ColorRGBA MINO_COLOR_ORANGE { 255, 165, 0, 255 };

	const int NUM_ROWS = 20;
	const int NUM_COLUMNS = 10;
	const int MINO_LENGTH = 32;
	const char EMPTY_CELL_SIGN = ' ';
	const std::string EMPTY_LINE = std::string(NUM_COLUMNS, EMPTY_CELL_SIGN);

	glm::ivec2 m_matrixPosTopLeft = glm::ivec2(0);
	glm::ivec2 m_screenDimension = glm::ivec2(0);

	std::vector<std::string> m_matrix;
	TextureData m_minoTexture;
};

