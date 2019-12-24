#include "main.hpp"

//Key Struct
Key::Key(sf::Keyboard::Key sfKey)
	:sfKey(sfKey), isDown(false), isSwitched(false), keySwitch(false)
{
}
Key::Key()
{
}

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
{
}

MouseButton::MouseButton()
{
}

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

//Input Struct
Input::Input()
	:keyboard(), mouse(), baseMoveWait(0.0f), moveXSpeed(0), moveYSpeed(0), movementClock(Clock(0))
{
	keyEvent = TCODSystem::checkForEvent(TCOD_EVENT_ANY, NULL, &mouse);
	TCODMouse::showCursor(false);

	keyList.push_back(w = std::make_shared<Key>(sf::Keyboard::W));
	keyList.push_back(a = std::make_shared<Key>(sf::Keyboard::A));
	keyList.push_back(s = std::make_shared<Key>(sf::Keyboard::S));
	keyList.push_back(d = std::make_shared<Key>(sf::Keyboard::D));

	keyList.push_back(lctrl = std::make_shared<Key>(sf::Keyboard::LControl));
	keyList.push_back(lshift = std::make_shared<Key>(sf::Keyboard::LShift));
	keyList.push_back(lalt = std::make_shared<Key>(sf::Keyboard::LAlt));

	keyList.push_back(z = std::make_shared<Key>(sf::Keyboard::Z));
	keyList.push_back(x = std::make_shared<Key>(sf::Keyboard::X));
	keyList.push_back(c = std::make_shared<Key>(sf::Keyboard::C));

	keyList.push_back(e = std::make_shared<Key>(sf::Keyboard::E));
	keyList.push_back(q = std::make_shared<Key>(sf::Keyboard::Q));
	keyList.push_back(r = std::make_shared<Key>(sf::Keyboard::R));

	keyList.push_back(i = std::make_shared<Key>(sf::Keyboard::I));

	keyList.push_back(num0 = std::make_shared<Key>(sf::Keyboard::Num0));
	keyList.push_back(num9 = std::make_shared<Key>(sf::Keyboard::Num9));

	keyList.push_back(space = std::make_shared<Key>(sf::Keyboard::Space));
	keyList.push_back(f11 = std::make_shared<Key>(sf::Keyboard::F11));
	keyList.push_back(escape = std::make_shared<Key>(sf::Keyboard::Escape));

	mouseList.push_back(leftMouseButton = std::make_shared<MouseButton>(sf::Mouse::Left));
	mouseList.push_back(rightMouseButton = std::make_shared<MouseButton>(sf::Mouse::Right));
}

void Input::getMouseInput()
{
	if (TCODConsole::hasMouseFocus())
	{
		for (auto& button : mouseList)
		{
			button->update();
		}
	}
}

void Input::getKeyDown()
{
	for (auto& key : keyList)
	{
		key->update();
	}

	if (lshift->isDown)
	{
		baseMoveWait = .25f;
	}
	else if (lctrl->isDown)
	{
		baseMoveWait = 1.0f;
	}
	else baseMoveWait = .5f;
}

void Input::getKeyInput(std::shared_ptr<Player> player)
{
	keyEvent = TCODSystem::checkForEvent(TCOD_EVENT_ANY, NULL, &mouse);

	getKeyDown();

	if (GUI->activeWindow != Gui::STARTUPSPLASH)
	{

		moveXSpeed = 0;
		moveYSpeed = 0;

		if (INPUT->z->isSwitched)
		{
			WORLD->player->height = 1;
		}
		if (INPUT->x->isSwitched)
		{
			WORLD->player->height = 2;
		}
		if (INPUT->c->isSwitched)
		{
			WORLD->player->height = 3;
		}
		
		movementClock.capacity = (int)((baseMoveWait / WORLD->player->height) * SETTINGS->fpsCount);

		//change so keys not favored

		if (w->isDown && s->isDown)
		{
			moveYSpeed = 0;
		}
		else if (w->isDown && !s->isDown)
		{
			moveYSpeed = -1;
		}
		else if (s->isDown && !w->isDown)
		{
			moveYSpeed = 1;
		}

		if (a->isDown && d->isDown)
		{
			moveXSpeed = 0;
		}
		else if (a->isDown && !d->isDown)
		{
			moveXSpeed = -1;
		}
		else if (d->isDown && !a->isDown)
		{
			moveXSpeed = 1;
		}

		if (moveXSpeed != 0 || moveYSpeed != 0)
		{
			if (movementClock.step == 0)
			{
				if (GUI->mapPane->world->getWalkability(player->mapPosition.x + moveXSpeed, player->mapPosition.y, player->mapPosition.level))
				{
					player->mapPosition.x += moveXSpeed;
					moveXSpeed = 0;
				}
				if (GUI->mapPane->world->getWalkability(player->mapPosition.x, player->mapPosition.y + moveYSpeed, player->mapPosition.level))
				{
					player->mapPosition.y += moveYSpeed;
					moveYSpeed = 0;
				}
			}
			movementClock.tickDownWithReset();
		}
	}
	
	if (escape->isSwitched)
	{
		engine->gamestate = Engine::EXIT;
	}
}

void Input::update(std::shared_ptr<Player> player)
{
	getKeyInput(player);
	getMouseInput();
}
