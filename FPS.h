#pragma once

#include <SDL/SDL_timer.h>
#include <iostream>

class FPS {
public:
	FPS();
	~FPS();

	bool init(float fps);
	void beginFrame();
	bool endFrame();

	float calculateFps();

	void setFps(float fps) { this->desiredFps = fps; }

private:
	float desiredFps = 0.0f;
	float frameStartTicks = 0.0f;

	// for FPS calculation
	float currentFps = 0.0f;

	static const int NUM_SAMPLES = 10;
	float frameTimes[NUM_SAMPLES] = {};
	unsigned int currentFrame = 1;
};

