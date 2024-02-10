#include "Camera.h"

Camera::Camera() {}

Camera::~Camera() {}

bool Camera::init(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, glm::mat4 modelMatrix) {
	this->m_projectionMatrix = projectionMatrix;
	this->m_viewMatrix = viewMatrix;
	this->m_modelMatrix = modelMatrix;

	this->m_mvp = m_projectionMatrix * m_viewMatrix * m_modelMatrix;

	return true;
}

void Camera::sendMatrixDataToShader(GLSLProgram& shaderProgram) {
	GLint mvpLoc = shaderProgram.getUniformLocation("mvpMatrix");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &m_mvp[0][0]);
}
