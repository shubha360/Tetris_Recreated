#pragma once

#include <GL/glew.h>
#include <map>

#include "stb_image.h"
#include "TextureData.h"

class ImageLoader {
public:
	ImageLoader();

	void loadTextureFromImage32(const std::string& imagePath, TextureData& texture);
	void bufferTextureData(TextureData& texture);
	void freeTexture(TextureData& texture);
	void deleteTexture(TextureData& texture);

private:
	std::map<std::string, TextureData> m_textureCache;
};
