#include "player.h"
#include "TextureHolder.h"
#include <iostream>
#include "Configuration.hpp"
#include <utility> //forward

Player::Player() : ActionTarget(Configuration::playerInputs)
{
	m_Speed = START_SPEED;
	m_Health = START_HEALTH;
	m_MaxHealth = START_HEALTH;

	m_Sprite = Sprite(Configuration::textures.get(Configuration::Textures::Player));

	m_Sprite.setScale(Vector2f(0.25f, 0.25f));
	m_Sprite.setOrigin(120, 120);

	shoot.setBuffer(Configuration::soundBuffers.get(Configuration::SoundEffect::Shoot));
	reload.setBuffer(Configuration::soundBuffers.get(Configuration::SoundEffect::Reload));
	reloadFailed.setBuffer(Configuration::soundBuffers.get(Configuration::SoundEffect::Reload_Failed));
	powerup.setBuffer(Configuration::soundBuffers.get(Configuration::SoundEffect::Powerup));

	bind(Configuration::PlayerInputs::Left, [this](const sf::Event&) {
		moveLeft();
		});

	bind(Configuration::PlayerInputs::Right, [this](const sf::Event&) {
		moveRight();
		});

	bind(Configuration::PlayerInputs::Down, [this](const sf::Event&) {
		moveDown();
		});

	bind(Configuration::PlayerInputs::Up, [this](const sf::Event&) {
		moveUp();
		});

	bind(Configuration::PlayerInputs::Fire, [this](const sf::Event&) {
		StartFireWeaon();
		});
}

void Player::processEvents()
{
	StopFireWeapon();
	stopDown();
	stopUp();
	stopLeft();
	stopRight();
	ActionTarget::processEvents();
}

void Player::resetPlayerStats()
{
	m_Speed = START_SPEED;
	m_Health = START_HEALTH;
	m_MaxHealth = START_HEALTH;

	ResetWeaponStats();
}

void Player::ResetWeaponStats()
{
	currentBullet = 0;
	bulletsSpare = 24;
	bulletsInClip = 6;
	clipSize = 6;
	fireRate = 1;
}

void Player::StartFireWeaon()
{
	m_IsShooting = true;

}

void Player::StopFireWeapon()
{
	m_IsShooting = false;
}

bool Player::UpdateWeapon(float delta, sf::Vector2f mousePos)
{
	if (!m_IsShooting)
		return false;

	if (bulletsInClip <= 0)
	{
		HandleReloading();
		return false;
	}

	if (!(delta > 1000 / fireRate && bulletsInClip > 0))
		return false;

	bullets[currentBullet].shoot(
		getCenter().x, getCenter().y,
		mousePos.x, mousePos.y);

	currentBullet++;
	if (currentBullet > 99)
		currentBullet = 0;

	shoot.play();
	bulletsInClip--;

	return true;
}
std::string Player::UpdateHUD()
{
	std::stringstream ssAmmo;
	ssAmmo << bulletsInClip << "/" << bulletsSpare;
	return ssAmmo.str();
}

void Player::AddAmmo(int ammo) {
	bulletsSpare += ammo;
	reload.play();
}

void Player::HandleReloading()
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

void Player::spawn(IntRect arena, Vector2f resolution, int tileSize)
{
	// Place the player in the middle of the arena
	m_Position.x = arena.width / 2;
	m_Position.y = arena.height / 2;

	// Copy the details of the arena to the player's m_Arena
	m_Arena.left = arena.left;
	m_Arena.width = arena.width;
	m_Arena.top = arena.top;
	m_Arena.height = arena.height;

	// Remember how big the tiles are in this arena
	m_TileSize = tileSize;

	// Strore the resolution for future use
	m_Resolution.x = resolution.x;
	m_Resolution.y = resolution.y;
}

Time Player::getLastHitTime()
{
	return m_LastHit;
}

bool Player::hit(Time timeHit)
{
	if (timeHit.asMilliseconds() - m_LastHit.asMilliseconds() > 200)// 2 tenths of second
	{
		m_LastHit = timeHit;
		m_Health -= 10;
		return true;
	}
	else
	{
		return false;
	}

}

FloatRect Player::getPosition()
{
	return m_Sprite.getGlobalBounds();
}

Vector2f Player::getCenter()
{
	return m_Position;
}

float Player::getRotation()
{
	return m_Sprite.getRotation();
}

Sprite Player::getSprite()
{
	return m_Sprite;
}

int Player::getHealth()
{
	return m_Health;
}

void Player::moveLeft()
{
	m_LeftPressed = true;
}

void Player::moveRight()
{
	m_RightPressed = true;
}

void Player::moveUp()
{
	m_UpPressed = true;
}

void Player::moveDown()
{
	m_DownPressed = true;
}

void Player::stopLeft()
{
	m_LeftPressed = false;
}

void Player::stopRight()
{
	m_RightPressed = false;
}

void Player::stopUp()
{
	m_UpPressed = false;
}

void Player::stopDown()
{
	m_DownPressed = false;
}

void Player::update(float elapsedTime, Vector2i mousePosition)
{
	UpdateProjectiles(elapsedTime);

	if (m_UpPressed)
	{
		m_Position.y -= m_Speed * elapsedTime;
	}

	if (m_DownPressed)
	{
		m_Position.y += m_Speed * elapsedTime;
	}

	if (m_RightPressed)
	{
		m_Position.x += m_Speed * elapsedTime;
	}

	if (m_LeftPressed)
	{
		m_Position.x -= m_Speed * elapsedTime;
	}

	m_Sprite.setPosition(m_Position);

	// Keep the player in the arena
	if (m_Position.x > m_Arena.width - m_TileSize)
	{
		m_Position.x = m_Arena.width - m_TileSize;
	}

	if (m_Position.x < m_Arena.left + m_TileSize)
	{
		m_Position.x = m_Arena.left + m_TileSize;
	}

	if (m_Position.y > m_Arena.height - m_TileSize)
	{
		m_Position.y = m_Arena.height - m_TileSize;
	}

	if (m_Position.y < m_Arena.top + m_TileSize)
	{
		m_Position.y = m_Arena.top + m_TileSize;
	}

	// Calculate the angle the player is facing
	float angle = (atan2(mousePosition.y - m_Resolution.y / 2,
		mousePosition.x - m_Resolution.x / 2)
		* 180) / 3.141;

	m_Sprite.setRotation(angle);
}

void Player::UpdateProjectiles(float& elapsedTime)
{
	for (int i = 0; i < 100; i++)
		if (bullets[i].isInFlight())
			bullets[i].update(elapsedTime);
}

void Player::upgradeSpeed()
{
	// 20% speed upgrade
	m_Speed += (START_SPEED * .2);
}

void Player::upgradeHealth()
{
	// 20% max health upgrade
	m_MaxHealth += (START_HEALTH * .2);
}

void Player::UpgradeFireRate()
{
	fireRate++;
}

void Player::UpgradeClipSize()
{
	clipSize += clipSize;
}

void Player::increaseHealthLevel(int amount)
{
	m_Health += amount;

	// But not beyond the maximum
	if (m_Health > m_MaxHealth)
	{
		m_Health = m_MaxHealth;
	}
}

