#include "main.hpp"

//Default Entity Class
Entity::Entity(int x, int y, int ch, TCODColor fgcol, const char* name)
	:x(x), y(y), ch(ch), fgcol(fgcol), name(name)
{
}

Entity::~Entity()
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
	return Position();
}

void Entity::render(TCODConsole* console)
{
	console->setChar(x, y, ch);
	console->setCharForeground(x, y, fgcol);
}

//Player Class
Player::Player(int x, int y)
	:Entity(x, y, '@', TCODColor::blue, "Player")
{
}