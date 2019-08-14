#include "Configuration.h"
#include "Player.h"
#include "ProjectilePool.h"

#include <ncine/Application.h>
#include <ncine/IInputManager.h>

Player::Player(ProjectilePool *rocketPool, nc::Texture *playerTexture)
    : lives_(0), score_(0), rocketPool_(rocketPool), sprite_(&nc::theApplication().rootNode(), playerTexture)
{
	ASSERT(rocketPool_);
	reset();
}

void Player::move(float interval)
{
	sprite_.x += roundf(interval * Conf::PlayerSpeed);

	// Clamping to window horizontal borders
	if (sprite_.x < 0.0f)
		sprite_.x = 0.0f;
	else if (sprite_.x >= nc::theApplication().width() - sprite_.width())
		sprite_.x = nc::theApplication().width() - sprite_.width();
}

void Player::shoot()
{
	// Check if enough time has passed before shooting again
	if (lastShootTime_.secondsSince() >= Conf::PlayerShootTime)
	{
		rocketPool_->shoot(sprite_.x, sprite_.y + sprite_.height() * 0.5f);
		lastShootTime_ = nc::TimeStamp::now();
	}
}

void Player::reset()
{
	// Centered at the bottom of the window
	sprite_.x = (nc::theApplication().width() - sprite_.width()) * 0.5f;
	sprite_.y = sprite_.height();

	lives_ = Conf::PlayerStartingLives;

	// Ready to shoot
	lastShootTime_ = nc::TimeStamp::now();
}
