#include "GameEngine.h"

using namespace sf;

GameEngine::GameEngine()
{
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;
	RenderWindow win(VideoMode(resolution.x, resolution.y),
		"Top Down Zombie Shooter", Style::Fullscreen);
	window = &win;
	View mview(sf::FloatRect(0, 0, resolution.x, resolution.y));
	mainView = &mview;

	window->setMouseCursorVisible(false);

	View hview(sf::FloatRect(0, 0, resolution.x, resolution.y));
	hudView = &hview;

	Pickup h(1);
	Pickup a(2);

	healthPickup = &h;
	ammoPickup = &a;

	InitEngine();
	InitializeAssets();
	InitPickups();
	Run();
}

void GameEngine::UpdateInput(Event event)
{
	UpdateGameStateInput(event);

	UpdateCameraInput();

	UpdateReloadInput(event);
}


void GameEngine::PrepareNextGame()
{
	SetState(State::LEVELING_UP);
	wave = 0;
	score = 0;

	currentBullet = 0;
	bulletsSpare = 24;
	bulletsInClip = 6;
	clipSize = 6;
	fireRate = 1;

	player.resetPlayerStats();
}

void GameEngine::RestartGame()
{
	SetState(State::PLAYING);
	// Reset the clock so there isn't a frame jump
	timerClock.restart();
}

void GameEngine::SetState(State newState)
{
	state = newState;
}

bool GameEngine::CheckState(State targetState)
{
	return state == targetState;
}

/// <summary>
/// Spin and zoom the world
/// </summary>
void GameEngine::UpdateCameraInput()
{
	if (Keyboard::isKeyPressed(Keyboard::Left))
	{
		mainView->rotate(.5f);
	}

	if (Keyboard::isKeyPressed(Keyboard::Right))
	{
		mainView->rotate(-.5f);
	}

	if (Keyboard::isKeyPressed(Keyboard::Up))
	{
		mainView->zoom(.99f);
	}

	if (Keyboard::isKeyPressed(Keyboard::Down))
	{
		mainView->zoom(1.01f);
	}
}

void GameEngine::HandleReloading()
{
	if (bulletsSpare <= 0)
	{
		reloadFailed.play();

		return;
	}

	int bulletsToLead = clipSize - bulletsInClip;
	int bulletsToDeduct = (bulletsSpare >= bulletsToLead) ? bulletsToLead : bulletsSpare;

	bulletsSpare -= bulletsToDeduct;
	bulletsInClip += bulletsToDeduct;

	reload.play();
}

void GameEngine::UpdateReloadInput(Event event)
{
	if (CheckState(State::PLAYING) && event.key.code == Keyboard::R)
		HandleReloading();
}

void GameEngine::UpdateGameStateInput(Event event)
{
	if (event.type != Event::KeyPressed)
		return;

	if (event.key.code == Keyboard::Return && CheckState(State::PLAYING))
		SetState(state = State::PAUSED);
	else if (event.key.code == Keyboard::Return && CheckState(State::PAUSED))
		RestartGame();
	else if (event.key.code == Keyboard::Return && CheckState(State::GAME_OVER))
		PrepareNextGame();
}

void GameEngine::HandleFireInput()
{
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		return;

	if (playTime.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / fireRate && bulletsInClip > 0)
	{
		// Pass the centre of the player and the centre of the crosshair
		// to the shoot function
		bullets[currentBullet].shoot(
			player.getCenter().x, player.getCenter().y,
			mouseWorldPosition.x, mouseWorldPosition.y);

		currentBullet++;
		if (currentBullet > 99)
			currentBullet = 0;

		lastPressed = playTime;
		shoot.play();
		bulletsInClip--;
	}
}

void GameEngine::HandlePlayerMoveInput()
{
	if (Keyboard::isKeyPressed(Keyboard::W))
		player.moveUp();
	else
		player.stopUp();

	if (Keyboard::isKeyPressed(Keyboard::S))
		player.moveDown();
	else
		player.stopDown();

	if (Keyboard::isKeyPressed(Keyboard::A))
		player.moveLeft();
	else
		player.stopLeft();

	if (Keyboard::isKeyPressed(Keyboard::D))
		player.moveRight();
	else
		player.stopRight();
}

void GameEngine::HandlePlayerInput()
{
	if (!CheckState(State::PLAYING))
		return;

	HandlePlayerMoveInput();
	HandleFireInput();
}

void GameEngine::UpdateLevelUpState(Event event)
{
	if (!CheckState(State::LEVELING_UP))
		return;

	HandleUpgradeSelection(event);

	if (!CheckState(State::PLAYING))
		return;

	ReinitializeTheLevel();
}

void GameEngine::UpgradeFireRate()
{
	fireRate++;
	SetState(State::PLAYING);
}

