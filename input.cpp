#include "main.hpp"

//Input Struct
Input::Input()
	:keyboard(), mouse(), moveUp(false), moveDown(false), moveLeft(false), moveRight(false), moveTimer(0), moveWait(10), f11Toggle(false), baseMoveWait(0), leftMouseClick(false), reload(false), changeFloor(false)
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
			//engine->gui->mapPane->world->player->testWeapon->weaponFire(engine->settings->input->mouse.cx - 1, engine->settings->input->mouse.cy - 3, engine->gui->mapPane->drawWindow);
		}
		else
		{
			//TCODLine::init(player->testWeapon->weaponx, player->testWeapon->weapony, engine->settings->input->mouse.cx - 1, engine->settings->input->mouse.cy - 3);
			leftMouseClick = false;
		}
	}
}

void Input::getKeyDown()
{
	/*---------- PLAYER AND MENU MOVEMENT KEYS ----------*/

	//	W & UP
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		moveUp = true;
	}
	else moveUp = false;

	//	S & DOWN
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		moveDown = true;
	}
	else moveDown = false;

	//	A & DOWN
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		moveLeft = true;
	}
	else moveLeft = false;

	//	D & RIGHT
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		moveRight = true;
	}
	else moveRight = false;

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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
	{
		baseMoveWait = 15;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
	{
		baseMoveWait = 60;
	}
	else
	{
		baseMoveWait = 30;
	}

	//HEIGHT
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	{
		engine->gui->mapPane->world->player->height = 1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
	{
		engine->gui->mapPane->world->player->height = 2;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		engine->gamestate = Engine::EXIT;
	}

	// ALT + KEY COMMANDS
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
	{
		//HARD CHANGES THE MAP RENDERED
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0))
		{
			engine->gui->mapPane->world->player->level = 0;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
		{
			engine->gui->mapPane->world->player->level = 1;
		}

	}
}

//multiply speed by height for slower movement at lower heights
void Input::getKeyInput(std::shared_ptr<Player> player)
{
	keyEvent = TCODSystem::checkForEvent(TCOD_EVENT_ANY, &keyboard, &mouse);

	if (engine->gamestate == Engine::MAIN)
	{
		getKeyDown();

		moveXSpeed = 0;
		moveYSpeed = 0;
		
		moveWait = baseMoveWait / engine->gui->mapPane->world->player->height;

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
				if (engine->gui->mapPane->world->getWalkability(player->mapPosition.x + moveXSpeed, player->mapPosition.y, player->level))
				{
					player->mapPosition.x += moveXSpeed;
					moveXSpeed = 0;
					moveTimer = moveWait;
				}
				if (engine->gui->mapPane->world->getWalkability(player->mapPosition.x, player->mapPosition.y + moveYSpeed, player->level))
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
