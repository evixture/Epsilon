#include "entity.h"

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

void Entity::render()
{
	TCODConsole::root->setChar(x, y, ch);
	TCODConsole::root->setCharForeground(x, y, fgcol);
}