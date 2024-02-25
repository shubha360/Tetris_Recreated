#include "Font.h"

Font::Font() { }

Font::~Font() {
	deleteFont();
}

bool Font::initFromBitmap16x16(const std::string& fontName, const std::string& bmpFilePath, 
	const float fontScale /*= 1.0f*/, const int letterSpacing /*= 5*/, 
	const int lineSpacing /*= 5*/, const int addToSpaceLength /*= 0*/) {

	ImageLoader::LoadTextureFromImage(bmpFilePath, m_fontTexture, 1);

	if (m_fontTexture.data == nullptr) {
		REPORT_ERROR("Failed to load image at " + bmpFilePath + " for bitmap font.", initFromBitmap16x16);
		return false;
	}

	const unsigned int CELL_WIDTH = m_fontTexture.width / 16;
	const unsigned int CELL_HEIGHT = m_fontTexture.height / 16;

	unsigned int top = CELL_HEIGHT;
	unsigned int bottom = 0;
	unsigned int aBottom = 0;

	unsigned int currentCellX = 0, currentCellY = 0;
	unsigned int currentPixelX = 0, currentPixelY = 0;

	UVDimension currentUV = {};

	unsigned int currentChar = 0;
	m_uvDimensions.resize(256);
	m_characterWidths.resize(256);

	for (unsigned int row = 0; row < 16; row++) {
		for (unsigned int column = 0; column < 16; column++) {

			currentCellX = CELL_WIDTH * column;
			currentCellY = CELL_HEIGHT * row;

			currentUV.set(
				(float) CELL_WIDTH * column / (float) m_fontTexture.width,

				// if texture height is 10 and cell height is 2
				// for row = 0, (10 - ((0 + 1) * 2)) = 10 - 2 = 8
				// for row = 1, (10 - ((1 + 1) * 2)) = 10 - 4 = 6
				// for row = 4, (10 - ((4 + 1) * 2)) = 10 - 10 = 10
				(float)(m_fontTexture.height - ((row + 1) * CELL_HEIGHT)) / (float)m_fontTexture.height,
				
				
				(float) CELL_WIDTH / (float) m_fontTexture.width,
				(float) CELL_HEIGHT / (float) m_fontTexture.height
			);

			// Calculate the top
			for (int i = 0; i < CELL_HEIGHT; i++) {
				for (int j = 0; j < CELL_WIDTH; j++) {
					
					currentPixelX = currentCellX + j;
					currentPixelY = currentCellY + i;
					
					unsigned char currentPixel = m_fontTexture.data[currentPixelY * m_fontTexture.width + currentPixelX];

					if (currentPixel != 0) {
						if (i < top) {
							top = i;
						}

						j = CELL_WIDTH;
						i = CELL_WIDTH;
					}
				}
			}

			// Calculate the bottom
			for (int i = CELL_HEIGHT - 1; i >= 0; i--) {
				for (int j = 0; j < CELL_WIDTH; j++) {

					currentPixelX = currentCellX + j;
					currentPixelY = currentCellY + i;

					unsigned char currentPixel = m_fontTexture.data[currentPixelY * m_fontTexture.width + currentPixelX];

					if (currentPixel != 0) {
						if (currentChar == 'A') {
							aBottom = i;
						}

						if (i > bottom) {
							bottom = i;
						}

						j = CELL_WIDTH;
						i = -1;
					}
				}
			}

			// Calculate the left
			for (int j = 0; j < CELL_WIDTH; j++) {
				for (int i = 0; i < CELL_HEIGHT; i++) {
					currentPixelX = currentCellX + j;
					currentPixelY = currentCellY + i;

					unsigned char currentPixel = m_fontTexture.data[currentPixelY * m_fontTexture.width + currentPixelX];

					if (currentPixel != 0) {
						currentUV.bottomLeftX = (float) currentPixelX / (float) m_fontTexture.width;

						// the left of current character
						m_characterWidths[currentChar] = j;

						i = CELL_HEIGHT;
						j = CELL_WIDTH;
					}
				}
			}


			// Calculate the right
			for (int j = CELL_WIDTH - 1; j >= 0; j--) {
				for (int i = 0; i < CELL_HEIGHT; i++) {
					currentPixelX = currentCellX + j;
					currentPixelY = currentCellY + i;

					unsigned char currentPixel = m_fontTexture.data[currentPixelY * m_fontTexture.width + currentPixelX];

					if (currentPixel != 0) {
						currentUV.width = 
							( ( (float) currentPixelX + 1) / (float) m_fontTexture.width ) - currentUV.bottomLeftX;

						// now setting the actual width
						m_characterWidths[currentChar] = j - m_characterWidths[currentChar];

						i = CELL_HEIGHT;
						j = -1;
					}
				}
			}

			m_uvDimensions[currentChar] = currentUV;
			currentChar++;
		}
	}

	ImageLoader::BufferTextureData(m_fontTexture);
	ImageLoader::FreeTexture(m_fontTexture);

	m_spaceSize = CELL_WIDTH / 2;
	m_newLine = aBottom - top;
	m_lineHeight = bottom - top;

	for (int i = 0; i < 256; i++) {
		m_uvDimensions[i].height = (float)m_lineHeight / (float)m_fontTexture.height;
	}

	m_fontName = fontName;
	m_initialized = true;

	m_fontScale = fontScale;
	m_letterSpacing = letterSpacing;
	m_lineSpacing = lineSpacing;
	m_addToSpaceLength = addToSpaceLength;

	return true;
}

