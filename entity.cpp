#include "main.hpp"

//Position Struct
Position::Position(int x, int y)
	:x(x), y(y)
{
}

Position Position::getPosition()
{
	return Position(x, y);
}

void Position::setPosition(int nx, int ny)
{
	this->x = nx;
	this->y = ny;
}

//Default Entity Class
Entity::Entity(Position pos, int symbol, const char* name, TCODColor color, int level)
	: position(pos), symbol(symbol), name(name), color(color), level(level), height(3)
{}

void Entity::update()
{
	return;
}

void Entity::render(std::shared_ptr<Pane> window)
{
	window->console->setChar(position.x, position.y, symbol);
	window->console->setCharForeground(position.x, position.y, color);
}

Weapon::Weapon(TCODColor color)
	:weaponColor(color), angle(0), dx(0), dy(0)
{}

void Weapon::update(int x, int y, int mx, int my)
{
	dx = (mx - 1) - x;
	dy = (my - 3) - y;
	double itan = (double)(dy) / (double)(dx);
	//weird range, check actual angle by checking x and y sign with angle
	angle = atan(itan) * 180 / PI;
}

void Weapon::render(std::shared_ptr<Entity> entity, std::shared_ptr<Pane> window)
{
	/*
	dx and dy values
	....|....
	.-..|..+.
	.-..|..-.
	....|....
	----@----
	....|....
	.-..|..+.
	.+..|..+.
	....|....
	*/
	
	if (dx >= 0 && dy >= 0)
	{
		if (angle <= 22.5)
		{
			window->console->setChar(entity->position.x + 1, entity->position.y, TCOD_CHAR_HLINE);
			window->console->setCharForeground(entity->position.x + 1, entity->position.y, weaponColor);
		}
		else if (angle >= 22.5 && angle <= 67.5)
		{
			window->console->setChar(entity->position.x + 1, entity->position.y + 1, '\\');
			window->console->setCharForeground(entity->position.x + 1, entity->position.y + 1, weaponColor);
		}
		else if (angle >= 67.5)
		{
			window->console->setChar(entity->position.x, entity->position.y + 1, TCOD_CHAR_VLINE);
			window->console->setCharForeground(entity->position.x, entity->position.y + 1, weaponColor);
		}
	}
	else if (dx >= 0 && dy <= 0)
	{
		if (angle >= -22.5)
		{
			window->console->setChar(entity->position.x + 1, entity->position.y, TCOD_CHAR_HLINE);
			window->console->setCharForeground(entity->position.x + 1, entity->position.y, weaponColor);
		}
		else if (angle <= -22.5 && angle >= -67.5)
		{
			window->console->setChar(entity->position.x + 1, entity->position.y - 1, '/');
			window->console->setCharForeground(entity->position.x + 1, entity->position.y - 1, weaponColor);
		}
		else if (angle <= -67.5)
		{
			window->console->setChar(entity->position.x, entity->position.y - 1, TCOD_CHAR_VLINE);
			window->console->setCharForeground(entity->position.x, entity->position.y - 1, weaponColor);
		}
	}
	else if (dx <= 0 && dy >= 0)
	{
		if (angle >= -22.5)
		{
			window->console->setChar(entity->position.x - 1, entity->position.y, TCOD_CHAR_HLINE);
			window->console->setCharForeground(entity->position.x - 1, entity->position.y, weaponColor);
		}
		else if (angle <= -22.5 && angle >= -67.5)
		{
			window->console->setChar(entity->position.x - 1, entity->position.y + 1, '/');
			window->console->setCharForeground(entity->position.x - 1, entity->position.y + 1, weaponColor);
		}
		else if (angle <= -67.5)
		{
			window->console->setChar(entity->position.x, entity->position.y + 1, TCOD_CHAR_VLINE);
			window->console->setCharForeground(entity->position.x, entity->position.y + 1, weaponColor);
		}
	}
	else if (dx <= 0 && dy <= 0)
	{
		if (angle <= 22.5)
		{
			window->console->setChar(entity->position.x - 1, entity->position.y, TCOD_CHAR_HLINE);
			window->console->setCharForeground(entity->position.x - 1, entity->position.y, weaponColor);
		}
		else if (angle >= 22.5 && angle <= 67.5)
		{
			window->console->setChar(entity->position.x - 1, entity->position.y - 1, '\\');
			window->console->setCharForeground(entity->position.x - 1, entity->position.y - 1, weaponColor);
		}
		else if (angle >= 67.5)
		{
			window->console->setChar(entity->position.x, entity->position.y - 1, TCOD_CHAR_VLINE);
			window->console->setCharForeground(entity->position.x, entity->position.y - 1, weaponColor);
		}
	}
}

Player::Player(Position pos, int symbol, const char* name, TCODColor color)
	:Entity(pos, symbol, name, color, 0), health(100), armor(0), testWeapon(std::make_shared<Weapon>(TCODColor::lighterGrey))
{}

void Player::update()
{
	testWeapon->update(position.x, position.y, engine->settings->input->mouse.cx, engine->settings->input->mouse.cy);
}

void Player::render(std::shared_ptr<Entity> entity, std::shared_ptr<Pane> window)
{
	window->console->setChar(position.x, position.y, symbol);
	window->console->setCharForeground(position.x, position.y, color);
	testWeapon->render(entity, window);
}
