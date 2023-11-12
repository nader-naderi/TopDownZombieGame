#include "InputManager.h"

bool InputManager::isSpriteClicked(sf::Sprite object, sf::Mouse::Button button, sf::RenderWindow& window)
{
    if (sf::Mouse::isButtonPressed(button))
    {
        sf::IntRect tempRect(static_cast<int>(object.getPosition().x), static_cast<int>(object.getPosition().y),
            static_cast<int>(object.getGlobalBounds().width), static_cast<int>(object.getGlobalBounds().height));

        return tempRect.contains(sf::Mouse::getPosition(window));
    }

    return false;
}


sf::Vector2i InputManager::GetMousePosition(sf::RenderWindow& window)
{
    return sf::Mouse::getPosition(window);
}