bool Font::initFromFontFile(const std::string& fontName, const std::string& fontFilePath, 
	const unsigned int fontSize /*= 32*/,
	const float fontScale /*= 1.0f*/, const int letterSpacing /*= 5*/, 
	const int lineSpacing /*= 5*/, const int addToSpaceLength /*= 0*/) {

	const unsigned int TOTAL_FONTS = 256;

	static FT_Library library;
	FT_Error error = FT_Init_FreeType(&library);
	if (error) {
		REPORT_ERROR("Failed to initialize FreeType.", initFromFontFile);
		return false;
	}

	unsigned int maxCellWidth = 0;
	unsigned int maxCellHeight = 0;
	int maxBearing = 0;
	int maxHang = 0;

	std::vector<TextureData> characterTextures;
	characterTextures.resize(TOTAL_FONTS);

	std::vector<FT_Glyph_Metrics> characterMetrics;
	characterMetrics.resize(TOTAL_FONTS);

	FT_Face face = 0;
	error = FT_New_Face(library, fontFilePath.c_str(), 0, &face);
	if (error) {
		REPORT_ERROR("Failed to create font face.", initFromFontFile);

		FT_Done_FreeType(library);
		return false;
	}

	error = FT_Set_Pixel_Sizes(face, 0, fontSize);
	if (error) {
		REPORT_ERROR("Failed to set font size.", initFromFontFile);

		FT_Done_Face(face);
		FT_Done_FreeType(library);
		return false;
	}

	for (int i = 0; i < TOTAL_FONTS; i++) {

		error = FT_Load_Char(face, i, FT_LOAD_RENDER);
		if (error) {
			REPORT_ERROR("Failed to load character " + std::to_string((char)i) + ".", initFromFontFile);

			FT_Done_Face(face);
			FT_Done_FreeType(library);
			return false;
		}

		characterMetrics[i] = face->glyph->metrics;

		characterTextures[i].width = face->glyph->bitmap.width;
		characterTextures[i].height = face->glyph->bitmap.rows;

		characterTextures[i].data = new unsigned char[characterTextures[i].width * characterTextures[i].height];

		memcpy(characterTextures[i].data, face->glyph->bitmap.buffer,
			characterTextures[i].width * characterTextures[i].height);

		if (characterMetrics[i].horiBearingY / 64 > maxBearing) {
			maxBearing = characterMetrics[i].horiBearingY / 64;
		}

		if (characterMetrics[i].width / 64 > maxCellWidth) {
			maxCellWidth = characterMetrics[i].width / 64;

		}

		int glyphHang = (characterMetrics[i].height - characterMetrics[i].horiBearingY) / 64;
		if (glyphHang > maxHang)
		{
			maxHang = glyphHang;
		}
	}

	maxCellHeight = maxBearing + maxHang;

	unsigned int textureWidth = maxCellWidth * 16;
	unsigned int textureHeight = maxCellHeight * 16;

	m_fontTexture.data = new unsigned char[textureWidth * textureHeight];
	memset(m_fontTexture.data, 0, textureWidth * textureHeight);

	m_fontTexture.width = textureWidth;
	m_fontTexture.height = textureHeight;
	m_fontTexture.bitsPerPixel = 1;

	UVDimension currentUV = {};
	unsigned int currentChar = 0;

	m_uvDimensions.resize(256);
	m_characterWidths.resize(256);

	unsigned int currentCellX = 0;
	unsigned int currentCellY = 0;

	for (int row = 0; row < 16; row++) {
		for (int column = 0; column < 16; column++) {

			currentCellX = maxCellWidth * column;
			currentCellY = maxCellHeight * row;

			currentUV.set(
				(float)maxCellWidth * column / (float)m_fontTexture.width,

				// if texture height is 10 and cell height is 2
				// for row = 0, (10 - ((0 + 1) * 2)) = 10 - 2 = 8
				// for row = 1, (10 - ((1 + 1) * 2)) = 10 - 4 = 6
				// for row = 4, (10 - ((4 + 1) * 2)) = 10 - 10 = 10
				(float)(m_fontTexture.height - ((row + 1) * maxCellHeight)) / (float)m_fontTexture.height,


				(float)(characterMetrics[currentChar].width / 64) / (float)m_fontTexture.width,
				(float)maxCellHeight / (float)m_fontTexture.height
			);

			m_uvDimensions[currentChar] = currentUV;
			m_characterWidths[currentChar] = characterMetrics[currentChar].width / 64;

			int currentCharTop = currentCellY + maxBearing - characterMetrics[currentChar].horiBearingY / 64;

			unsigned char* destPixels = m_fontTexture.data;
			unsigned char* srcPixels = characterTextures[currentChar].data;

			for (int i = 0; i < characterTextures[currentChar].height; i++) {

				memcpy(
					&destPixels[(i + currentCharTop) * m_fontTexture.width + currentCellX],
					&srcPixels[i * characterTextures[currentChar].width],
					characterTextures[currentChar].width
				);
			}

			currentChar++;
		}
	}

	ImageLoader::BufferTextureData(m_fontTexture);

	delete[] m_fontTexture.data;
	m_fontTexture.data = nullptr;

	for (auto& it : characterTextures) {
		delete[] it.data;
		it.data = nullptr;
	}

	FT_Done_Face(face);
	FT_Done_FreeType(library);

	m_spaceSize = maxCellWidth / 2;
	m_newLine = maxBearing;
	m_lineHeight = maxCellHeight;

	for (int i = 0; i < 256; i++) {
		m_uvDimensions[i].height = (float)m_lineHeight / (float)m_fontTexture.height;
	}

	m_fontName = fontName;
	m_initialized = true;

	m_letterSpacing = letterSpacing;
	m_lineSpacing = lineSpacing;
	m_addToSpaceLength = addToSpaceLength;
	m_fontScale = fontScale;

	return true;
}

