#include "Configuration.hpp"

ResourceManager<sf::Texture, int> Configuration::textures;
ActionMap<int> Configuration::playerInputs;

void Configuration::initialize()
{
    initTextures();
    initPlayerInputs();
}

void Configuration::initTextures()
{
    textures.load(Textures::Player, "graphics/Top_Down_Survivor/handgun/idle/survivor-idle_handgun_0.png");
}

void Configuration::initPlayerInputs()
{
	playerInputs.map(PlayerInputs::Up, Action(sf::Keyboard::W));
	playerInputs.map(PlayerInputs::Left, Action(sf::Keyboard::A));
	playerInputs.map(PlayerInputs::Down, Action(sf::Keyboard::S));
	playerInputs.map(PlayerInputs::Right, Action(sf::Keyboard::D));
}