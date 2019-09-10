#include "main.hpp"

//Input Struct
Input::Input()
	:keyboard(), mouse(), movingUp(false), movingDown(false), movingLeft(false), movingRight(false), moveTimer(0), moveWait(10)
{
	keyEvent = TCODSystem::checkForEvent(TCOD_EVENT_ANY, &keyboard, &mouse);
}

void Input::getMouseInput()
{
	return;
}

void Input::getKeyDown()
{
	if (keyEvent == TCOD_KEY_PRESSED)
	{
		switch (keyboard.c)
		{
		case 'w':
			movingUp = true;
			break;
		case 's':
			movingDown = true;
			break;
		case 'a':
			movingLeft = true;
			break;
		case 'd':
			movingRight = true;
			break;
		default:
			break;
		}
 
		switch (keyboard.vk)
		{
		case TCODK_UP:
			movingUp = true;
			break;
		case TCODK_DOWN:
			movingDown = true;
			break;
		case TCODK_LEFT:
			movingLeft = true;
			break;
		case TCODK_RIGHT:
			movingRight = true;
			break;
		case TCODK_F11:
			engine.settings->setFullscreen();
			break;
		default:
			break;
		}
	}
}

void Input::getKeyUp()
{
	if (keyEvent == TCOD_KEY_RELEASED)
	{
		switch (keyboard.c)
		{
		case 'w':
			movingUp = false;
			break;
		case 's':
			movingDown = false;
			break;
		case 'a':
			movingLeft = false;
			break;
		case 'd':
			movingRight = false;
			break;
		default:
			break;
		}

		switch (keyboard.vk)
		{
		case TCODK_UP:
			movingUp = false;
			break;
		case TCODK_DOWN:
			movingDown = false;
			break;
		case TCODK_LEFT:
			movingLeft = false;
			break;
		case TCODK_RIGHT:
			movingRight = false;
			break;
		default:
			break;
		}
	}
}

void Input::getKeyInput(std::shared_ptr<Player> player)
{
	keyEvent = TCODSystem::checkForEvent(TCOD_EVENT_ANY, &keyboard, &mouse);

	if (engine.gamestate == Engine::MAIN)
	{
		getKeyDown();
		getKeyUp();

		moveXSpeed = 0;
		moveYSpeed = 0;

		if (keyboard.shift) moveWait = 5;
		else if (keyboard.lctrl) moveWait = 20;
		else moveWait = 10;

		if (movingUp)
		{
			moveYSpeed = -1;
		}
		if (movingDown)
		{
			moveYSpeed = 1;
		}
		if (movingLeft)
		{
			moveXSpeed = -1;
		}
		if (movingRight)
		{
			moveXSpeed = 1;
		}

		if (moveXSpeed != 0 || moveYSpeed != 0)
		{
			if (moveTimer == 0)
			{
				if (player->position.x + moveXSpeed >= 0 &&
					player->position.y + moveYSpeed >= 0 &&
					player->position.x + moveXSpeed < engine.gui->mapWindow->map->debugmap.mapW &&
					player->position.y + moveYSpeed < engine.gui->mapWindow->map->debugmap.mapH)
				{
					if (engine.gui->mapWindow->map->getWalkability(player->position.x + moveXSpeed, player->position.y))
					{
						player->position.x += moveXSpeed;
						moveTimer = moveWait;
					}
					if (engine.gui->mapWindow->map->getWalkability(player->position.x, player->position.y + moveYSpeed))
					{
						player->position.y += moveYSpeed;
						moveTimer = moveWait;
					}
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
