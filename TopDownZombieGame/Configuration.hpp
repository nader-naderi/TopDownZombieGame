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

    enum Textures : int { Player, Zombie,  };
    enum SoundEffect : int { Shoot, Reload, Reload_Failed, Powerup };

    static ResourceManager<sf::Texture, int> textures;
    static ResourceManager<sf::SoundBuffer, int> soundBuffers;

    enum PlayerInputs : int { Up, Left, Right, Down, Fire };
    static ActionMap<int> playerInputs;

    static void initialize();

private:

    static void initTextures();
    static void initSoundEffects();
    static void initPlayerInputs();
};

#endif