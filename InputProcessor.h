#pragma once

#include <unordered_map>
#include <glm/glm.hpp>

class InputProcessor {
public:
	InputProcessor();
	~InputProcessor();

	void pressKey(unsigned int keyID);
	bool isKeyDown(unsigned int keyID);
	void releaseKey(unsigned int keyID);

	void setMouseCoords(int x, int y) { m_mouseCoords.x = x; m_mouseCoords.y = y; }

	glm::ivec2 getMouseCoords() { return m_mouseCoords; }

private:
	std::unordered_map<unsigned int, bool> m_keyMap;
	glm::ivec2 m_mouseCoords;
};