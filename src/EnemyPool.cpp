#include "Configuration.h"
#include "EnemyPool.h"
#include "ProjectilePool.h"

#include <ncine/Random.h>
#include <ncine/Application.h>
#include <ncine/Sprite.h>

EnemyPool::EnemyPool(unsigned int maxSize, ProjectilePool *bombPool, nc::Texture *enemyTextureOne, nc::Texture *enemyTextureTwo)
    : size_(0), width_(0), enemies_(maxSize, enemyTextureOne), state_(MOVE_RIGHT),
      xMin_(0.0f), xMax_(0.0f), yMin_(0.0f), yMax_(0.0f),
      horizontalSpeed_(Conf::EnemyStartingHSpeed), verticalSpeed_(Conf::EnemyStartingVSpeed), bombPool_(bombPool),
      enemyTextureOne_(enemyTextureOne), enemyTextureTwo_(enemyTextureTwo)
{
	ASSERT(bombPool_);
	const float aspect = nc::theApplication().width() / nc::theApplication().height();
	width_ = static_cast<int>(sqrtf(static_cast<float>(maxSize) * aspect));
	const int height = static_cast<int>(width_ / aspect);

	size_ = static_cast<unsigned int>(width_ * height);
	ASSERT(size_ > 0);

	reset();
}

void EnemyPool::reset()
{
	horizontalSpeed_ = Conf::EnemyStartingHSpeed;
	verticalSpeed_ = Conf::EnemyStartingVSpeed;

	enemies_.reserve(size_);

	float paddingX = (nc::theApplication().width() * (1.0f - Conf::EnemyHMargin * 2.0f)) / width_;
	for (unsigned int i = 0; i < size_; i++)
	{
		enemies_[i].x = (Conf::EnemyHMargin * nc::theApplication().width()) + paddingX * (i % width_) + paddingX * 0.5f * (i / width_ % 2);
		enemies_[i].y = nc::theApplication().height() - enemies_[i].height() * (i / width_ + 0.5f); // Shift down vertically half the sprite size to make text readable
		enemies_[i].setTexture((i % 2) ? enemyTextureOne_ : enemyTextureTwo_);
		enemies_[i].setEnabled(true);
	}
	for (unsigned int i = size_; i < enemies_.totalSize(); i++)
		enemies_[i].setEnabled(false);

	// Ready to shoot
	shootTimer_.start();
}

void EnemyPool::update(float interval)
{
	// Bounding box of alive enemies
	xMin_ = nc::theApplication().width();
	xMax_ = 0.0f;
	yMin_ = nc::theApplication().height();
	yMax_ = 0.0f;

	// Check if enough time has passed before shooting again and randomize the shooter
	if (enemies_.acquiredSize() > 0 && shootTimer_.interval() >= Conf::EnemyShootTime)
	{
		const unsigned int shootIdx = nc::random().integer(0, enemies_.acquiredSize());
		bombPool_->shoot(enemies_[shootIdx].x, enemies_[shootIdx].y - enemies_[shootIdx].height() * 0.5f);
		shootTimer_.start();
	}

	const nc::Vector2f halfEnemySize(enemies_.spriteWidth() * 0.5f, enemies_.spriteHeight() * 0.5f);
	for (unsigned int i = 0; i < enemies_.acquiredSize(); i++)
	{
		// Update the bounding box
		if (enemies_[i].x - halfEnemySize.x < xMin_)
			xMin_ = enemies_[i].x - halfEnemySize.x;
		if (enemies_[i].x + halfEnemySize.x > xMax_)
			xMax_ = enemies_[i].x + halfEnemySize.x;
		if (enemies_[i].y - halfEnemySize.y < yMin_)
			yMin_ = enemies_[i].y - halfEnemySize.y;
		if (enemies_[i].y + halfEnemySize.y > yMax_)
			yMax_ = enemies_[i].y + halfEnemySize.y;

		switch (state_)
		{
			case MOVE_RIGHT:
			{
				enemies_[i].x += roundf(interval * horizontalSpeed_);
				break;
			}
			case MOVE_LEFT:
			{
				enemies_[i].x -= roundf(interval * horizontalSpeed_);
				break;
			}
			case MOVE_DOWNRIGHT:
			case MOVE_DOWNLEFT:
			{
				enemies_[i].y -= roundf(verticalSpeed_);
				horizontalSpeed_ *= Conf::EnemySpeedIncrease;
				verticalSpeed_ *= Conf::EnemySpeedIncrease;
				break;
			}
		}
	}

	// Check if a state transition is needed
	switch (state_)
	{
		case MOVE_RIGHT:
		{
			if (xMax_ >= nc::theApplication().width() - enemies_.spriteWidth() * 0.5f)
				state_ = MOVE_DOWNLEFT;
			break;
		}
		case MOVE_LEFT:
		{
			if (xMin_ <= enemies_.spriteWidth() * 0.5f)
				state_ = MOVE_DOWNRIGHT;
			break;
		}
		case MOVE_DOWNRIGHT:
		{
			state_ = MOVE_RIGHT;
			break;
		}
		case MOVE_DOWNLEFT:
		{
			state_ = MOVE_LEFT;
			break;
		}
	}
}

void EnemyPool::draw()
{
	for (unsigned int i = 0; i < enemies_.acquiredSize(); i++)
		enemies_[i].setEnabled(true);
	for (unsigned int i = enemies_.acquiredSize(); i < enemies_.totalSize(); i++)
		enemies_[i].setEnabled(false);
}

bool EnemyPool::reachedBottom() const
{
	return yMin_ >= nc::theApplication().height() - enemies_.spriteHeight();
}
