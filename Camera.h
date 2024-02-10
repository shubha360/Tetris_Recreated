#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GLSLProgram.h"

class Camera {
public:
	Camera();
	~Camera();

	bool init(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, glm::mat4 modelMatrix);

	void sendMatrixDataToShader(GLSLProgram& shaderProgram);

private:
	glm::mat4 m_mvp = glm::mat4(1.0f);

	glm::mat4 m_projectionMatrix = glm::mat4(1.0f);
	glm::mat4 m_viewMatrix = glm::mat4(1.0f);
	glm::mat4 m_modelMatrix = glm::mat4(1.0f);
};

