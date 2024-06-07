#include "GameObject.hpp"

GameObject::GameObject(Configuration::Textures tex_id, Scene& world) : scene(world), _alive(true)
{
    sf::Texture& texture = Configuration::textures.get(tex_id);
    _sprite.setTexture(texture);
    _sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
}

GameObject::~GameObject()
{
}

bool GameObject::isAlive()const
{
    return _alive;
}

const sf::Vector2f& GameObject::getPosition()const
{
    return _sprite.getPosition();
}

void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(_sprite, states);
}

void GameObject::onDestroy()
{
    _alive = false;
}