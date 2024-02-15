#pragma once

#include <freetype/ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include <vector>

#include "TextureData.h"
#include "ImageLoader.h"
#include "UVDimension.h"
#include "ColorRGBA.h"
#include "TextureRenderer.h"
#include "ErrorReporter.h"

class Font {
public:
	Font();
	~Font();

	// only font bitmap that has 16 rows and 16 colummn
	bool initFontBitmap16x16(const std::string& bmpFilePath, const float fontScale = 1.0f, 
		const int letterSpacing = 0, const int lineSpacing = 0, const int addToSpaceLength = 0);

	bool initFromFontFile(const std::string& fontFilePath, const unsigned int fontSize = 32, 
		const int letterSpacing = 0, const int lineSpacing = 0, const int addToSpaceLength = 0);

	void renderText(const std::string& text, const int topLeftX, const int topLeftY,
		const ColorRGBA& color, TextureRenderer& textureRenderer);

	void setLetterSpacing(const int letterSpacing) { m_letterSpacing = letterSpacing; }
	void setLineSpacing(const int lineSpacing) { m_lineSpacing = lineSpacing; }
	void setAddToSpaceLength(const int addToSpaceLength) { m_addToSpaceLength = addToSpaceLength; }

	void deleteFont();

private:
	unsigned int m_spaceSize = 0;
	unsigned int m_newLine = 0;
	unsigned int m_lineHeight = 0;

	int m_letterSpacing = 0;
	int m_lineSpacing = 0;
	int m_addToSpaceLength = 0;

	float m_bitmapFontScale = 1.0f;

	TextureData m_fontTexture;

	std::vector<UVDimension> m_uvDimensions;
	std::vector<int> m_characterWidths;
};