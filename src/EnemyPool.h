#ifndef ENEMY_POOL_H
#define ENEMY_POOL_H

#include "SpritePool.h"
#include "CollisionManager.h"

#include <ncine/Texture.h>
#include <ncine/TimeStamp.h>

namespace nc = ncine;

class ProjectilePool;

/// The class that handles enemy sprites and their update logic
class EnemyPool
{
  public:
	/*! \param maxSize Maximum number of enemies allowed */
	/*! \note Number of enemies on screen might be less than \p maxSize as they are organized in a grid taking aspect ratio into account */
	EnemyPool(unsigned int maxSize, ProjectilePool *bombPool, nc::Texture *enemyTextureOne, nc::Texture *enemyTextureTwo);

	void reset();

	/// Updates positions, transitions states and calculates the bounding box
	void update(float frameTime);
	/*! \note This is the only drawing method that contains logic to use more than a single sprite for pool entities */
	void draw();

	/// Checks if the enemies have reached the bottom of the window
	bool reachedBottom() const;
	/// Checks if the player has killed all enemies
	inline bool isEmpty() const { return enemies_.acquiredSize() == 0; }

  private:
	enum EnemyState
	{
		MOVE_RIGHT,
		MOVE_LEFT,
		MOVE_DOWNRIGHT,
		MOVE_DOWNLEFT
	};

	unsigned int size_;
	int width_;
	SpritePool enemies_;
	EnemyState state_;

	float xMin_, xMax_;
	float yMin_, yMax_;

	float horizontalSpeed_;
	float verticalSpeed_;

	ProjectilePool *bombPool_;

	nc::Texture *enemyTextureOne_;
	nc::Texture *enemyTextureTwo_;
	nc::TimeStamp lastShootTime_;

	EnemyPool(const EnemyPool &) = delete;
	EnemyPool &operator=(const EnemyPool &) = delete;

	friend bool CollisionManager::performCollisions(ProjectilePool *rocketPool, EnemyPool *enemyPool);
	friend bool CollisionManager::performCollisions(Player *player, EnemyPool *enemyPool);
};

#endif
