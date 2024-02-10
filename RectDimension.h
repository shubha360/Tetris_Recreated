#pragma once

struct RectDimension {
	int topLeftX, topLeftY;
	unsigned int width, height;

	void set(int topLeftX, int topLeftY, unsigned int width, unsigned int height) {
		this->topLeftX = topLeftX;
		this->topLeftY = topLeftY;
		this->width = width;
		this->height = height;
	}
};