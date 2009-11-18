#include "InputManager.h"
#include "Munny.h"
#include "Ogre.h"

InputManager::InputManager(Ogre::RenderWindow *win) {
	mKeyboard = NULL;
	mMouse = NULL;
	mJoysticks = std::vector<OIS::JoyStick*>();

	size_t windowHnd = 0;
	std::ostringstream windowHndStr;
	OIS::ParamList params;

	win->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	params.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
	mOISInputManager = OIS::InputManager::createInputSystem(params);
	
	if(mOISInputManager->numKeyboards() > 0) {
		mKeyboard = static_cast<OIS::Keyboard*>(mOISInputManager->createInputObject(OIS::OISKeyboard, false));
	}

	if(mOISInputManager->numMice() > 0) {
		mMouse = static_cast<OIS::Mouse*>(mOISInputManager->createInputObject(OIS::OISMouse, false));
	}

	for(int i = 0; i < mOISInputManager->numJoySticks(); ++i) {
		mJoysticks.push_back(static_cast<OIS::JoyStick*>(mOISInputManager->createInputObject(OIS::OISJoyStick, false)));
		mLastJoyStickStates.push_back(mJoysticks.back()->getJoyStickState());
	}
}

InputManager::~InputManager() {

	if(mKeyboard != NULL) {
		mOISInputManager->destroyInputObject(mKeyboard);
	}

	if(mMouse != NULL) {
		mOISInputManager->destroyInputObject(mMouse);
	}

	for(unsigned int i = 0; i < mJoysticks.size(); ++i) {
		mOISInputManager->destroyInputObject(mJoysticks[i]);
	}

	OIS::InputManager::destroyInputSystem(mOISInputManager);
}

void InputManager::update() {
	mKeyboard->copyKeyStates(mLastKeyState);
	mKeyboard->capture();
	mMouse->capture();

	for(unsigned int i = 0; i < mJoysticks.size(); ++i) {
		mLastJoyStickStates[i] = mJoysticks[i]->getJoyStickState();
		mJoysticks[i]->capture();
	}
}

bool InputManager::keyDown(OIS::KeyCode k) {
	return mKeyboard->isKeyDown(k);
}

bool InputManager::keyPressed(OIS::KeyCode k) {
	if(mLastKeyState[k] == 0 && mKeyboard->isKeyDown(k)) {
		return true;
	}

	return false;
}

bool InputManager::keyReleased(OIS::KeyCode k) {
	if(mLastKeyState[k] == 1 && !mKeyboard->isKeyDown(k)) {
		return true;
	}

	return false;
}

bool InputManager::mouseButtonDown(OIS::MouseButtonID b) {
	OIS::MouseState state = mMouse->getMouseState();
	return state.buttonDown(b);
}

Ogre::Vector2 InputManager::mousePos() {
	OIS::MouseState state = mMouse->getMouseState();
	
	OIS::Axis xAxis;
	OIS::Axis yAxis;
	
	Ogre::Vector2 pos(xAxis.abs, yAxis.abs);

	return pos;
}

int InputManager::numJoysticks() const {
	return (int)mJoysticks.size();
}

bool InputManager::joystickButtonDown(int joystickID, int button){
	if(joystickID < (int)mJoysticks.size()) {
		OIS::JoyStickState state = mJoysticks[joystickID]->getJoyStickState();
		return (bool)state.buttonDown(button);
	}
	
	return false;
}

bool InputManager::joystickButtonPressed(int joystickID, int button) {
	if(joystickID < (int)mJoysticks.size()) {
		OIS::JoyStickState state = mJoysticks[joystickID]->getJoyStickState();
		
		if(state.buttonDown(button) == 1 && mLastJoyStickStates[joystickID].buttonDown(button) == 0) {
			return true;
		}
	}

	return false;
}

bool InputManager::joystickButtonReleased(int joystickID, int button) {
	if(joystickID < (int)mJoysticks.size()) {
		OIS::JoyStickState state = mJoysticks[joystickID]->getJoyStickState();

		if(state.buttonDown(button) == 0 && mLastJoyStickStates[joystickID].buttonDown(button) == 1) {
			return true;
		}
	}

	return false;
}