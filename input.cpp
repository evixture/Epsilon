#include "main.hpp"

Button::Button()
	: isDown(false), isSwitched(false), buttonSwitch(false)
{
}

//----------------------------------------------------------------------------------------------------

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

//----------------------------------------------------------------------------------------------------

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

//----------------------------------------------------------------------------------------------------

Mouse::Mouse()
	:screenPosition(Position2(0, 0)), mapPosition(Position2(0, 0))
{
}

void Mouse::update(TCOD_mouse_t TCODmouse)
{
	screenPosition = Position2(TCODmouse.cx, TCODmouse.cy);
	mapPosition = Position2(screenPosition.x + WORLD->xOffset - 1, screenPosition.y + WORLD->yOffset - 3);
}

//----------------------------------------------------------------------------------------------------

Input::Input()
	:keyboard(), TCODmouse(), mouse(std::make_shared<Mouse>())
{
	keyEvent = TCODSystem::checkForEvent(TCOD_EVENT_ANY, NULL, &TCODmouse);
	TCODMouse::showCursor(false);

	buttonList.push_back(moveUpKey				= std::make_shared<KeyboardButton>(sf::Keyboard::W));
	buttonList.push_back(moveDownKey			= std::make_shared<KeyboardButton>(sf::Keyboard::S));
	buttonList.push_back(moveLeftKey			= std::make_shared<KeyboardButton>(sf::Keyboard::A));
	buttonList.push_back(moveRightKey			= std::make_shared<KeyboardButton>(sf::Keyboard::D));

	buttonList.push_back(moveSlowKey			= std::make_shared<KeyboardButton>(sf::Keyboard::LControl));
	buttonList.push_back(moveFastKey			= std::make_shared<KeyboardButton>(sf::Keyboard::LShift));

	buttonList.push_back(stanceDownKey			= std::make_shared<KeyboardButton>(sf::Keyboard::Z));
	buttonList.push_back(stanceUpKey			= std::make_shared<KeyboardButton>(sf::Keyboard::X));

	buttonList.push_back(pickUpKey				= std::make_shared<KeyboardButton>(sf::Keyboard::E));
	buttonList.push_back(dropKey				= std::make_shared<KeyboardButton>(sf::Keyboard::Q));

	buttonList.push_back(reloadKey				= std::make_shared<KeyboardButton>(sf::Keyboard::R));

	buttonList.push_back(deepInteractKey		= std::make_shared<KeyboardButton>(sf::Keyboard::LAlt));
	buttonList.push_back(worldInteractKey		= std::make_shared<KeyboardButton>(sf::Keyboard::Space));

	buttonList.push_back(inventoryKey			= std::make_shared<KeyboardButton>(sf::Keyboard::I));
	buttonList.push_back(fullscreenKey			= std::make_shared<KeyboardButton>(sf::Keyboard::F11));
	buttonList.push_back(infoKey				= std::make_shared<KeyboardButton>(sf::Keyboard::N));
	buttonList.push_back(menuKey				= std::make_shared<KeyboardButton>(sf::Keyboard::Escape));

	buttonList.push_back(highlightKey			= std::make_shared<KeyboardButton>(sf::Keyboard::H));

	buttonList.push_back(debug1Key				= std::make_shared<KeyboardButton>(sf::Keyboard::Num9));
	buttonList.push_back(debug2Key				= std::make_shared<KeyboardButton>(sf::Keyboard::Num0));
	buttonList.push_back(debug3Key				= std::make_shared<KeyboardButton>(sf::Keyboard::C));

	buttonList.push_back(primaryUseButton		= std::make_shared<MouseButton>(sf::Mouse::Button::Left));
	buttonList.push_back(alternateUseButton		= std::make_shared<MouseButton>(sf::Mouse::Button::Right));
}

void Input::update()
{
	keyEvent = TCODSystem::checkForEvent(TCOD_EVENT_ANY, NULL, &TCODmouse);

	if (TCODConsole::hasMouseFocus()) TCODMouse::showCursor(false);

	for (auto& button : buttonList)
	{
		if		(TCODConsole::hasMouseFocus()) button->update();
		else	button->isDown = false;
	}

	if (menuKey->isSwitched)
	{
		if (GUI->activeWindow == Gui::ActiveWindow::NONE)
		{
			GUI->activeWindow = Gui::ActiveWindow::PAUSE;
			INPUT->menuKey->isSwitched = false;
		}
	}

	if (infoKey->isSwitched)
	{
		if		(GUI->activeLogWindow == Gui::ActiveLogWindow::LOG) GUI->activeLogWindow = Gui::ActiveLogWindow::INFO;
		else if (GUI->activeLogWindow == Gui::ActiveLogWindow::INFO) GUI->activeLogWindow = Gui::ActiveLogWindow::LOG;
	}

	mouse->update(TCODmouse);
}

