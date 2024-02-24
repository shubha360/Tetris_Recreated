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
	virtual ~Matrix();

	bool init(const glm::ivec2& topLeftPos, GLuint minoTextureId, const int numRows = 20, 
		const int numColumns = 10, const int minoLength = 32);
	
	int checkLineClears();

	void drawMatrix(TextureRenderer& renderer) const;

	std::vector<std::string>& getMatrix() { return m_matrix; }
	const int getColumns() const { return m_numColumns; }
	const int getRows() const { return m_numRows; }
	const int getMinoLength() const { return m_minoLength; }
	const char getEmptyCellSign() const { return EMPTY_CELL_SIGN; }
	const char getGhostCellSign() const { return GHOST_CELL_SIGN; }

protected:
	const ColorRGBA MINO_COLOR_BLACK {0, 0, 0, 255};
	const ColorRGBA MINO_COLOR_RED { 255, 0, 0, 255 };
	const ColorRGBA MINO_COLOR_GREEN { 0, 255, 0, 255 };
	const ColorRGBA MINO_COLOR_BLUE { 0, 0, 255, 255 };
	const ColorRGBA MINO_COLOR_YELLOW { 255, 255, 0, 255 };
	const ColorRGBA MINO_COLOR_CYAN { 0, 255, 255, 255 };
	const ColorRGBA MINO_COLOR_MAGENTA { 255, 0, 255, 255 };
	const ColorRGBA MINO_COLOR_ORANGE { 255, 165, 0, 255 };
	const ColorRGBA MINO_COLOR_GHOST { 0, 0, 0, 100 };

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

