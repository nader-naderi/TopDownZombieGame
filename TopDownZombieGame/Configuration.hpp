#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include "ResourceManager.hpp"
#include "ActionMap.h"

class Configuration
{
public:
    Configuration() = delete;
    Configuration(const Configuration&) = delete;
    Configuration& operator=(const Configuration&) = delete;

    enum Textures : int { Player };
    static ResourceManager<sf::Texture, int> textures;

    enum PlayerInputs : int { Up, Left, Right, Down };
    static ActionMap<int> playerInputs;

    static void initialize();

private:

    static void initTextures();

    static void initPlayerInputs();
};

#endif