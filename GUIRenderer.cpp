#include "GUIRenderer.h"

GUIRenderer::GUIRenderer() {}

GUIRenderer::~GUIRenderer() {
	freeGUIRenderer();
}

bool GUIRenderer::init() {
	if (!m_glslProgram.compileAndLinkShaders("gui_resources/shaders/gui_shader.vert",
		"gui_resources/shaders/gui_shader.frag")) {
		REPORT_ERROR("Failed to compile or link GUI shader.", init);
		return false;
	}

	ImageLoader::LoadTextureFromImage(
		"gui_resources/images/rounded_rect_button512x256.png", m_roundedRectButtonTexture, 4
	);
	ImageLoader::BufferTextureData(m_roundedRectButtonTexture);
	ImageLoader::FreeTexture(m_roundedRectButtonTexture);

	if (!m_font.initFromFontFile("gui_resources/fonts/Quicksand-Regular.otf", 64)) {
		REPORT_ERROR("Failed to initialize font for GUI.", init);
		return false;
	}
	return true;
}

void GUIRenderer::renderGUI(GUI& gui, Camera& camera) {
	
	m_glslProgram.useProgram();

	glActiveTexture(GL_TEXTURE0);
	GLint samplerLoc = m_glslProgram.getUniformLocation("u_imageSampler");
	glUniform1i(samplerLoc, 0);

	camera.sendMatrixDataToShader(m_glslProgram);

	m_renderer.begin();

	for (auto& comp : gui.m_components) {
		
		if (comp->m_isVisible) {
			
			switch (comp->m_type) {
			
			case GUI::Component::ComponentType::BUTTON:
				
				GUI::Button* button = (GUI::Button*) comp.get();

				m_renderer.draw(
					button->m_renderOrigin,
					button->m_dimension,
					UVDimension{ 0.0f, 0.0f, 1.0f, 1.0f },
					m_roundedRectButtonTexture.id,
					button->m_buttonColor
					);
				
				m_font.setFontScale(button->m_labelScale);

				if (!button->m_labelCoordinatesFound) {
					getLabelCoordinates(button->m_labelTopLeftX, button->m_labelTopLeftY, 
						button->m_label, button->m_centerX, button->m_centerY);

					button->m_labelCoordinatesFound = true;
				}

				m_font.drawTextToRenderer(button->m_label, button->m_labelTopLeftX, button->m_labelTopLeftY, 
					button->m_primaryColor, m_renderer);

				break;
			}
		}
	}

	m_renderer.end();

	m_renderer.renderTextures();
}

void GUIRenderer::freeGUIRenderer() {
	m_glslProgram.freeProgram();

	ImageLoader::DeleteTexture(m_roundedRectButtonTexture);
}

void GUIRenderer::getLabelCoordinates(int& x, int& y, const std::string& label, 
	const int componentCenterX, const int componentCenterY) {	
	
	unsigned int labelWidth = m_font.getLineWidth(label);
	unsigned int labelHeight = m_font.getLineHeight();

	x = componentCenterX - labelWidth / 2;
	y = componentCenterY + labelHeight / 2;
}

