#include "main.hpp"

//input class
Input::Input()
	:key(TCODConsole::checkForKeypress(TCOD_KEY_PRESSED))
{
}

void Input::getInp(std::shared_ptr<Entity> entity)
{
	TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS | TCOD_EVENT_MOUSE, &key, &mouse);

	if (engine.gameState == engine.enMAIN)
	{
		int ix = 0;
		int iy = 0;

		switch (key.vk)
		{
		case TCODK_UP:
			iy = -1;
			break;
		case TCODK_DOWN:
			iy = 1;
			break;
		case TCODK_LEFT:
			ix = -1;
			break;
		case TCODK_RIGHT:
			ix = 1;
			break;
		default:
			break;
		}

		switch (key.c)
		{
		case 'w':
			iy = -1;
			break;
		case 's':
			iy = 1;
			break;
		case 'a':
			ix = -1;
			break;
		case 'd':
			ix = 1;
			break;
		default:
			break;
		}

		if (ix != 0 || iy != 0)
		{
			if ()
				entity->position.x += ix;
			entity->position.y += iy;
		}
	}
}

//Default Entity Class
Entity::Entity(int x, int y, int ch, TCODColor fgcol, const char* name)
	:position(Position(x, y)), ch(ch), fgcol(fgcol), name(name)
{
}

void Entity::update()
{
}

void Entity::setPosition(int x, int y)
{
}

Position Entity::getPosition(std::shared_ptr<Entity> entity)
{
	return position;
}

void Entity::render(TCODConsole* console)
{
	console->setChar(position.x, position.y, ch);
	console->setCharForeground(position.x, position.y, fgcol);
}

//Player Class
Player::Player(int x, int y)
	:Entity(x, y, '@', TCODColor::blue, "Player")
{
}