#pragma once

#include <SFML/Graphics.hpp>

#define AXIS_HORIZONTAL "Horizontal"
#define AXIS_VERTICAL "Vertical"

#define AXIS_MOUSE_X "Mouse X"
#define AXIS_MOUSE_Y "Mouse Y"


class InputManager
{
public:
	InputManager() {}
	~InputManager() {}

	bool isSpriteClicked(sf::Sprite object, sf::Mouse::Button button, sf::RenderWindow& window);

	sf::Vector2i GetMousePosition(sf::RenderWindow& window);

	static sf::Vector2i MousePosition(sf::RenderWindow& window)
	{
		return sf::Mouse::getPosition(window);
	}

	static float GetAxis(std::string axis)
	{
		float target = 0;

		if (axis == AXIS_HORIZONTAL)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
			{
				target = 1.0f;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
			{
				target = -1.0f;
			}
		}
		else if (axis == AXIS_VERTICAL)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
			{
				target = -1.0f;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
			{
				target = 1.0f;
			}
		}
		else if (axis == AXIS_MOUSE_X)
		{
			// TODO:
		}
		else if (axis == AXIS_MOUSE_Y)
		{
			// TODO:
		}

		return target;
	}
};

