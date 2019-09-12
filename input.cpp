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

//TODO : figure out how to get char and vk switches working
void Input::getKeyDown()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) movingUp = true;
	else movingUp = false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movingDown = true;
	else movingDown = false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) movingLeft = true;
	else movingLeft = false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) movingRight = true;
	else movingRight = false;	

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) movingUp = true;
	else movingUp = false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) movingDown = true;
	else movingDown = false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) movingLeft = true;
	else movingLeft = false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) movingRight = true;
	else movingRight = false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F11)) engine.settings->setFullscreen();

	/*
	case TCODK_F11:
		engine.settings->setFullscreen();
		break;
	}*/
}


//void Input::getKeyUp()
//{
//	if (keyEvent == TCOD_KEY_RELEASED)
//	{
//		switch (keyboard.c)
//		{
//		case 'i':
//			if (movingUp)
//			{
//				movingUp = false;
//				std::cout << "up released" << std::endl;
//			}
//			break;
//		case 'k':
//			if (movingDown)
//			{
//				movingDown = false;
//				std::cout << "down released" << std::endl;
//			}
//			break;
//		case 'j':
//			if (movingLeft)
//			{
//				movingLeft = false;
//				std::cout << "left released" << std::endl;
//			}
//			break;
//		case 'l':
//			if (movingRight)
//			{
//				movingRight = false;
//				std::cout << "right released" << std::endl;
//			}
//			break;
//		}
//
//		switch (keyboard.vk)
//		{
//		case TCODK_UP:
//			if (movingUp)
//			{
//			movingUp = false;
//			std::cout << "up released" << std::endl;
//			}
//			break;
//		case TCODK_DOWN:
//			if (movingDown)
//			{
//			movingDown = false;
//			std::cout << "down released" << std::endl;
//			}
//			break;
//		case TCODK_LEFT:
//			if (movingLeft)
//			{
//			movingLeft = false;
//			std::cout << "left released" << std::endl;
//			}
//			break;
//		case TCODK_RIGHT:
//			if (movingRight)
//			{
//			movingRight = false;
//			std::cout << "right released" << std::endl;
//			}
//			break;
//		}
//	}
//}

void Input::getKeyInput(std::shared_ptr<Player> player)
{
	keyEvent = TCODSystem::checkForEvent(TCOD_EVENT_ANY, &keyboard, &mouse);

	if (engine.gamestate == Engine::MAIN)
	{
		getKeyDown();

		/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			movingUp = true;
			std::cout << "up press" << std::endl;
		}*/
		/*else
		{
			movingUp = false;
		}*/

		if (keyboard.shift) moveWait = 5;
		else if (keyboard.lctrl) moveWait = 20;
		else moveWait = 10;

		moveXSpeed = 0;
		moveYSpeed = 0;
		
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
				if (engine.gui->mapWindow->map->getWalkability(player->position.x + moveXSpeed, player->position.y))
				{
					player->position.x += moveXSpeed;
					moveXSpeed = 0;
					moveTimer = moveWait;
				}
				if (engine.gui->mapWindow->map->getWalkability(player->position.x, player->position.y + moveYSpeed))
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
