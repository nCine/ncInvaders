#include <cmath>

#include "invaders.h"
#include "Game.h"

#include <ncine/Application.h>
#include <ncine/AppConfiguration.h>
#include <ncine/SceneNode.h>
#include <ncine/Texture.h>
#include <ncine/Sprite.h>
#include <ncine/TextNode.h>

nctl::UniquePtr<nc::IAppEventHandler> createAppEventHandler()
{
	return nctl::makeUnique<MyEventHandler>();
}

void MyEventHandler::onPreInit(nc::AppConfiguration &config)
{
#if defined(__ANDROID__)
	config.dataPath() = "asset::";
#elif defined(__EMSCRIPTEN__)
	config.dataPath() = "/";
#else
	#ifdef NCPROJECT_DEFAULT_DATA_DIR
	config.dataPath() = NCPROJECT_DEFAULT_DATA_DIR;
	#else
	config.dataPath() = "data/";
	#endif
#endif

	config.windowTitle = "ncInvaders";
	config.windowIconFilename = "icon48.png";

	config.resolution.set(1920, 1080);
}

void MyEventHandler::onInit()
{
	game_ = nctl::makeUnique<Game>();
}

void MyEventHandler::onFrameStart()
{
	if (shouldStop_)
		game_->movePlayer(Game::PlayerMovement::STOP);

	// Keyboard input handling
	const nc::KeyboardState &keyState = nc::theApplication().inputManager().keyboardState();
	if (keyState.isKeyDown(nc::KeySym::RIGHT))
		game_->movePlayer(Game::PlayerMovement::RIGHT);
	else if (keyState.isKeyDown(nc::KeySym::LEFT))
		game_->movePlayer(Game::PlayerMovement::LEFT);

	const nc::JoyMappedState &joyState = nc::theApplication().inputManager().joyMappedState(0);
	const float axisValue = joyState.axisValue(nc::AxisName::LX);
	if (axisValue > nc::IInputManager::LeftStickDeadZone)
		game_->movePlayer(Game::PlayerMovement::RIGHT);
	else if (axisValue < -nc::IInputManager::LeftStickDeadZone)
		game_->movePlayer(Game::PlayerMovement::LEFT);

	const float interval = nc::theApplication().interval();
	game_->update(interval);
}

#ifdef __ANDROID__
void MyEventHandler::onTouchDown(const nc::TouchEvent &event)
{
	if (event.pointers[0].x > nc::theApplication().width() * 0.75f)
		game_->movePlayer(Game::PlayerMovement::RIGHT);
	else if (event.pointers[0].x < nc::theApplication().width() * 0.25f)
		game_->movePlayer(Game::PlayerMovement::LEFT);
	else
		game_->firePressed();

	shouldStop_ = false;
}

void MyEventHandler::onTouchUp(const nc::TouchEvent &event)
{
	game_->fireReleased();
	shouldStop_ = true;
}
#endif

void MyEventHandler::onKeyPressed(const nc::KeyboardEvent &event)
{
	if (event.sym == nc::KeySym::SPACE)
		game_->firePressed();
}

void MyEventHandler::onKeyReleased(const nc::KeyboardEvent &event)
{
	if (event.sym == nc::KeySym::ESCAPE)
		nc::theApplication().quit();
	else if (event.sym == nc::KeySym::P)
		game_->togglePause();
	else if (event.sym == nc::KeySym::SPACE)
		game_->fireReleased();
	else if (event.sym == nc::KeySym::R)
		game_->reset();
}

void MyEventHandler::onMouseButtonPressed(const nc::MouseEvent &event)
{
	if (event.x > nc::theApplication().width() * 0.75f)
		game_->movePlayer(Game::PlayerMovement::RIGHT);
	else if (event.x < nc::theApplication().width() * 0.25f)
		game_->movePlayer(Game::PlayerMovement::LEFT);
	else
		game_->firePressed();

	shouldStop_ = false;
}

void MyEventHandler::onMouseButtonReleased(const nc::MouseEvent &event)
{
	game_->fireReleased();
	shouldStop_ = true;
}

void MyEventHandler::onJoyMappedButtonPressed(const nc::JoyMappedButtonEvent &event)
{
	if (event.buttonName == nc::ButtonName::A)
		game_->firePressed();
}

void MyEventHandler::onJoyMappedButtonReleased(const nc::JoyMappedButtonEvent &event)
{
	if (event.buttonName == nc::ButtonName::START)
		game_->reset();
	else if (event.buttonName == nc::ButtonName::GUIDE)
		nc::theApplication().quit();
	else if (event.buttonName == nc::ButtonName::BACK)
		game_->togglePause();
	else if (event.buttonName == nc::ButtonName::A)
		game_->fireReleased();
}
