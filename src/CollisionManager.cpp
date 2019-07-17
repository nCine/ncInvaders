#include "CollisionManager.h"
#include "ProjectilePool.h"
#include "EnemyPool.h"
#include "Player.h"
#include "Configuration.h"

#include <ncine/common_macros.h>

bool CollisionManager::performCollisions(ProjectilePool *rocketPool, EnemyPool *enemyPool)
{
	ASSERT(rocketPool);
	ASSERT(enemyPool);

	SpritePool &rockets = rocketPool->projectiles_;
	SpritePool &enemies = enemyPool->enemies_;
	const nc::Vector2f halfRocketSize(rockets.spriteWidth() * 0.5f, rockets.spriteHeight() * 0.5f);
	const nc::Vector2f halfEnemySize(enemies.spriteWidth() * 0.5f, enemies.spriteHeight() * 0.5f);

	for (int rocketIdx = rockets.acquiredSize() - 1; rocketIdx >= 0; rocketIdx--)
	{
		const nc::Sprite &r = rockets[rocketIdx];

		// Skip check for rockets outside the bounding box of alive enemies
		if (r.x + halfRocketSize.x < enemyPool->xMin_ ||
		    r.x - halfRocketSize.x > enemyPool->xMax_ ||
		    r.y + halfRocketSize.y < enemyPool->yMin_ ||
		    r.y - halfRocketSize.y > enemyPool->yMax_)
		{
			continue;
		}

		for (int enemyIdx = enemies.acquiredSize() - 1; enemyIdx >= 0; enemyIdx--)
		{
			const nc::Sprite &e = enemies[enemyIdx];

			if (r.x + halfRocketSize.x >= e.x - halfEnemySize.x && r.x - halfRocketSize.x <= e.x + halfEnemySize.x &&
			    r.y + halfRocketSize.y >= e.y - halfEnemySize.y && r.y - halfRocketSize.y <= e.y + halfEnemySize.y)
			{
				rockets.release(rocketIdx);
				enemies.release(enemyIdx);
				return true; // collision detected
			}
		}
	}

	return false; // no collisions
}

bool CollisionManager::performCollisions(Player *player, ProjectilePool *bombPool)
{
	ASSERT(player);
	ASSERT(bombPool);

	SpritePool &bombs = bombPool->projectiles_;
	const nc::Sprite &p = player->sprite_;
	const nc::Vector2f halfPlayerSize(p.width() * 0.5f, p.height() * 0.5f);
	const nc::Vector2f halfBombSize(bombs.spriteWidth() * 0.5f, bombs.spriteHeight() * 0.5f);

	for (int bombIdx = bombs.acquiredSize() - 1; bombIdx >= 0; bombIdx--)
	{
		const nc::Sprite &b = bombs[bombIdx];

		if (p.x + halfPlayerSize.x >= b.x - halfBombSize.x && p.x - halfPlayerSize.x <= b.x + halfBombSize.x &&
		    p.y + halfPlayerSize.y >= b.y - halfBombSize.y && p.y - halfPlayerSize.y <= b.y + halfBombSize.y)
		{
			bombs.release(bombIdx);
			return true; // collision detected
		}
	}

	return false; // no collisions
}

bool CollisionManager::performCollisions(Player *player, EnemyPool *enemyPool)
{
	ASSERT(player);
	ASSERT(enemyPool);

	SpritePool &enemies = enemyPool->enemies_;
	const nc::Sprite &p = player->sprite_;
	const nc::Vector2f halfPlayerSize(p.width() * 0.5f, p.height() * 0.5f);
	const nc::Vector2f halfEnemySize(enemies.spriteWidth() * 0.5f, enemies.spriteHeight() * 0.5f);

	// Skip check if the player is outside the bounding box of alive enemies
	if (p.x + halfPlayerSize.x < enemyPool->xMin_ ||
	    p.x - halfPlayerSize.x > enemyPool->xMax_ ||
	    p.y + halfPlayerSize.y < enemyPool->yMin_ ||
	    p.y - halfPlayerSize.y > enemyPool->yMax_)
	{
		return false; // no collisions
	}

	for (int enemyIdx = enemies.acquiredSize() - 1; enemyIdx >= 0; enemyIdx--)
	{
		const nc::Sprite &e = enemies[enemyIdx];

		if (p.x + halfPlayerSize.x >= e.x - halfEnemySize.x && p.x - halfPlayerSize.x <= e.x + halfEnemySize.x &&
		    p.y + halfPlayerSize.y >= e.y - halfEnemySize.y && p.y - halfPlayerSize.y <= e.y + halfEnemySize.y)
		{
			enemies.release(enemyIdx);
			return true; // collision detected
		}
	}

	return false; // no collisions
}
