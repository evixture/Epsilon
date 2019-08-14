#include "main.hpp"

Position::Position(int x, int y)
	:x(x), y(y)
{
}

Entity::Entity(Position pos, int symbol, const char* name, TCODColor color)
	:position(pos), symbol(symbol), name(name), color(color)
{
}

void Entity::render(TCODConsole console)
{
	console.setChar(position.x, position.y, symbol);
	console.setCharForeground(position.x, position.y, color);
}