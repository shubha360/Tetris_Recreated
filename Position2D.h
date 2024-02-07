#pragma once

#include <GL/glew.h>

struct Position2D {
	GLint x, y;

	void set(GLint x, GLint y) {
		this->x = x;
		this->y = y;
	}
};