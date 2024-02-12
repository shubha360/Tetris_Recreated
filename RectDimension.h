#pragma once

struct RectDimension {
	int bottomLeftX, bottomLeftY;
	unsigned int width, height;

	void set(int bottomLeftX, int bottomLeftY, unsigned int width, unsigned int height) {
		this->bottomLeftX = bottomLeftX;
		this->bottomLeftY = bottomLeftY;
		this->width = width;
		this->height = height;
	}
};