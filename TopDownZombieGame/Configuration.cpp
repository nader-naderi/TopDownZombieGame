#include "Configuration.hpp"

ResourceManager<sf::Texture, int> Configuration::textures;
ResourceManager<sf::SoundBuffer, int> Configuration::soundBuffers;

ActionMap<int> Configuration::playerInputs;

void Configuration::initialize()
{
    initTextures();
	initSoundEffects();
    initPlayerInputs();
}

void Configuration::initTextures()
{
	textures.load(Textures::Player, "graphics/Top_Down_Survivor/handgun/idle/survivor-idle_handgun_0.png");
	textures.load(Textures::Zombie, "graphics/zombie/idle/Idle.png");
}

void Configuration::initSoundEffects()
{
	soundBuffers.load(SoundEffect::Shoot, "sound/shoot.mp3");
	soundBuffers.load(SoundEffect::Reload, "sound/reload.wav");
	soundBuffers.load(SoundEffect::Reload_Failed, "sound/reload_failed.wav");
	soundBuffers.load(SoundEffect::Powerup, "sound/powerup.wav");
}

void Configuration::initPlayerInputs()
{
	playerInputs.map(PlayerInputs::Up, Action(sf::Keyboard::W));
	playerInputs.map(PlayerInputs::Left, Action(sf::Keyboard::A));
	playerInputs.map(PlayerInputs::Down, Action(sf::Keyboard::S));
	playerInputs.map(PlayerInputs::Right, Action(sf::Keyboard::D));
	playerInputs.map(PlayerInputs::Fire, Action(sf::Mouse::Left));
}