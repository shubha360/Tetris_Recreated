#pragma once

#include <GL/glew.h>
#include <map>

#include "stb_image.h"
#include "TextureData.h"

class ImageLoader {
public:
	static void LoadTextureFromImage32(const std::string& imagePath, TextureData& texture);
	static void LoadTextureFromImage8(const std::string& imagePath, TextureData& texture);

	static void BufferTextureData32(TextureData& texture);
	static void BufferTextureData8(TextureData& texture);

	static void FreeTexture(TextureData& texture);
	static void DeleteTexture(TextureData& texture);

private:
	static std::map<std::string, TextureData> m_textureCache;
};
