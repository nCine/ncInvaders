#ifndef GAME_H
#define GAME_H

#include <nctl/UniquePtr.h>
#include <nctl/String.h>

namespace ncine {

class Texture;
class Font;
class TextNode;

}

namespace nc = ncine;

class ProjectilePool;
class EnemyPool;
class Player;

/// The class that handles all game states and objects
class Game
{
  public:
	enum class GameState
	{
		/// The game has just started (waiting for a keypress)
		START,
		/// A transition state used to reset game entities
		RESET,
		/// The game is running, game entities are updating
		RUN,
		/// The game was running but now is in pause
		PAUSE,
		/// The player has killed all enemies (waiting for a keypress)
		CLEARED,
		/// The player is dead (waiting for a keypress)
		GAME_OVER
	};

	enum class PlayerMovement
	{
		LEFT,
		RIGHT,
		STOP
	};

	Game();
	~Game();
	void update(float interval);
	inline void reset() { state_ = GameState::START; }
	void togglePause();

	inline void movePlayer(PlayerMovement movement) { playerMovement_ = movement; }
	void firePressed();
	void fireReleased();

  private:
	GameState state_;
	nctl::String screenString_;
	PlayerMovement playerMovement_;

	nctl::UniquePtr<nc::Texture> bombTexture_;
	nctl::UniquePtr<nc::Texture> enemy1Texture_;
	nctl::UniquePtr<nc::Texture> enemy2Texture_;
	nctl::UniquePtr<nc::Texture> rocketTexture_;
	nctl::UniquePtr<nc::Texture> playerTexture_;

	nctl::UniquePtr<nc::Font> font_;
	nctl::UniquePtr<nc::TextNode> screenText_;

	nctl::UniquePtr<ProjectilePool> bombPool_;
	nctl::UniquePtr<EnemyPool> enemyPool_;
	nctl::UniquePtr<ProjectilePool> rocketPool_;
	nctl::UniquePtr<Player> player_;

	Game(const Game &) = delete;
	Game &operator=(const Game &) = delete;
};

#endif
