#include <utility> //forward

template<typename ... Args>
void GameObject::setPosition(Args&& ... args)
{
    _sprite.setPosition(std::forward<Args>(args)...);
}