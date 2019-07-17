#ifndef PROJECTILE_POOL_H
#define PROJECTILE_POOL_H

#include "SpritePool.h"
#include "CollisionManager.h"

namespace nc = ncine;

/// The class that handles projectile sprites and their update logic
class ProjectilePool
{
  public:
	ProjectilePool(unsigned int maxSize, nc::Texture *texture);

	/// Attempts to shoot a projectile from the pool
	/*! \returns `false` if none is available */
	bool shoot(float x, float y);

	/// Updates positions and returns to the pool a bomb that goes beyond the bottom of the window
	void updateBombs(float interval);
	/// Updates positions and returns to the pool a rocket that goes beyond the top of the window
	void updateRockets(float interval);

	void draw();
	void reset();

  private:
	SpritePool projectiles_;

	ProjectilePool(const ProjectilePool &) = delete;
	ProjectilePool &operator=(const ProjectilePool &) = delete;

	friend bool CollisionManager::performCollisions(ProjectilePool *rocketPool, EnemyPool *enemyPool);
	friend bool CollisionManager::performCollisions(Player *player, ProjectilePool *bombPool);
};

#endif
