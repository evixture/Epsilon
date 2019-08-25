#include "main.hpp"

//Position Struct
Position::Position(int x, int y)
	:x(x), y(y)
{
}

//Default Entity Class
Entity::Entity(Position pos, int symbol, const char* name, TCODColor color)
	: position(pos), symbol(symbol), name(name), color(color)
{
}

void Entity::render(std::shared_ptr<Window> window)
{
	window->console->setChar(position.x, position.y, symbol);
	window->console->setCharForeground(position.x, position.y, color);
}