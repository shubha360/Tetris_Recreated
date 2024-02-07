#include "InputProcessor.h"

InputProcessor::InputProcessor() {
	m_mouseCoords = glm::ivec2(0);
}

InputProcessor::~InputProcessor() {}

void InputProcessor::pressKey(unsigned int keyID) {
	m_keyMap[keyID] = true;
}

bool InputProcessor::isKeyDown(unsigned int keyID) {
	auto it = m_keyMap.find(keyID);

	if (it != m_keyMap.end()) {
		return it->second;
	}
	return false;
}

void InputProcessor::releaseKey(unsigned int keyID) {
	m_keyMap[keyID] = false;
}
