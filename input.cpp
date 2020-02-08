#include "main.hpp"

Button::Button()
	: isDown(false), isSwitched(false), buttonSwitch(false)
{
}

void Button::update()
{
}

KeyboardButton::KeyboardButton(sf::Keyboard::Key key)
	: Button(), button(key)
{
}

void KeyboardButton::update()
{
	isDown = sf::Keyboard::isKeyPressed(button);
	
	if (sf::Keyboard::isKeyPressed(button))
	{
		if (!buttonSwitch)
		{
			isSwitched = true;
			buttonSwitch = true;
		}
		else if (buttonSwitch)
		{
			isSwitched = false;
		}
	}
	else
	{
		isSwitched = false;
		buttonSwitch = false;
	}
}

MouseButton::MouseButton(sf::Mouse::Button button)
	: Button(), button(button)
{
}

void MouseButton::update()
{
	isDown = sf::Mouse::isButtonPressed(button);

	if (sf::Mouse::isButtonPressed(button))
	{
		if (!buttonSwitch)
		{
			isSwitched = true;
			buttonSwitch = true;
		}
		else if (buttonSwitch)
		{
			isSwitched = false;
		}
	}
	else
	{
		isSwitched = false;
		buttonSwitch = false;
	}
}

//Key::Key(sf::Keyboard::Key sfKey)
//	:sfKey(sfKey), isDown(false), isSwitched(false), keySwitch(false)
//{}
//Key::Key()
//{}
//
//void Key::update()
//{
//	isDown = sf::Keyboard::isKeyPressed(sfKey);
//
//	if (sf::Keyboard::isKeyPressed(sfKey))
//	{
//		if (!keySwitch)
//		{
//			isSwitched = true;
//			keySwitch = true;
//		}
//		else if (keySwitch)
//		{
//			isSwitched = false;
//		}
//	}
//	else
//	{
//		isSwitched = false;
//		keySwitch = false;
//	}
//}
//
////----------------------------------------------------------------------------------------------------
//
//MouseButton::MouseButton(sf::Mouse::Button MButton)
//	:sfMButton(MButton), isDown(false), isSwitched(false), buttonSwitch(false)
//{}
//
//MouseButton::MouseButton()
//{}
//
//void MouseButton::update()
//{
//	isDown = sf::Mouse::isButtonPressed(sfMButton);
//
//	if (sf::Mouse::isButtonPressed(sfMButton))
//	{
//		if (!buttonSwitch)
//		{
//			isSwitched = true;
//			buttonSwitch = true;
//		}
//		else if (buttonSwitch)
//		{
//			isSwitched = false;
//		}
//	}
//	else
//	{
//		isSwitched = false;
//		buttonSwitch = false;
//	}
//}

//----------------------------------------------------------------------------------------------------

