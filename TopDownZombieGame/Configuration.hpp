// The Configuration class in the provided code is a singleton-like class that manages various resources and configuration settings
//  for a game. It contains multiple static members to store and manage textures, sound buffers, fonts, music, player inputs, and the player object itself.

#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include "ResourceManager.hpp"
#include "ActionMap.h"

class Player;
class Configuration
{
public:
    Configuration() = delete;
    Configuration(const Configuration&) = delete;
    Configuration& operator=(const Configuration&) = delete;
    static Player* player;

    enum Textures : int { Player, Zombie, TileSheet, AmmoPickup, HealthPickup };
    enum SoundEffect : int { Shoot, Reload, Reload_Failed, Powerup };
    enum Fonts : int { GUI };
    enum Musics : int { Theme };

    static ResourceManager<sf::Texture, int> textures;
    static ResourceManager<sf::SoundBuffer, int> soundBuffers;
    static ResourceManager<sf::Font, int> fonts;
    static ResourceManager<sf::Music, int> musics;

    enum PlayerInputs : int { Up, Left, Right, Down, Fire, ReloadKey };
    static ActionMap<int> playerInputs;

    static void initialize();

private:

    static void initTextures();
    static void initSoundEffects();
    static void initFonts();
    static void initMusics();

    static void initPlayerInputs();
};

#endif