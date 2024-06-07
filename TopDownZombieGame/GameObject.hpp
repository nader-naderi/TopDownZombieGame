#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <SFML/Graphics.hpp> //Drawable, Sprite
#include "Configuration.hpp" //Configuration

class Scene;

class GameObject : public sf::Drawable
{
public:
    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;

    GameObject(Configuration::Textures tex_id, Scene& world);

    virtual ~GameObject();

    virtual bool isAlive()const;

    const sf::Vector2f& getPosition()const;

    template<typename ... Args>
    void setPosition(Args&& ... args);

    virtual bool isCollide(const GameObject& other)const = 0;

    virtual void update(sf::Time deltaTime) = 0;

    virtual void onDestroy();

protected:
    friend class Player;
    sf::Sprite          _sprite;

    sf::Vector2f        _impulse;

    Scene& scene;

    bool _alive;

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#include "GameObject.tpl"
#endif