void GameEngine::UpgradeClipSize()
{
	clipSize += clipSize;
	SetState(State::PLAYING);
}

void GameEngine::UpgradeHealth()
{
	player.upgradeHealth();
	SetState(State::PLAYING);
}

void GameEngine::UpgradeSpeed()
{
	player.upgradeSpeed();
	SetState(State::PLAYING);
}

void GameEngine::UpgradeHealthPickup()
{
	healthPickup->upgrade();
	SetState(State::PLAYING);
}

void GameEngine::UpgradeAmmoPickup()
{
	ammoPickup->upgrade();
	SetState(State::PLAYING);
}

void GameEngine::HandleUpgradeSelection(Event event)
{
	if (event.key.code == Keyboard::Num1)
		UpgradeFireRate();

	if (event.key.code == Keyboard::Num2)
		UpgradeClipSize();

	if (event.key.code == Keyboard::Num3)
		UpgradeHealth();

	if (event.key.code == Keyboard::Num4)
		UpgradeSpeed();

	if (event.key.code == Keyboard::Num5)
		UpgradeHealthPickup();

	if (event.key.code == Keyboard::Num6)
		UpgradeAmmoPickup();
}

void GameEngine::ReinitializeTheLevel()
{
	wave++;

	// Prepare thelevel
	// We will modify the next two lines later
	arena.width = 500 * wave;
	arena.height = 500 * wave;
	arena.left = 0;
	arena.top = 0;

	// Pass the vertex array by reference 
	// to the createBackground function
	int tileSize = createBackground(background, arena);

	// Spawn the player in the middle of the arena
	player.spawn(arena, resolution, tileSize);

	// Configure the pick-ups
	healthPickup->setArena(arena);
	ammoPickup->setArena(arena);

	// Create a horde of zombies
	numZombies = 5 * wave;

	// Delete the previously allocated memory (if it exists)
	delete[] zombies;
	zombies = createHorde(numZombies, arena);
	numZombiesAlive = numZombies;

	// Play the powerup sound
	powerup.play();

	// Reset the clock so there isn't a frame jump
	timerClock.restart();
}

void GameEngine::UpdateInput()
{
	Event event;

	while (window->pollEvent(event))
		UpdateInput(event);

	if (Keyboard::isKeyPressed(Keyboard::Escape))
		window->close();

	HandlePlayerInput();
	UpdateLevelUpState(event);
}

void GameEngine::UpdateScene()
{
	if (state != State::PLAYING)
		return;

	// Update the delta time
	dt = timerClock.restart();
	// Update the total game time
	playTime += dt;
	// Make a decimal fraction of 1 from the delta time
	float dtAsSeconds = dt.asSeconds();

	// Where is the mouse pointer
	mouseScreenPosition = Mouse::getPosition();

	// Convert mouse position to world coordinates of mainView
	mouseWorldPosition = window->mapPixelToCoords(
		Mouse::getPosition(), *mainView);

	// Set the crosshair to the mouse world location
	spriteCrosshair.setPosition(mouseWorldPosition);

	// Update the player
	player.update(dtAsSeconds, Mouse::getPosition());

	// Make a note of the players new position
	Vector2f playerPosition(player.getCenter());

	// Make the view centre around the player				
	mainView->setCenter(player.getCenter());

	// Loop through each Zombie and update them
	for (int i = 0; i < numZombies; i++)
		if (zombies[i].isAlive())
			zombies[i].update(dt.asSeconds(), playerPosition);

	// Update any bullets that are in-flight
	for (int i = 0; i < 100; i++)
		if (bullets[i].isInFlight())
			bullets[i].update(dtAsSeconds);

	// Update the pickups
	healthPickup->update(dtAsSeconds);
	ammoPickup->update(dtAsSeconds);

	HandlePhysics();
	UpdateUI();
}

void GameEngine::HandlePhysics()
{
	HandlePlayerPhysics();
	HandleZombiesPhysics();
	PickupablesPhysics();
}

void GameEngine::HandlePlayerPhysics()
{
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < numZombies; j++)
		{
			if (bullets[i].isInFlight() && zombies[j].isAlive())
			{
				if (bullets[i].getPosition().intersects
				(zombies[j].getPosition()))
				{
					// Stop the bullet
					bullets[i].stop();

					// Register the hit and see if it was a kill
					if (zombies[j].hit()) {
						// Not just a hit but a kill too
						score += 10;
						if (score >= hiScore)
							hiScore = score;

						numZombiesAlive--;

						// When all the zombies are dead (again)
						if (numZombiesAlive == 0)
							SetState(State::LEVELING_UP);
					}

					splat.play();
				}
			}

		}
	}
}