Input::Input()
	:keyboard(), mouse()
{
	keyEvent = TCODSystem::checkForEvent(TCOD_EVENT_ANY, NULL, &mouse);
	TCODMouse::showCursor(false);

	/*buttonList.push_back(w = std::make_shared<Key>(sf::Keyboard::W));
	buttonList.push_back(a = std::make_shared<Key>(sf::Keyboard::A));
	buttonList.push_back(s = std::make_shared<Key>(sf::Keyboard::S));
	buttonList.push_back(d = std::make_shared<Key>(sf::Keyboard::D));

	buttonList.push_back(lctrl		= std::make_shared<Key>(sf::Keyboard::LControl));
	buttonList.push_back(lshift	= std::make_shared<Key>(sf::Keyboard::LShift));
	buttonList.push_back(lalt		= std::make_shared<Key>(sf::Keyboard::LAlt));

	buttonList.push_back(z = std::make_shared<Key>(sf::Keyboard::Z));
	buttonList.push_back(x = std::make_shared<Key>(sf::Keyboard::X));
	buttonList.push_back(c = std::make_shared<Key>(sf::Keyboard::C));

	buttonList.push_back(e = std::make_shared<Key>(sf::Keyboard::E));
	buttonList.push_back(q = std::make_shared<Key>(sf::Keyboard::Q));
	buttonList.push_back(r = std::make_shared<Key>(sf::Keyboard::R));

	buttonList.push_back(i = std::make_shared<Key>(sf::Keyboard::I));

	buttonList.push_back(num0 = std::make_shared<Key>(sf::Keyboard::Num0));
	buttonList.push_back(num9 = std::make_shared<Key>(sf::Keyboard::Num9));

	buttonList.push_back(space		= std::make_shared<Key>(sf::Keyboard::Space));
	buttonList.push_back(f11		= std::make_shared<Key>(sf::Keyboard::F11));
	buttonList.push_back(escape	= std::make_shared<Key>(sf::Keyboard::Escape));

	mouseList.push_back(leftMouseButton		= std::make_shared<MouseButton>(sf::Mouse::Left));
	mouseList.push_back(rightMouseButton	= std::make_shared<MouseButton>(sf::Mouse::Right));*/

	buttonList.push_back(moveUpKey =			std::make_shared<KeyboardButton>(sf::Keyboard::W));
	buttonList.push_back(moveDownKey =			std::make_shared<KeyboardButton>(sf::Keyboard::S));
	buttonList.push_back(moveLeftKey =			std::make_shared<KeyboardButton>(sf::Keyboard::A));
	buttonList.push_back(moveRightKey =			std::make_shared<KeyboardButton>(sf::Keyboard::D));

	buttonList.push_back(moveSlowKey =			std::make_shared<KeyboardButton>(sf::Keyboard::LControl));
	buttonList.push_back(moveFastKey =			std::make_shared<KeyboardButton>(sf::Keyboard::LShift));

	buttonList.push_back(proneKey =				std::make_shared<KeyboardButton>(sf::Keyboard::Z));
	buttonList.push_back(crouchKey =			std::make_shared<KeyboardButton>(sf::Keyboard::X));
	buttonList.push_back(standKey =				std::make_shared<KeyboardButton>(sf::Keyboard::C));

	buttonList.push_back(pickUpKey =			std::make_shared<KeyboardButton>(sf::Keyboard::E));
	buttonList.push_back(dropKey =				std::make_shared<KeyboardButton>(sf::Keyboard::Q));

	buttonList.push_back(reloadKey =			std::make_shared<KeyboardButton>(sf::Keyboard::R));

	buttonList.push_back(deepInteractKey =		std::make_shared < KeyboardButton>(sf::Keyboard::LAlt));
	buttonList.push_back(worldInteractKey =		std::make_shared < KeyboardButton>(sf::Keyboard::Space));

	buttonList.push_back(inventoryKey =			std::make_shared < KeyboardButton>(sf::Keyboard::I));
	buttonList.push_back(fullscreenKey =		std::make_shared < KeyboardButton>(sf::Keyboard::F11));
	buttonList.push_back(menuKey =				std::make_shared < KeyboardButton>(sf::Keyboard::Escape));

	buttonList.push_back(debug1Key =			std::make_shared < KeyboardButton>(sf::Keyboard::Num9));
	buttonList.push_back(debug2Key =			std::make_shared < KeyboardButton>(sf::Keyboard::Num0));

	buttonList.push_back(primaryUseButton =		std::make_shared < MouseButton>(sf::Mouse::Button::Left));
	buttonList.push_back(alternateUseButton =	std::make_shared < MouseButton>(sf::Mouse::Button::Right));
}

void Input::update()
{
	keyEvent = TCODSystem::checkForEvent(TCOD_EVENT_ANY, NULL, &mouse);

	if (TCODConsole::hasMouseFocus())
	{
		TCODMouse::showCursor(false);

	}

	for (auto& button : buttonList)
	{
		if (TCODConsole::hasMouseFocus())
		{
			button->update();
		}
		else
		{
			button->isDown = false;
		}
	}

	if (menuKey->isSwitched)
	{
		engine->gamestate = Engine::Gamestate::EXIT;
	}
}