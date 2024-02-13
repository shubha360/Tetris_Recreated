#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>

#include "RectDimension.h"
#include "UVDimension.h"
#include "Vertex2D.h"

class TextureRenderer;
class RenderBatch;

enum class GlyphOrigin {
	BOTTOM_LEFT,
	BOTTOM_RIGHT,
	TOP_RIGHT,
	TOP_LEFT,
	CENTER
};

class Glyph {
public:	
	friend class TextureRenderer;
	friend class RenderBatch;

	Glyph(const GlyphOrigin renderOrigin, const RectDimension& destRect, const UVDimension& uvRect, 
		GLuint textureID, const ColorRGBA& color);

private:
	GLuint m_textureID = 0;

	Vertex2D m_vertices[4] = {};
};

class RenderBatch {
public:
	friend class TextureRenderer;	
	RenderBatch(unsigned int offset, unsigned int numIndices, GLuint textureID);

private:

	unsigned int m_offset;
	unsigned int m_numIndices;
	GLuint m_textureID;
};

class TextureRenderer {
public:
	TextureRenderer();
	~TextureRenderer();

	void begin();

	void draw(const GlyphOrigin renderOrigin,  const RectDimension& destRect, const UVDimension& uvRect, 
		GLuint textureID, const ColorRGBA& color);

	void end();

	void renderTextures();

	void freeTextureRenderer();	

private:
	GLuint m_vaoID = 0, m_vboID = 0;

	std::vector<GLuint> m_vertexIndices;

	std::vector<Glyph> m_glyphs;
	std::vector<Glyph*> m_glyphPointers;
	std::vector<RenderBatch> m_renderBatches;

	void createVao();
	void setupRenderBatches();
	void addIndicesToBuffer(unsigned int& currentIndex, unsigned int& currentVertex);

	static bool compareByTextureID(Glyph* a, Glyph* b);
};