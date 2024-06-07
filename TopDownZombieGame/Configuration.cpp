#include "Configuration.hpp"

ResourceManager<sf::Texture, int> Configuration::textures;
ResourceManager<sf::SoundBuffer, int> Configuration::soundBuffers;
ResourceManager<sf::Font, int> Configuration::fonts;
ResourceManager<sf::Music, int> Configuration::musics;

ActionMap<int> Configuration::playerInputs;

Player* Configuration::player = nullptr;

void Configuration::initialize()
{
	initFonts();
    initTextures();
	initMusics();
	initSoundEffects();

    initPlayerInputs();

	musics.get(Musics::Theme).setLoop(true);
	musics.get(Musics::Theme).play();
}

void Configuration::initFonts()
{
	fonts.load(Fonts::GUI, "fonts/OpenSans-Regular.ttf");
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

void Configuration::initMusics()
{
	musics.load(Musics::Theme, "sound/ambientmain_0.ogg");
}