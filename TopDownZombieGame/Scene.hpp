#ifndef Scene

#include <SFML/Graphics.hpp> //Drawable
#include <SFML/Audio.hpp> //Drawable
#include <list> //list
#include <memory>

#include "Configuration.hpp"

class GameObject;
class Scene : public sf::Drawable
{
    public:
        Scene(const Scene&) = delete;
        Scene& operator=(const Scene&) = delete;

        Scene(float x, float y);
        ~Scene();

        void update(sf::Time deltaTime);

        void add(GameObject* entity);

        void clear();

        void add(Configuration::SoundEffect sound_id);

        bool isCollide(const GameObject& other);

        int size();

        int getX()const;
        int getY()const;

        const std::list<GameObject*> getEntities()const;

    private:
        std::list<GameObject*>  _entities;
        std::list<GameObject*>  _entitiesTmp;
        std::list<std::unique_ptr<sf::Sound>> _sounds;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        const int _x;
        const int _y;
};
#endif // !Scene