#include "Scene.hpp"
#include "GameObject.hpp"

Scene::Scene(float x, float y) : _x(x), _y(y)
{
}

Scene::~Scene()
{
    clear();
}

void Scene::add(GameObject* entity)
{
    _entitiesTmp.push_back(entity);
}

void Scene::clear()
{
    for (GameObject* entity : _entities)
        delete entity;
    _entities.clear();

    for (GameObject* entity : _entitiesTmp)
        delete entity;
    _entitiesTmp.clear();

    _sounds.clear();
}

void Scene::add(Configuration::SoundEffect sound_id)
{
    std::unique_ptr<sf::Sound> sound(new sf::Sound(Configuration::soundBuffers.get(sound_id)));
    sound->setAttenuation(0);
    sound->play();
    _sounds.emplace_back(std::move(sound));
}

bool Scene::isCollide(const GameObject& other)
{
    for (GameObject* entity_ptr : _entities)
        if (other.isCollide(*entity_ptr))
            return true;
    return false;
}
int Scene::size()
{
    return _entities.size() + _entitiesTmp.size();
}

int Scene::getX()const
{
    return _x;
}

int Scene::getY()const
{
    return _y;
}

const std::list<GameObject*> Scene::getEntities()const
{
    return _entities;
}

void Scene::update(sf::Time deltaTime)
{
    if (_entitiesTmp.size() > 0)
        _entities.merge(_entitiesTmp);

    for (GameObject* entity_ptr : _entities)
    {
        GameObject& entity = *entity_ptr;

        entity.update(deltaTime);

        sf::Vector2f pos = entity.getPosition();

        if (pos.x < 0)
        {
            pos.x = _x;
            pos.y = _y - pos.y;
        }
        else if (pos.x > _x)
        {
            pos.x = 0;
            pos.y = _y - pos.y;
        }

        if (pos.y < 0)
            pos.y = _y;
        else if (pos.y > _y)
            pos.y = 0;

        entity.setPosition(pos);
    }


    const auto end = _entities.end();
    for (auto it_i = _entities.begin(); it_i != end; ++it_i)
    {
        GameObject& entity_i = **it_i;
        auto it_j = it_i;
        it_j++;
        for (; it_j != end; ++it_j)
        {
            GameObject& entity_j = **it_j;

            if (entity_i.isAlive() and entity_i.isCollide(entity_j))
                entity_i.onDestroy();

            if (entity_j.isAlive() and entity_j.isCollide(entity_i))
                entity_j.onDestroy();
        }
    }

    for (auto it = _entities.begin(); it != _entities.end();)
    {
        if (not (*it)->isAlive())
        {
            delete* it;
            it = _entities.erase(it);
        }
        else
            ++it;
    }

    _sounds.remove_if([](const std::unique_ptr<sf::Sound>& sound) -> bool {
        return sound->getStatus() != sf::SoundSource::Status::Playing;
        });
}

void Scene::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (GameObject* entity : _entities)
        target.draw(*entity, states);
}