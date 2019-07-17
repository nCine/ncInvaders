#ifndef _CONFIGURATION_H
#define _CONFIGURATION_H

namespace Configuration {

// Sprite images
const char *const PlayerSpriteImage = "player.png";
const char *const RocketSpriteImage = "rocket.png";
const char *const BombSpriteImage = "bomb.png";
const char *const EnemySpriteOneImage = "enemy1.png";
const char *const EnemySpriteTwoImage = "enemy2.png";

// Screen text
#ifndef __ANDROID__
const char *const FontTextureFile = "DroidSans32_256.png";
#else
const char *const FontTextureFile = "DroidSans32_256.webp";
#endif
const char *const FontFntFile = "DroidSans32_256.fnt";

// Game class
const int MaxNumberEnemies = 50;

// Player class
const float PlayerSpeed = 160.0f;
const float PlayerShootTime = 1.0f;
const unsigned int PlayerStartingLives = 3;
const unsigned int PlayerDamage = 1;
const unsigned int PlayerKillScore = 10;

// RocketPool class
const unsigned int RocketPoolSize = 3;
const float RocketSpeed = 300.0f;

// BombPool class
const unsigned int BombPoolSize = 4;
const float BombSpeed = 100.0f;

// EnemyPool class
const float EnemyHMargin = 0.075f;
const float EnemyStartingHSpeed = 100.0f;
const float EnemyStartingVSpeed = 1.0f;
const float EnemySpeedIncrease = 1.00035f;
const float EnemyShootTime = 0.75f;

}

namespace Conf = Configuration;

#endif
