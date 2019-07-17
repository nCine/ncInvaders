#ifndef _COLLISION_MANAGER_H
#define _COLLISION_MANAGER_H

class ProjectilePool;
class EnemyPool;
class Player;

/// Collection of functions that check for collisions between entities in the game
/*! \note Assuming no more than one collision per frame per function. */
namespace CollisionManager {

/// Performs the collision detection between rockets and enemies
/*! \warning It relies on a the bounding box of alive enemies and thus it needs to be called after `EnemyPool::update()` */
/*! \returns `true` if a collision has been detected */
bool performCollisions(ProjectilePool *rocketPool, EnemyPool *enemyPool);

/// Performs the collision detection between the player and bombs
/*! \returns `true` if a collision has been detected */
bool performCollisions(Player *player, ProjectilePool *bombPool);

/// Performs the collision detection between the player and enemies
/*! \returns `true` if a collision has been detected */
bool performCollisions(Player *player, EnemyPool *enemyPool);

};

#endif
