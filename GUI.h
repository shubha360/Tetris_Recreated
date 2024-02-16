#pragma once

#include "GLSLProgram.h"
#include "TextureRenderer.h"
#include "RectDimension.h"
#include "UVDimension.h"
#include "ColorRGBA.h"
#include "InputProcessor.h"
#include "Camera.h"

#include <vector>
#include <iostream>
#include <string>
#include <memory>
#include <functional>

#include <SDL/SDL_events.h>
#include <glm/glm.hpp>

class GUIRenderer;

class GUI {
public:
	friend class GUIRenderer;

	GUI();
	~GUI();

	bool init();

	void addTextButton(const std::string& label, float labelScale, 
		const ColorRGBA& textColor, const ColorRGBA& buttonColor,
		const GlyphOrigin& renderOrigin, const RectDimension& dimension, std::function<void()> buttonFunction);

	void updateGUI(InputProcessor& inputProcessor, Camera& camera);

	void freeGUI();

private:
	class Component {
	public:
		friend class GUI;
		friend class GUIRenderer;

		enum class ComponentType { NONE, BUTTON };

		Component();
		virtual ~Component();

		void showComponent() { m_isVisible = true; };
		void hideComponent() { m_isVisible = false; };

	protected:
		std::string m_label = "";
		GlyphOrigin m_renderOrigin = GlyphOrigin::BOTTOM_LEFT;
		ComponentType m_type = ComponentType::NONE;
		RectDimension m_dimension = {};
		float m_labelScale = 0;
		ColorRGBA m_primaryColor = {};

		int m_centerX = 0, m_centerY = 0;
		int m_labelTopLeftX = 0, m_labelTopLeftY = 0;

		bool m_isFunctional = true;
		bool m_isVisible = true;
		bool m_labelCoordinatesFound = false;

		void findComponentCenter();
	};

	
	
	class Button : public Component {
	public:
		friend class GUI;
		friend class GUIRenderer;

		Button(const std::string& label, float labelScale, const ColorRGBA& textColor,
			const ColorRGBA& buttonColor, const GlyphOrigin& renderOrigin, 
			const RectDimension& dimension, std::function<void()> buttonFunction);

	private:
		ColorRGBA m_buttonColor;
		std::function<void()> m_buttonFunc;
	};

	std::vector<std::unique_ptr<Component>> m_components;

	SDL_Cursor* m_arrowCursor = nullptr;
	SDL_Cursor* m_indexPointerCursor = nullptr;

	SDL_Cursor* m_currentCursor = nullptr;

	bool isMouseInsideComponent(const glm::ivec2& mouseScreenCoords, Component& component);
};