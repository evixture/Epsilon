#include "main.hpp"

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

void Entity::render()
{
	TCODConsole::root->setChar(x, y, ch);
	TCODConsole::root->setCharForeground(x, y, fgcol);
}