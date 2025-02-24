#ifndef PLAYER_H
#define PLAYER_H

#include "CollisionManager.h"

#include <ncine/Sprite.h>
#include <ncine/TimeStamp.h>

namespace nc = ncine;

class RocketPool;

/// The class the handles the logic behind the ship controlled by the player
class Player
{
  public:
	Player(ProjectilePool *rocketPool, nc::Texture *playerTexture);

	/// Move the player by an amount specified by the time passed since last frame
	void move(float frameTime);
	void shoot();

	inline unsigned int lives() const { return lives_; }
	void damage(unsigned int amount) { lives_ -= amount; }

	inline unsigned int score() const { return score_; }
	void increaseScore(unsigned int amount) { score_ += amount; }
	void resetScore() { score_ = 0; }

	void reset();

  private:
	unsigned int lives_;
	unsigned int score_;

	ProjectilePool *rocketPool_;

	nc::Sprite sprite_;
	nc::TimeStamp lastShootTime_;

	Player(const Player &) = delete;
	Player &operator=(const Player &) = delete;

	friend bool CollisionManager::performCollisions(Player *player, ProjectilePool *bombPool);
	friend bool CollisionManager::performCollisions(Player *player, EnemyPool *enemyPool);
};

#endif
