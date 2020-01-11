#include "main.hpp"

Key::Key(sf::Keyboard::Key sfKey)
	:sfKey(sfKey), isDown(false), isSwitched(false), keySwitch(false)
{}
Key::Key()
{}

void Key::update()
{
	isDown = sf::Keyboard::isKeyPressed(sfKey);

	if (sf::Keyboard::isKeyPressed(sfKey))
	{
		if (!keySwitch)
		{
			isSwitched = true;
			keySwitch = true;
		}
		else if (keySwitch)
		{
			isSwitched = false;
		}
	}
	else
	{
		isSwitched = false;
		keySwitch = false;
	}
}

//----------------------------------------------------------------------------------------------------

MouseButton::MouseButton(sf::Mouse::Button MButton)
	:sfMButton(MButton), isDown(false), isSwitched(false), buttonSwitch(false)
{}

MouseButton::MouseButton()
{}

void MouseButton::update()
{
	isDown = sf::Mouse::isButtonPressed(sfMButton);

	if (sf::Mouse::isButtonPressed(sfMButton))
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

Input::Input()
	:keyboard(), mouse()
{
	keyEvent = TCODSystem::checkForEvent(TCOD_EVENT_ANY, NULL, &mouse);
	TCODMouse::showCursor(false);

	keyList.push_back(w = std::make_shared<Key>(sf::Keyboard::W));
	keyList.push_back(a = std::make_shared<Key>(sf::Keyboard::A));
	keyList.push_back(s = std::make_shared<Key>(sf::Keyboard::S));
	keyList.push_back(d = std::make_shared<Key>(sf::Keyboard::D));

	keyList.push_back(lctrl		= std::make_shared<Key>(sf::Keyboard::LControl));
	keyList.push_back(lshift	= std::make_shared<Key>(sf::Keyboard::LShift));
	keyList.push_back(lalt		= std::make_shared<Key>(sf::Keyboard::LAlt));

	keyList.push_back(z = std::make_shared<Key>(sf::Keyboard::Z));
	keyList.push_back(x = std::make_shared<Key>(sf::Keyboard::X));
	keyList.push_back(c = std::make_shared<Key>(sf::Keyboard::C));

	keyList.push_back(e = std::make_shared<Key>(sf::Keyboard::E));
	keyList.push_back(q = std::make_shared<Key>(sf::Keyboard::Q));
	keyList.push_back(r = std::make_shared<Key>(sf::Keyboard::R));

	keyList.push_back(i = std::make_shared<Key>(sf::Keyboard::I));

	keyList.push_back(num0 = std::make_shared<Key>(sf::Keyboard::Num0));
	keyList.push_back(num9 = std::make_shared<Key>(sf::Keyboard::Num9));

	keyList.push_back(space		= std::make_shared<Key>(sf::Keyboard::Space));
	keyList.push_back(f11		= std::make_shared<Key>(sf::Keyboard::F11));
	keyList.push_back(escape	= std::make_shared<Key>(sf::Keyboard::Escape));

	mouseList.push_back(leftMouseButton		= std::make_shared<MouseButton>(sf::Mouse::Left));
	mouseList.push_back(rightMouseButton	= std::make_shared<MouseButton>(sf::Mouse::Right));
}

void Input::updateMouseInput()
{
	if (TCODConsole::hasMouseFocus())
	{
		for (auto& button : mouseList)
		{
			button->update();
		}
		TCODMouse::showCursor(false);
	}
}

void Input::updateKeyInput()
{
	for (auto& key : keyList)
	{
		if (TCODConsole::hasMouseFocus())
		{
			key->update();
		}
		else key->isDown = false;
	}
}

void Input::updateInput(std::shared_ptr<Player> player)
{
	keyEvent = TCODSystem::checkForEvent(TCOD_EVENT_ANY, NULL, &mouse);

	updateKeyInput();

	if (escape->isSwitched)
	{
		engine->gamestate = Engine::Gamestate::EXIT;
	}
}

void Input::update(std::shared_ptr<Player> player)
{
	updateInput(player);
	updateMouseInput();
}
