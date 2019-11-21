#include "main.hpp"

//Key Struct
Key::Key(sf::Keyboard::Key sfKey)
	:sfKey(sfKey), isDown(false), isPressed(false), isSwitched(false), keySwitch(false)
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

//Input Struct
Input::Input()
	:keyboard(), mouse(), moveTimer(0), moveWait(10), baseMoveWait(0.0f), leftMouseClick(false), moveXSpeed(0), moveYSpeed(0)
{
	keyEvent = TCODSystem::checkForEvent(TCOD_EVENT_ANY, NULL, &mouse);
	TCODMouse::showCursor(false);

	keyList.push_back(w = std::make_shared<Key>(sf::Keyboard::W));
	keyList.push_back(a = std::make_shared<Key>(sf::Keyboard::A));
	keyList.push_back(s = std::make_shared<Key>(sf::Keyboard::S));
	keyList.push_back(d = std::make_shared<Key>(sf::Keyboard::D));

	keyList.push_back(lctrl = std::make_shared<Key>(sf::Keyboard::LControl));
	keyList.push_back(lshift = std::make_shared<Key>(sf::Keyboard::LShift));

	keyList.push_back(z = std::make_shared<Key>(sf::Keyboard::Z));
	keyList.push_back(x = std::make_shared<Key>(sf::Keyboard::X));
	keyList.push_back(c = std::make_shared<Key>(sf::Keyboard::C));

	keyList.push_back(e = std::make_shared<Key>(sf::Keyboard::E));
	keyList.push_back(q = std::make_shared<Key>(sf::Keyboard::Q));
	keyList.push_back(r = std::make_shared<Key>(sf::Keyboard::R));

	keyList.push_back(num0 = std::make_shared<Key>(sf::Keyboard::Num0));
	keyList.push_back(num9 = std::make_shared<Key>(sf::Keyboard::Num9));

	keyList.push_back(space = std::make_shared<Key>(sf::Keyboard::Space));
	keyList.push_back(f11 = std::make_shared<Key>(sf::Keyboard::F11));
	keyList.push_back(escape = std::make_shared<Key>(sf::Keyboard::Escape));
}

void Input::getMouseInput()
{
	if (TCODConsole::hasMouseFocus())
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			leftMouseClick = true;
		}
		else
		{
			leftMouseClick = false;
		}
	}
}

void Input::getKeyDown()
{
	/*---------- PLAYER AND MENU MOVEMENT KEYS ----------*/

	////	W & UP
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	//{
	//	moveUp = true;
	//}
	//else moveUp = false;

	////	S & DOWN
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	//{
	//	moveDown = true;
	//}
	//else moveDown = false;

	////	A & DOWN
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	//{
	//	moveLeft = true;
	//}
	//else moveLeft = false;

	////	D & RIGHT
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	//{
	//	moveRight = true;
	//}
	//else moveRight = false;

	////CHANGE FLOOR
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	//{
	//	if (!changeFloorToggle)
	//	{
	//		changeFloor = true;
	//		changeFloorToggle = true;
	//	}
	//	else if (changeFloorToggle)
	//	{
	//		changeFloor = false;
	//	}
	//}
	//else
	//{
	//	changeFloor = false;
	//	changeFloorToggle = false;
	//}

	////	SPEED
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
	//{
	//	baseMoveWait = .25f;
	//}
	//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
	//{
	//	baseMoveWait = 1.0f;
	//}
	//else
	//{
	//	baseMoveWait = .5f;
	//}

	////HEIGHT
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	//{
	//	engine->gui->mapPane->world->player->height = 1;
	//}
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
	//{
	//	engine->gui->mapPane->world->player->height = 2;
	//}
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
	//{
	//	engine->gui->mapPane->world->player->height = 3;
	//}

	////WEAPONS
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	//{
	//	if (!reloadToggle)
	//	{
	//		reloadToggle = true;
	//		reload = true;
	//	}
	//	else if (reloadToggle)
	//	{
	//		reload = false;
	//	}
	//}
	//else
	//{
	//	reload = false;
	//	reloadToggle = false;
	//}

	///*---------- FUNCTION KEYS ----------*/

	////	F11
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::F11))
	//{
	//	if (f11Toggle)
	//	{
	//		engine->settings->setFullscreen();
	//		f11Toggle = false;
	//	}
	//}
	//else
	//{
	//	f11Toggle = true;
	//}

	//	//CLOSES APPLICATION
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	//{
	//	engine->gamestate = Engine::EXIT;
	//}
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
		
		moveWait = (int)((baseMoveWait / GUI->mapPane->world->player->height) * SETTINGS->fpsCount);

		if (w->isDown)
		{
			moveYSpeed = -1;
		}
		if (s->isDown)
		{
			moveYSpeed = 1;
		}
		if (a->isDown)
		{
			moveXSpeed = -1;
		}
		if (d->isDown)
		{
			moveXSpeed = 1;
		}

		if (moveXSpeed != 0 || moveYSpeed != 0)
		{
			if (moveTimer == 0)
			{
				if (GUI->mapPane->world->getWalkability(player->mapPosition.x + moveXSpeed, player->mapPosition.y, player->mapPosition.level))
				{
					player->mapPosition.x += moveXSpeed;
					moveXSpeed = 0;
					moveTimer = moveWait;
				}
				if (GUI->mapPane->world->getWalkability(player->mapPosition.x, player->mapPosition.y + moveYSpeed, player->mapPosition.level))
				{
					player->mapPosition.y += moveYSpeed;
					moveYSpeed = 0;
					moveTimer = moveWait;
				}
			}
			else
			{
				moveTimer--;
			}
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