void Font::drawTextToRenderer(const std::string& text, const int topLeftX, const int topLeftY, 
	const ColorRGBA& color, TextureRenderer& textureRenderer) {

	if (m_fontTexture.id == 0) {
		REPORT_ERROR("Didn't load any font yet.", drawTextToRenderer);
		return;
	}

	/*textureRenderer.draw(
		GlyphOrigin::BOTTOM_LEFT,
		RectDimension { 0,0,700,700 },
		UVDimension { 0.0f, 0.0f, 1.0f, 1.0f },
		m_fontTexture.id, ColorRGBA { 255,255,255,255 }
	);*/

	int drawX = topLeftX;
	int drawY = topLeftY;
	
	RectDimension currentDims = {};

	for (int i = 0; i < text.length(); i++) {
		if (text[i] == ' ') {
			drawX += (m_spaceSize + m_addToSpaceLength) * m_fontScale;
		}
		else if (text[i] == '\n') {
			drawX = topLeftX;
			drawY -= (m_newLine + m_lineSpacing) * m_fontScale;
		}
		else {
			unsigned int ASCII = (unsigned char) text[i];

			currentDims.set(
				drawX,
				drawY,
				m_characterWidths[ASCII] * m_fontScale,
				m_lineHeight * m_fontScale
			);

			textureRenderer.draw(
				GlyphOrigin::TOP_LEFT,
				currentDims,
				m_uvDimensions[ASCII],
				m_fontTexture.id,
				color
			);

			drawX += (m_characterWidths[ASCII] + m_letterSpacing) * m_fontScale;
		}
	}
}

unsigned int Font::getLineWidth(const std::string& text) {

	unsigned int width = 0;

	for (int i = 0; i < text.length(); i++) {
		if (text[i] == '\n') {
			break;
		}
		else if (text[i] == ' ') {
			width += (m_spaceSize + m_addToSpaceLength) * m_fontScale;
		}
		else {
			width += (m_characterWidths[(unsigned char)text[i]] + m_letterSpacing) * m_fontScale;
		}
	}
	return width;
}

void Font::deleteFont() {
	ImageLoader::DeleteTexture(m_fontTexture);

	m_uvDimensions.clear();
	m_characterWidths.clear();
}