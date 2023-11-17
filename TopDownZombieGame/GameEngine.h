#pragma once
#ifndef ZOMBIE_ARENA_H
#define ZOMBIE_ARENA_H

#include "Zombie.h"
#include <sstream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include "TextureHolder.h"
#include "Pickup.h"

using namespace sf;

enum class State { PAUSED, LEVELING_UP, GAME_OVER, PLAYING };

Zombie* createHorde(int numZombies, IntRect arena);
int createBackground(VertexArray& rVA, IntRect arena);

class GameEngine 
{
public:
	void InitEngine();
	GameEngine();

	void InitializeAssets();
	void InitPickups();
	void InitializeAudio();
	void InitUIHighScore();
	void LoadHighscoreFile();
	void InitUIScore();
	void InitUIAmmo();
	void InitUIState_LevelUp();
	void InitUIState_GameOver();
	void InitUIState_Paused();
	void InitializeUIAmmo();
	void InitializeWaves();
	void InitializeZombies();
	void InitializeBackground();
	void InitializeCrosshair();
	void InitUI();
	
	void UpdateInput(Event event);
	void PrepareNextGame();
	void RestartGame();
	void SetState(State newState);
	bool CheckState(State targetState);
	void UpdateCameraInput();
	void HandleReloading();
	void UpdateReloadInput(Event event);
	void UpdateGameStateInput(Event event);
	void HandlePlayerInput();
	void UpdateLevelUpState(Event event);
	void UpgradeFireRate();
	void UpgradeClipSize();
	void UpgradeHealth();
	void UpgradeSpeed();
	void UpgradeHealthPickup();
	void UpgradeAmmoPickup();
	void HandleUpgradeSelection(Event event);
	void ReinitializeTheLevel();
	void UpdateInput();
	void UpdateScene();
	void HandlePhysics();
	void HandlePlayerPhysics();
	void HandleZombiesPhysics();
	void PickupablesPhysics();
	void UpdateUI();
	void UpdateHUD();
	void DrawScene();
	void DrawOnPlayState();
	void DrawOnLevelingUpState();
	void DrawOnPausedState();
	void DrawOnGameOverState();
	void DrawHUD();

	GameEngine(GameEngine&&) {}
	GameEngine(const GameEngine&) = default;
	GameEngine& operator=(const GameEngine&) = delete;
	~GameEngine();

	void InitializeHealthBar();

	void Run();

private:
	// Methods

	// Fields:
	//Objects:
	Time lastPressed;
	Sprite spriteCrosshair;
	Sprite spriteGameOver;
	// Where is the mouse in relation to world coordinates
	Vector2f mouseWorldPosition;
	// Where is the mouse in relation to screen coordinates
	Vector2i mouseScreenPosition;
	TextureHolder holder;
	State state = State::GAME_OVER;
	Player player;
	Time dt;

	Vector2f resolution;

	int numZombies;
	int numZombiesAlive;
	Zombie* zombies = NULL;
	Font font;
	View* mainView;
	View* hudView;
	RenderWindow* window;
	Pickup* healthPickup;
	Pickup* ammoPickup;

	Texture textureIcons = TextureHolder::GetTexture("graphics/zombie_icons.png");
	Texture textureBackground = TextureHolder::GetTexture("graphics/floor-tiles.png");
	Texture textureCrosshair = TextureHolder::GetTexture("graphics/Crosshairs/WeaponCrosshair.png");
	Texture textureGameOver = TextureHolder::GetTexture("graphics/background.png");
	Texture textureAmmoIcon = TextureHolder::GetTexture("graphics/AmmoPickup.png");

	Text zombiesRemainingText;
	int wave = 0;
	Text waveNumberText;
	RectangleShape healthBar;
	// When did we last update the HUD?
	int framesSinceLastHUDUpdate = 0;
	// What time was the last update
	Time timeSinceLastUpdate;
	// How often (in frames) should we update the HUD
	int fpsMeasurementFrameInterval = 1000;
	Sprite spriteAmmoIcon;

	// About the game
	int score = 0;
	int hiScore = 0;
	IntRect arena;
	VertexArray background;

	Clock timerClock;

	Time playTime;

	// Prepare the hit sound
	Sound hitSound;
	Sound splat;
	
	Sound pickup;
	Sound ambient;
	SoundBuffer hitBuffer;
	SoundBuffer splatBuffer;

	SoundBuffer pickupBuffer;
	SoundBuffer ambientBuffer;

	Text pausedText;
	Text gameOverText;
	Text ammoText;
	Text levelUpText;
	Text scoreText;
	Text hiScoreText;
};

#endif