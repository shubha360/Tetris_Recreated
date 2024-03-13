#include "../include/Matrix.h"

Matrix::Matrix() {}

Matrix::~Matrix() {}

bool Matrix::init(const glm::ivec2& topLeftPos, GLuint minoTextureId, const int numRows /*= 20*/,
	const int numColumns /*= 10*/, const int minoLength /*= 32*/) {

	m_matrixPosTopLeft = topLeftPos;
	m_minoTextureId = minoTextureId;
	m_numRows = numRows;
	m_numColumns = numColumns;
	m_minoLength = minoLength;
	
	m_matrix.resize(m_numRows);

	for (int i = 0; i < m_matrix.size(); i++) {
		m_matrix[i] = EMPTY_LINE;
	}
	
	/*m_matrix[16] = "OOOOOOOOO ";
	m_matrix[17] = "O         ";
	m_matrix[18] = "O         ";
	m_matrix[19] = "OOOOOOOOO ";*/

	return true;
}

int Matrix::checkLineClears() {
	int lineClears = 0;

	for (int i = m_numRows - 1; i >= 0; i--) {

		// line clear
		if (m_matrix[i].find(' ') == std::string::npos) {
			lineClears++;
			m_matrix[i] = EMPTY_LINE;
			i++;
		}

		// found empty line
		else if (m_matrix[i].compare(EMPTY_LINE) == 0) {
			int lineToPull = i - lineClears;

			if (lineToPull >= 0) {

				// if line to pull down is also empty
				if (m_matrix[lineToPull].compare(EMPTY_LINE) == 0) {
					break;
				}
				else { // pull the line down
					m_matrix[i] = m_matrix[lineToPull];
					m_matrix[lineToPull] = EMPTY_LINE;
					i++;
				}
			}
			else { // line to pull is out of bound
				break;
			}
		}
	}

	return lineClears;
}

void Matrix::drawMatrix(Evolve::TextureRenderer& renderer) const {

	static Evolve::UvDimension minoUV = {};

	for (int row = 0; row < m_numRows; row++) {
		for (int column = 0; column < m_numColumns; column++) {
			
			// determine color
			switch (m_matrix[row][column]) {
			
			case ' ':
				minoUV = MINO_UV_BLACK;
				break;

			case 'T':
				minoUV = MINO_UV_MAGENTA;
				break;

			case 'L':
				minoUV = MINO_UV_ORANGE;
				break;

			case 'J':
				minoUV = MINO_UV_BLUE;
				break;

			case 'O':
				minoUV = MINO_UV_YELLOW;
				break;

			case 'I':
				minoUV = MINO_UV_CYAN;
				break;

			case 'Z':
				minoUV = MINO_UV_RED;
				break;

			case 'S':
				minoUV = MINO_UV_GREEN;
				break;

			case 'G':
				minoUV = MINO_UV_GHOST;
				break;
			}

			Evolve::RectDimension minoDimension(
				Evolve::Origin::TOP_LEFT, 
				m_matrixPosTopLeft.x + column * m_minoLength,
				m_matrixPosTopLeft.y - row * m_minoLength,
				m_minoLength, m_minoLength
			);


			renderer.draw(
				minoDimension,
				minoUV,
				m_minoTextureId,
				MINO_COLOR
			);
		}
	}
}

void Matrix::reset() {
	for (auto& row : m_matrix) {
		for (auto& c : row) {
			c = ' ';
		}
	}
}
