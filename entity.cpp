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
	: position(pos), symbol(symbol), name(name), color(color), level(level)
{}

void Entity::render(std::shared_ptr<Pane> window)
{
	window->console->setChar(position.x, position.y, symbol);
	window->console->setCharForeground(position.x, position.y, color);
}

Player::Player(Position pos, int symbol, const char* name, TCODColor color)
	:Entity( pos, symbol, name, color, 0), health(100), height(5)
{}