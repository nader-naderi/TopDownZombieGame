#ifndef BOOK_COLLISION_HPP
#define BOOK_COLLISION_HPP

#include <SFML/Graphics.hpp>

class Collision
{
public:
    Collision() = delete;
    Collision(const Collision&) = delete;
    Collision& operator=(const Collision&) = delete;

    static bool circleTest(const sf::Sprite& first, const sf::Sprite& second);
};

#endif
