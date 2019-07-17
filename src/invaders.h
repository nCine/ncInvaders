#ifndef CLASS_MYEVENTHANDLER
#define CLASS_MYEVENTHANDLER

#include <ncine/IAppEventHandler.h>
#include <ncine/IInputEventHandler.h>
#include <nctl/UniquePtr.h>

namespace ncine {

class AppConfiguration;

}

namespace nc = ncine;

class Game;

/// My nCine event handler
class MyEventHandler :
    public nc::IAppEventHandler,
    public nc::IInputEventHandler
{
  public:
	void onPreInit(nc::AppConfiguration &config) override;
	void onInit() override;
	void onFrameStart() override;

#ifdef __ANDROID__
	virtual void onTouchDown(const nc::TouchEvent &event) override;
	virtual void onTouchUp(const nc::TouchEvent &event) override;
#endif
	void onKeyPressed(const nc::KeyboardEvent &event) override;
	void onKeyReleased(const nc::KeyboardEvent &event) override;
	void onMouseButtonPressed(const nc::MouseEvent &event) override;
	void onMouseButtonReleased(const nc::MouseEvent &event) override;
	void onJoyMappedButtonPressed(const nc::JoyMappedButtonEvent &event) override;
	void onJoyMappedButtonReleased(const nc::JoyMappedButtonEvent &event) override;

  private:
	nctl::UniquePtr<Game> game_;
	bool shouldStop_ = true;
};

#endif
