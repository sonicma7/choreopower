// InputManager
// The input manager is a wrapper for the OIS system. Just
// makes it easier to manage and deal with OIS for our
// specific needs


#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <Ogre.h>
#include <OIS/OIS.h>

class InputManager {
public:
	InputManager(Ogre::RenderWindow *win);
	~InputManager();

	void update();

	bool keyDown(OIS::KeyCode k);
	bool keyPressed(OIS::KeyCode k);
	bool keyReleased(OIS::KeyCode k);

	bool mouseButtonDown(OIS::MouseButtonID b);
	Ogre::Vector2 mousePos();
	
	int numJoysticks() const;
	bool joystickButtonDown(int joystickID, int button);
	bool joystickButtonPressed(int joystickID, int button);
	bool joystickButtonReleased(int joystickID, int button);

private:
	OIS::InputManager *mOISInputManager;
	OIS::Keyboard *mKeyboard;
	OIS::Mouse *mMouse;
	std::vector<OIS::JoyStick*> mJoysticks;
	char mLastKeyState[256];
	std::vector<OIS::JoyStickState> mLastJoyStickStates;
};

#endif