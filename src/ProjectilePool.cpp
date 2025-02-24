#include "Configuration.h"
#include "ProjectilePool.h"

#include <ncine/Application.h>
#include <ncine/Sprite.h>

ProjectilePool::ProjectilePool(unsigned int maxSize, nc::Texture *texture)
    : projectiles_(maxSize, texture)
{
	reset();
}

bool ProjectilePool::shoot(float x, float y)
{
	nc::Sprite *projectile = projectiles_.acquire();

	if (projectile)
		projectile->setPosition(x, y);

	// Was a projectile found and dropped?
	return (projectile != nullptr);
}

void ProjectilePool::updateBombs(float frameTime)
{
	// Traverse the array backwards to release sprites
	for (int i = projectiles_.acquiredSize() - 1; i >= 0; i--)
	{
		projectiles_[i].moveY(-roundf(frameTime * Conf::BombSpeed));
		if (projectiles_[i].position().y + projectiles_.spriteHeight() * 0.5f < 0.0f)
			projectiles_.release(i);
	}
}

void ProjectilePool::updateRockets(float frameTime)
{
	// Traverse the array backwards to release sprites
	for (int i = projectiles_.acquiredSize() - 1; i >= 0; i--)
	{
		projectiles_[i].moveY(roundf(frameTime * Conf::RocketSpeed));
		if (projectiles_[i].position().y - projectiles_.spriteHeight() * 0.5f > nc::theApplication().height())
			projectiles_.release(i);
	}
}

void ProjectilePool::draw()
{
	for (unsigned int i = 0; i < projectiles_.acquiredSize(); i++)
		projectiles_[i].setEnabled(true);
	for (unsigned int i = projectiles_.acquiredSize(); i < projectiles_.totalSize(); i++)
		projectiles_[i].setEnabled(false);
}

void ProjectilePool::reset()
{
	for (unsigned int i = 0; i < projectiles_.totalSize(); i++)
		projectiles_[i].setEnabled(false);
	projectiles_.releaseAll();
}