void GameEngine::HandleZombiesPhysics()
{
	for (int i = 0; i < numZombies; i++)
	{
		if (player.getPosition().intersects
		(zombies[i].getPosition()) && zombies[i].isAlive())
		{

			if (player.hit(playTime))
			{
				// More here later
				hitSound.play();
			}

			if (player.getHealth() <= 0)
			{
				state = State::GAME_OVER;
				std::ofstream outputFile("gamedata/scores.txt");
				outputFile << hiScore;
				outputFile.close();
			}
		}
	}// End player touched
}

void GameEngine::PickupablesPhysics()
{
	// Has the player touched health pickup
	if (player.getPosition().intersects
	(healthPickup->getPosition()) && healthPickup->isSpawned())
	{
		player.increaseHealthLevel(healthPickup->gotIt());
		// Play a sound
		pickup.play();

	}

	// Has the player touched ammo pickup
	if (player.getPosition().intersects
	(ammoPickup->getPosition()) && ammoPickup->isSpawned())
	{
		bulletsSpare += ammoPickup->gotIt();
		// Play a sound
		reload.play();
	}
}

void GameEngine::UpdateUI()
{
	// size up the health bar
	healthBar.setSize(Vector2f(player.getHealth() * 3, 70));

	// Increment the amount of time since the last HUD update
	timeSinceLastUpdate += dt;
	// Increment the number of frames since the last HUD calculation
	framesSinceLastHUDUpdate++;
	// Calculate FPS every fpsMeasurementFrameInterval frames
	if (framesSinceLastHUDUpdate <= fpsMeasurementFrameInterval)
		return;

	UpdateHUD();
}

void GameEngine::UpdateHUD()
{

	// Update game HUD text
	std::stringstream ssAmmo;
	std::stringstream ssScore;
	std::stringstream ssHiScore;
	std::stringstream ssWave;
	std::stringstream ssZombiesAlive;

	// Update the ammo text
	ssAmmo << bulletsInClip << "/" << bulletsSpare;
	ammoText.setString(ssAmmo.str());

	// Update the score text
	ssScore << "Score:" << score;
	scoreText.setString(ssScore.str());

	// Update the high score text
	ssHiScore << "High Score:" << hiScore;
	hiScoreText.setString(ssHiScore.str());

	// Update the wave
	ssWave << "Wave:" << wave;
	waveNumberText.setString(ssWave.str());

	// Update the high score text
	ssZombiesAlive << "Zombies:" << numZombiesAlive;
	zombiesRemainingText.setString(ssZombiesAlive.str());

	framesSinceLastHUDUpdate = 0;
	timeSinceLastUpdate = Time::Zero;
}

void GameEngine::DrawScene()
{
	DrawOnPlayState();
	DrawOnLevelingUpState();
	DrawOnPausedState();
	DrawOnGameOverState();
}

void GameEngine::DrawOnPlayState()
{
	if (!CheckState(State::PLAYING))
		return;

	window->clear();
	window->setView(*mainView);
	window->draw(background, &textureBackground);

	// Draw the zombies
	for (int i = 0; i < numZombies; i++)
		window->draw(zombies[i].getSprite());

	for (int i = 0; i < 100; i++)
		if (bullets[i].isInFlight())
			window->draw(bullets[i].getShape());

	// Draw the player
	window->draw(player.getSprite());

	// Draw the pickups is currently spawned
	if (ammoPickup->isSpawned())
		window->draw(ammoPickup->getSprite());

	if (healthPickup->isSpawned())
		window->draw(healthPickup->getSprite());

	//Draw the crosshair
	window->draw(spriteCrosshair);

	// Switch to the HUD view
	window->setView(*hudView);

	// Draw all the HUD elements
	DrawHUD();
	
}

void GameEngine::DrawOnLevelingUpState()
{
	if (!CheckState(State::LEVELING_UP))
		return;

	window->draw(spriteGameOver);
	window->draw(levelUpText);
}

void GameEngine::DrawOnPausedState()
{
	if (!CheckState(State::PAUSED))
		return;

	window->draw(pausedText);
}

void GameEngine::DrawOnGameOverState()
{
	if (!CheckState(State::GAME_OVER))
		return;

	window->draw(spriteGameOver);
	window->draw(gameOverText);
	window->draw(scoreText);
	window->draw(hiScoreText);
}

void GameEngine::DrawHUD()
{
	window->draw(spriteAmmoIcon);
	window->draw(ammoText);
	window->draw(scoreText);
	window->draw(hiScoreText);
	window->draw(healthBar);
	window->draw(waveNumberText);
	window->draw(zombiesRemainingText);
}

void GameEngine::InitializeAssets()
{
	font.loadFromFile("fonts/OpenSans-Regular.ttf");
	InitializeCrosshair();
	InitializeBackground();
	LoadHighscoreFile();
	InitializeUIAmmo();
	InitUI();
	InitializeZombies();
	InitializeWaves();
	InitializeHealthBar();
	InitializeAudio();
}

