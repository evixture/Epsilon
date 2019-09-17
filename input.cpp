#include "main.hpp"

//Input Struct
Input::Input()
	:keyboard(), mouse(), moveUp(false), moveDown(false), moveLeft(false), moveRight(false), moveTimer(0), moveWait(10), f11Toggle(false)
{
	keyEvent = TCODSystem::checkForEvent(TCOD_EVENT_ANY, &keyboard, &mouse);
}

void Input::getMouseInput()
{
	return;
}

//TODO : figure out how to get char and vk switches working
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

	//	SHIFT
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
	{
		moveWait = 5;
	}

	//	LEFT CONTROL
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
	{
		moveWait = 20;
	}
	else
	{
		moveWait = 10;
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

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		//CLOSES APPLICATION
		engine->settings->isActive = false;
	}
}

void Input::getKeyInput(std::shared_ptr<Player> player)
{
	keyEvent = TCODSystem::checkForEvent(TCOD_EVENT_ANY, &keyboard, &mouse);

	if (engine->gamestate == Engine::MAIN)
	{
		getKeyDown();

		moveXSpeed = 0;
		moveYSpeed = 0;

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
				if (engine->gui->mapWindow->world->getWalkability(player->position.x + moveXSpeed, player->position.y, player->level))
				{
					player->position.x += moveXSpeed;
					moveXSpeed = 0;
					moveTimer = moveWait;
				}
				if (engine->gui->mapWindow->world->getWalkability(player->position.x, player->position.y + moveYSpeed, player->level))
				{
					player->position.y += moveYSpeed;
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

void Input::getInput(std::shared_ptr<Player> player)
{
	//check mortality in here
	getKeyInput(player);
}
