#include "Configuration.h"
#include "Game.h"
#include "ProjectilePool.h"
#include "EnemyPool.h"
#include "Player.h"

#include <ncine/Application.h>
#include <ncine/IInputManager.h>
#include <ncine/Font.h>
#include <ncine/TextNode.h>
#include <ncine/FileSystem.h>
#include <nctl/String.h>

Game::Game()
    : state_(GameState::START), screenString_(128), playerMovement_(PlayerMovement::STOP)
{
	bombTexture_ = nctl::makeUnique<nc::Texture>(nc::fs::joinPath(nc::fs::dataPath(), Conf::BombSpriteImage).data());
	enemy1Texture_ = nctl::makeUnique<nc::Texture>(nc::fs::joinPath(nc::fs::dataPath(), Conf::EnemySpriteOneImage).data());
	enemy2Texture_ = nctl::makeUnique<nc::Texture>(nc::fs::joinPath(nc::fs::dataPath(), Conf::EnemySpriteTwoImage).data());
	rocketTexture_ = nctl::makeUnique<nc::Texture>(nc::fs::joinPath(nc::fs::dataPath(), Conf::RocketSpriteImage).data());
	playerTexture_ = nctl::makeUnique<nc::Texture>(nc::fs::joinPath(nc::fs::dataPath(), Conf::PlayerSpriteImage).data());

	font_ = nctl::makeUnique<nc::Font>(nc::fs::joinPath(nc::fs::dataPath(), Conf::FontFntFile).data(),
	                                   nc::fs::joinPath(nc::fs::dataPath(), Conf::FontTextureFile).data());

	// Ignore inter-nodes drawing order to allow batching without a texture atlas
	nc::theApplication().rootNode().setVisitOrderState(nc::SceneNode::VisitOrderState::DISABLED);

	screenText_ = nctl::makeUnique<nc::TextNode>(&nc::theApplication().rootNode(), font_.get());
	screenText_->setLayer(1);
	screenText_->setPosition((nc::theApplication().width() - screenText_->width()) * 0.5f,
	                         nc::theApplication().height() - screenText_->lineHeight() * 0.5f * 2.0f);
	screenText_->setColor(255, 255, 0, 255);
	screenText_->setAlignment(nc::TextNode::Alignment::CENTER);

	bombPool_ = nctl::makeUnique<ProjectilePool>(Conf::BombPoolSize, bombTexture_.get());
	enemyPool_ = nctl::makeUnique<EnemyPool>(Conf::MaxNumberEnemies, bombPool_.get(), enemy1Texture_.get(), enemy2Texture_.get());
	rocketPool_ = nctl::makeUnique<ProjectilePool>(Conf::RocketPoolSize, rocketTexture_.get());
	player_ = nctl::makeUnique<Player>(rocketPool_.get(), playerTexture_.get());
}

Game::~Game() = default;

void Game::update(float interval)
{
	switch (state_)
	{
		case GameState::START:
		{
			screenText_->setString("Press fire to start");
			break;
		}
		case GameState::RESET:
		{
			bombPool_->reset();
			enemyPool_->reset();
			rocketPool_->reset();
			player_->reset();
			state_ = GameState::RUN;

			break;
		}
		case GameState::RUN:
		{
			screenString_.format("Score: %u - Lives: %u", player_->score(), player_->lives());
			screenText_->setString(screenString_);

			bombPool_->updateBombs(interval);
			bombPool_->draw();
			enemyPool_->update(interval);
			enemyPool_->draw();
			rocketPool_->updateRockets(interval);
			rocketPool_->draw();

			if (playerMovement_ == PlayerMovement::RIGHT)
				player_->move(interval);
			else if (playerMovement_ == PlayerMovement::LEFT)
				player_->move(-interval);

			bool rocketEnemyCollision = CollisionManager::performCollisions(rocketPool_.get(), enemyPool_.get());
			bool bombPlayerCollision = CollisionManager::performCollisions(player_.get(), bombPool_.get());
			bool enemyPlayerCollision = CollisionManager::performCollisions(player_.get(), enemyPool_.get());

			if (rocketEnemyCollision || enemyPlayerCollision)
				player_->increaseScore(Conf::PlayerKillScore);
			if (bombPlayerCollision || enemyPlayerCollision)
				player_->damage(Conf::PlayerDamage);

			if (player_->lives() <= 0 || enemyPool_->reachedBottom())
				state_ = GameState::GAME_OVER;
			else if (enemyPool_->isEmpty())
				state_ = GameState::CLEARED;

			break;
		}
		case GameState::PAUSE:
		{
			// Note: Pause state is currently not pausing shooting timers, which might result in wrong behavior

			screenString_.format("PAUSED (Score: %u - Lives: %u)", player_->score(), player_->lives());
			screenText_->setString(screenString_);

			break;
		}
		case GameState::CLEARED:
		{
			screenString_.format("Level cleared, press fire to start again (score: %u)", player_->score());
			screenText_->setString(screenString_);
			break;
		}
		case GameState::GAME_OVER:
		{
			screenString_.format("Game over, press fire to start again (score: %u)", player_->score());
			screenText_->setString(screenString_);
			break;
		}
	}
}

void Game::togglePause()
{
	if (state_ == GameState::RUN)
		state_ = GameState::PAUSE;
	else if (state_ == GameState::PAUSE)
		state_ = GameState::RUN;
}

void Game::firePressed()
{
	switch (state_)
	{
		case GameState::RUN:
		{
			player_->shoot();
			break;
		}
		default:
			break;
	}
}

void Game::fireReleased()
{
	switch (state_)
	{
		case GameState::START:
		{
			state_ = GameState::RESET;
			break;
		}
		case GameState::CLEARED:
		{
			state_ = GameState::RESET;
			break;
		}
		case GameState::GAME_OVER:
		{
			player_->resetScore();
			state_ = GameState::RESET;
			break;
		}
		default:
			break;
	}
}
