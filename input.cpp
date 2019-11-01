#include "main.hpp"

//Input Struct
Input::Input()
	:keyboard(), mouse(), moveUp(false), moveDown(false), moveLeft(false), moveRight(false), moveTimer(0), moveWait(10), f11Toggle(false), baseMoveWait(0.0f), leftMouseClick(false), reload(false), changeFloor(false),
	changeFloorToggle(false), moveXSpeed(0), moveYSpeed(0), reloadToggle(false), toggle(false)
{
	keyEvent = TCODSystem::checkForEvent(TCOD_EVENT_ANY, &keyboard, &mouse);
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

bool Input::getKeyDown(sf::Keyboard::Key key)
{
	if (sf::Keyboard::isKeyPressed(key))
	{
		return true;
	}
	return false;
}

bool Input::getKeyToggle(sf::Keyboard::Key key)
{
	return false;
}

bool Input::getKeyPressed(sf::Keyboard::Key key)
{
	if (getKeyDown(key))
	{
		if (!toggle)
		{
			toggle = true;
			return true;
		}
		else if (toggle)
		{
			return false;
		}
	}
	else
	{
		toggle = false;
		return false;
	}
}

void Input::getKeyActivity()
{
	/*---------- PLAYER AND MENU MOVEMENT KEYS ----------*/

	//MOVEMENT
	moveUp = getKeyDown(sf::Keyboard::W);
	moveDown = getKeyDown(sf::Keyboard::S);
	moveLeft = getKeyDown(sf::Keyboard::A);
	moveRight = getKeyDown(sf::Keyboard::D);

	//CHANGE FLOOR
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		if (!changeFloorToggle)
		{
			changeFloor = true;
			changeFloorToggle = true;
		}
		else if (changeFloorToggle)
		{
			changeFloor = false;
		}
	}
	else
	{
		changeFloor = false;
		changeFloorToggle = false;
	}

	//	SPEED
	if (getKeyDown(sf::Keyboard::LShift))
	{
		baseMoveWait = .25f;
	}
	else if (getKeyDown(sf::Keyboard::LControl))
	{
		baseMoveWait = 1.0f;
	}
	else
	{
		baseMoveWait = .5f;
	}

	//HEIGHT
	if (getKeyDown(sf::Keyboard::Z))
	{
		engine->gui->mapPane->world->player->height = 1;
	}
	if (getKeyDown(sf::Keyboard::X))
	{
		engine->gui->mapPane->world->player->height = 2;
	}
	if (getKeyDown(sf::Keyboard::C))
	{
		engine->gui->mapPane->world->player->height = 3;
	}

	//WEAPONS
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		if (!reloadToggle)
		{
			reloadToggle = true;
			reload = true;
		}
		else if (reloadToggle)
		{
			reload = false;
		}
	}
	else
	{
		reload = false;
		reloadToggle = false;
	}


	/*---------- FUNCTION KEYS ----------*/

	//	F11
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F11))
	{
		if (f11Toggle)
		{
			engine->settings->setFullscreen();
			f11Toggle = false;
		}
	}
	else
	{
		f11Toggle = true;
	}

		//CLOSES APPLICATION
	if (getKeyDown(sf::Keyboard::Escape))
	{
		engine->gamestate = Engine::EXIT;
	}
}

void Input::getKeyInput(std::shared_ptr<Player> player)
{
	keyEvent = TCODSystem::checkForEvent(TCOD_EVENT_ANY, &keyboard, &mouse);

	if (engine->gamestate == Engine::MAIN)
	{
		getKeyActivity();

		moveXSpeed = 0;
		moveYSpeed = 0;
		
		moveWait = (int)((baseMoveWait / engine->gui->mapPane->world->player->height) * TCODSystem::getFps());

		if (moveUp)
		{
			moveYSpeed = -1;
		}
		if (moveDown)
		{
			moveYSpeed = 1;
		}
		if (moveLeft)
		{
			moveXSpeed = -1;
		}
		if (moveRight)
		{
			moveXSpeed = 1;
		}

		if (moveXSpeed != 0 || moveYSpeed != 0)
		{
			if (moveTimer == 0)
			{
				if (engine->gui->mapPane->world->getWalkability(player->mapPosition.x + moveXSpeed, player->mapPosition.y, player->mapPosition.level))
				{
					player->mapPosition.x += moveXSpeed;
					moveXSpeed = 0;
					moveTimer = moveWait;
				}
				if (engine->gui->mapPane->world->getWalkability(player->mapPosition.x, player->mapPosition.y + moveYSpeed, player->mapPosition.level))
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
}

void Input::update(std::shared_ptr<Player> player)
{
	getKeyInput(player);
	getMouseInput();
}

