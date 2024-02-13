#include "Font.h"

Font::Font() { }

Font::~Font() {
	ImageLoader::DeleteTexture(m_fontTexture);
}

bool Font::initFontBitmap16x16(const std::string& bmpFilePath, const float fontScale /*= 1.0f*/) {

	ImageLoader::LoadTextureFromImage("resources/fonts/lazy_font.png", m_fontTexture, 1);

	if (m_fontTexture.data == nullptr) {
		REPORT_ERROR("Failed to load image at " + bmpFilePath + " for bitmap font.", initFontBitmap16x16);
		return false;
	}

	m_bitmapFontScale = fontScale;

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

	// top height
	// if top is 2 and cell height is 20
	// then top height will be, cell height - top
	// 20 - 2 = 18
	m_maxHeight = CELL_HEIGHT - top;

	for (int i = 0; i < 256; i++) {
		m_uvDimensions[i].height = (float) m_maxHeight / (float) m_fontTexture.height;
	}

	ImageLoader::BufferTextureData(m_fontTexture);
	ImageLoader::FreeTexture(m_fontTexture);

	m_spaceSize = CELL_WIDTH / 2;
	m_newLine = aBottom - top;
	m_lineHeight = bottom - top;

	return true;
}

void Font::renderText(const std::string& text, const int topLeftX, const int topLeftY, 
	const ColorRGBA& color, TextureRenderer& textureRenderer) {

	if (m_fontTexture.id != 0) {
		REPORT_ERROR("Didn't load any font yet.", renderText);
		return;
	}

	int drawX = topLeftX;
	int drawY = topLeftY;
	
	RectDimension currentDims = {};

	for (int i = 0; i < text.length(); i++) {
		if (text[i] == ' ') {
			drawX += m_spaceSize * m_bitmapFontScale;
		}
		else if (text[i] == '\n') {
			drawX = topLeftX;
			drawY -= m_newLine * m_bitmapFontScale;
		}
		else {
			unsigned int ASCII = (unsigned char) text[i];

			currentDims.set(
				drawX,
				drawY,
				m_characterWidths[ASCII] * m_bitmapFontScale,
				m_maxHeight * m_bitmapFontScale
			);

			textureRenderer.draw(
				GlyphOrigin::TOP_LEFT,
				currentDims,
				m_uvDimensions[ASCII],
				m_fontTexture.id,
				color
			);

			drawX += m_characterWidths[ASCII] * m_bitmapFontScale;
		}
	}
}

//bool Font::initFontTTF(const std::string& fontFilePath,
//    const unsigned int fontSize /*= 32*/, const unsigned int spaceSize /*= 0*/) {
//
//    const unsigned int TOTAL_FONTS = 256;
//
//    static FT_Library library;
//    FT_Error error = FT_Init_FreeType(&library);
//    if (error) {
//        printf("Error at initFont() in Font.cpp\n"
//            "Failed to initialize FreeType! FT error: %X", error);
//        return false;
//    }
//
//    unsigned int maxCellWidth = 0;
//    unsigned int maxCellHeight = 0;
//    int maxBearing = 0;
//    int maxHang = 0;
//
//    std::vector<TextureData> characterTextures;
//    characterTextures.resize(TOTAL_FONTS);
//    
//    std::vector<FT_Glyph_Metrics> characterMetrics;
//    characterMetrics.resize(TOTAL_FONTS);
//
//    FT_Face face = 0;
//    error = FT_New_Face(library, fontFilePath.c_str(), 0, &face);
//    if (error) {
//        printf("Error at initFont() in Font.cpp\n"
//            "Failed to create font face! FT error: %X", error);
//        
//        FT_Done_FreeType(library);
//        return false;
//    }
//
//    error = FT_Set_Pixel_Sizes(face, 0, fontSize);
//    if (error) {
//        printf("Error at initFont() in Font.cpp\n"
//            "Failed to set font size! FT error: %X", error);
//
//        FT_Done_Face(face);
//        FT_Done_FreeType(library);
//        return false;
//    }
//
//    for (int i = 0; i < TOTAL_FONTS; i++) {
//
//        error = FT_Load_Char(face, i, FT_LOAD_RENDER);
//        if (error) {
//            printf("Error at initFont() in Font.cpp\n"
//                "Failed to load character %c ! FT error: %X", i, error);
//
//            FT_Done_Face(face);
//            FT_Done_FreeType(library);
//            return false;
//        }
//
//        characterMetrics[i] = face->glyph->metrics;
//
//        characterTextures[i].width = face->glyph->bitmap.width;
//        characterTextures[i].height = face->glyph->bitmap.rows;
//
//        
//        
//        /// TODO: DO NOT FORGET TO DELETE THIS DATA!!!
//
//        characterTextures[i].data = new unsigned char[characterTextures[i].width * characterTextures[i].height];
//        
//        memcpy(characterTextures[i].data, face->glyph->bitmap.buffer, 
//            characterTextures[i].width * characterTextures[i].height);
//
//        if (characterMetrics[i].horiBearingY / 64 > maxBearing) {
//            maxBearing = characterMetrics[i].horiBearingY / 64;
//        }
//
//        if (characterMetrics[i].width / 64 > maxCellWidth) {
//            maxCellWidth = characterMetrics[i].width / 64;
//        }
//
//        // TODO: Experiment with glyphHang
//
//        int glyphHang = (characterMetrics[i].horiBearingY - characterMetrics[i].height) / 64;
//        if (glyphHang < maxHang)
//        {
//            maxHang = glyphHang;
//        }
//    }
//
//    maxCellHeight = maxBearing - maxHang;
//
//    FT_Done_FreeType(library);
//    return true;
//}