void GameEngine::InitEngine()
{
}

void GameEngine::InitPickups()
{

}

void GameEngine::InitializeAudio()
{
	hitBuffer.loadFromFile("sound/hit.wav");
	hitSound.setBuffer(hitBuffer);

	// Prepare the splat sound
	splatBuffer.loadFromFile("sound/splat.wav");
	splat.setBuffer(splatBuffer);

	// Prepare the shoot sound
	shootBuffer.loadFromFile("sound/shoot.mp3");
	shoot.setBuffer(shootBuffer);

	// Prepare the reload sound
	reloadBuffer.loadFromFile("sound/reload.wav");
	reload.setBuffer(reloadBuffer);

	// Prepare the failed sound
	reloadFailedBuffer.loadFromFile("sound/reload_failed.wav");
	reloadFailed.setBuffer(reloadFailedBuffer);

	// Prepare the powerup sound
	powerupBuffer.loadFromFile("sound/powerup.wav");
	powerup.setBuffer(powerupBuffer);

	// Prepare the pickup sound
	pickupBuffer.loadFromFile("sound/pickup.wav");
	pickup.setBuffer(pickupBuffer);

	ambientBuffer.loadFromFile("sound/ambientmain_0.ogg");
	ambient.setBuffer(ambientBuffer);
	ambient.setLoop(true);
	ambient.play();
}

void GameEngine::InitUIHighScore()
{
	hiScoreText.setFont(font);
	hiScoreText.setCharacterSize(55);
	hiScoreText.setFillColor(Color::White);
	hiScoreText.setPosition(1400, 0);
	std::stringstream s;
	s << "High Score:" << hiScore;
	hiScoreText.setString(s.str());
}

void GameEngine::LoadHighscoreFile()
{
	std::ifstream inputFile("gamedata/scores.txt");
	if (inputFile.is_open())
	{
		inputFile >> hiScore;
		inputFile.close();
	}
}

void GameEngine::InitUIScore()
{
	scoreText.setFont(font);
	scoreText.setCharacterSize(55);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(20, 0);
}

void GameEngine::InitUIAmmo()
{
	ammoText.setFont(font);
	ammoText.setCharacterSize(55);
	ammoText.setFillColor(Color::White);
	ammoText.setPosition(200, 980);

}

void GameEngine::InitUIState_LevelUp()
{
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(80);
	levelUpText.setFillColor(Color::White);
	levelUpText.setPosition(150, 250);
	std::stringstream levelUpStream;
	levelUpStream <<
		"1- Increased rate of fire" <<
		"\n2- Increased clip size(next reload)" <<
		"\n3- Increased max health" <<
		"\n4- Increased run speed" <<
		"\n5- More and better health pickups" <<
		"\n6- More and better ammo pickups";
	levelUpText.setString(levelUpStream.str());
}

void GameEngine::InitUIState_GameOver()
{
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(125);
	gameOverText.setFillColor(Color::White);
	gameOverText.setPosition(250, 850);
	gameOverText.setString("Press Enter to play");
}

void GameEngine::InitUIState_Paused()
{
	pausedText.setFont(font);
	pausedText.setCharacterSize(155);
	pausedText.setFillColor(Color::White);
	pausedText.setPosition(400, 400);
	pausedText.setString("Press Enter \nto continue");
}

void GameEngine::InitializeUIAmmo()
{
	spriteAmmoIcon.setTexture(textureAmmoIcon);
	spriteAmmoIcon.setPosition(20, 980);
}

void GameEngine::InitializeWaves()
{
	waveNumberText.setFont(font);
	waveNumberText.setCharacterSize(55);
	waveNumberText.setFillColor(Color::White);
	waveNumberText.setPosition(1250, 980);
	waveNumberText.setString("Wave: 0");
}

void GameEngine::InitializeZombies()
{
	zombiesRemainingText.setFont(font);
	zombiesRemainingText.setCharacterSize(55);
	zombiesRemainingText.setFillColor(Color::White);
	zombiesRemainingText.setPosition(1500, 980);
	zombiesRemainingText.setString("Zombies: 100");
}

void GameEngine::InitializeBackground()
{
	spriteGameOver.setTexture(textureGameOver);
	spriteGameOver.setPosition(0, 0);
}

void GameEngine::InitializeCrosshair()
{
	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(16, 16);
}

void GameEngine::InitUI()
{
	InitUIState_Paused();
	InitUIState_GameOver();
	InitUIState_LevelUp();
	InitUIAmmo();
	InitUIScore();
	InitUIHighScore();
}

GameEngine::~GameEngine()
{
}

void GameEngine::InitializeHealthBar()
{
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition(450, 980);
}

void GameEngine::Run()
{
	while (window->isOpen())
	{
		UpdateInput();
		UpdateScene();
		DrawScene();
		window->display();
	}
}