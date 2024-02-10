#include "TextureRenderer.h"

Glyph::Glyph(const RectDimension& destRect, const UVDimension& uvRect, GLuint textureID, const ColorRGBA& color) {
	this->m_textureID = textureID;

	// TOP LEFT
	m_vertices[0].setPosition(destRect.topLeftX, destRect.topLeftY);
	m_vertices[0].setTextureCoords(uvRect.bottomLeftX, uvRect.bottomLeftY + uvRect.height);
	m_vertices[0].color = color;

	// TOP RIGHT
	m_vertices[1].setPosition(destRect.topLeftX + destRect.width, destRect.topLeftY);
	m_vertices[1].setTextureCoords(uvRect.bottomLeftX + uvRect.width, uvRect.bottomLeftY + uvRect.height);
	m_vertices[1].color = color;

	// BOTTOM RIGHT
	m_vertices[2].setPosition(destRect.topLeftX + destRect.width, destRect.topLeftY + destRect.height);
	m_vertices[2].setTextureCoords(uvRect.bottomLeftX + uvRect.width, uvRect.bottomLeftY);
	m_vertices[2].color = color;

	// BOTTOM LEFT
	m_vertices[3].setPosition(destRect.topLeftX, destRect.topLeftY + destRect.height);
	m_vertices[3].setTextureCoords(uvRect.bottomLeftX, uvRect.bottomLeftY);
	m_vertices[3].color = color;
}

RenderBatch::RenderBatch(unsigned int offset, unsigned int numIndices, GLuint textureID) :
	m_offset(offset), m_numIndices(numIndices), m_textureID(textureID) {}

TextureRenderer::TextureRenderer() {}

TextureRenderer::~TextureRenderer() {
	freeTextureRenderer();
}

void TextureRenderer::begin() {

	if (m_vaoID == 0) {
		createVao();
	}

	m_glyphs.clear();
	m_glyphPointers.clear();
	m_renderBatches.clear();
	m_vertexIndices.clear();
}

void TextureRenderer::draw(const RectDimension& destRect, const UVDimension& uvRect, GLuint textureID, const ColorRGBA& color) {
	m_glyphs.emplace_back(destRect, uvRect, textureID, color);
}

void TextureRenderer::end() {
	if (!m_glyphs.empty()) {
		m_glyphPointers.resize(m_glyphs.size());

		for (int i = 0; i < m_glyphPointers.size(); i++) {
			m_glyphPointers[i] = &m_glyphs[i];
		}

		std::stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), compareByTextureID);

		setupRenderBatches();
	}
}

void TextureRenderer::renderTextures() {
	if (!m_glyphPointers.empty()) {
		glBindVertexArray(m_vaoID);

		for (auto& batch : m_renderBatches) {
			glBindTexture(GL_TEXTURE_2D, batch.m_textureID);
			glDrawElements(GL_TRIANGLES, batch.m_numIndices, GL_UNSIGNED_INT, &(m_vertexIndices[batch.m_offset]));
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		glBindVertexArray(0);
	}
}

void TextureRenderer::freeTextureRenderer() {
	
	if (m_vboID != 0) {
		glDeleteBuffers(1, &m_vboID);
		m_vboID = 0;
	}
	
	if (m_vaoID != 0) {
		glDeleteVertexArrays(1, &m_vaoID);
		m_vaoID = 0;
	}
}

void TextureRenderer::createVao() {
	if (m_vaoID == 0) {
		glGenVertexArrays(1, &m_vaoID);
	}

	if (m_vboID == 0) {
		glGenBuffers(1, &m_vboID);
	}

	glBindVertexArray(m_vaoID);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 2, GL_INT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, position));
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, color));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, textureCoords));

	glBindVertexArray(0);
}

void TextureRenderer::setupRenderBatches() {

	if (!m_glyphPointers.empty()) {

		// setup the vbo and buffer vertex data
		std::vector<Vertex2D> vertices;
		vertices.resize(m_glyphPointers.size() * 4);

		unsigned int currentVertex = 0;

		for (auto& glyph : m_glyphPointers) {
			for (int vertex = 0; vertex < 4; vertex++) {
				vertices[currentVertex++] = glyph->m_vertices[vertex];
			}
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex2D), nullptr, GL_DYNAMIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex2D), vertices.data());

		glBindBuffer(GL_ARRAY_BUFFER, 0);


		// Setup render batches and the ibo
		m_vertexIndices.resize(m_glyphPointers.size() * 6);

		unsigned int numIndices = 6;
		unsigned int curentIndex = 0;
		currentVertex = 0;

		m_renderBatches.emplace_back(curentIndex, numIndices, m_glyphPointers[0]->m_textureID);
		addIndices(curentIndex, currentVertex);
		curentIndex += 6;
		currentVertex += 4;

		for (int i = 1; i < m_glyphPointers.size(); i++) {
			if (m_glyphPointers[i]->m_textureID == m_glyphPointers[i - 1]->m_textureID) {
				m_renderBatches.back().m_numIndices += 6;
				addIndices(curentIndex, currentVertex);
				curentIndex += 6;
				currentVertex += 4;
			}
			else {
				m_renderBatches.emplace_back(curentIndex, numIndices, m_glyphPointers[i]->m_textureID);
				addIndices(curentIndex, currentVertex);
				curentIndex += 6;
				currentVertex += 4;
			}
		}
	}
}

void TextureRenderer::addIndices(const unsigned int currentIndex, const unsigned int currentVertex) {
	// first triangle
	m_vertexIndices[currentIndex] = currentVertex; // top left
	m_vertexIndices[currentIndex + 1] = currentVertex + 1; // top right
	m_vertexIndices[currentIndex + 2] = currentVertex + 2; // bottom right

	// second triangle
	m_vertexIndices[currentIndex + 3] = currentVertex; // top left
	m_vertexIndices[currentIndex + 4] = currentVertex + 3; // bottom left
	m_vertexIndices[currentIndex + 5] = currentVertex + 2; // bottom right
}

bool TextureRenderer::compareByTextureID(Glyph* a, Glyph* b) {
	return (a->m_textureID < b->m_textureID);
}
