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

Weapon::Weapon(double ang)
	:angle(ang)
{}

void Weapon::render(int x, int y, int mx, int my)
{
	double itan = (double)((my - 3) - y) / (double)((mx - 1) - x);
	//weird range, check actual angle by checking x and y sign with angle
	angle = atan(itan) * 180 / PI;
}

Player::Player(Position pos, int symbol, const char* name, TCODColor color)
	:Entity(pos, symbol, name, color, 0), health(100), armor(0), testWeapon(std::make_shared<Weapon>(0.0))
{}

void Player::update()
{
	testWeapon->render(position.x, position.y, engine->settings->input->mouse.cx, engine->settings->input->mouse.cy);
}

void Player::render(std::shared_ptr<Pane> window)
{
	window->console->setChar(position.x, position.y, symbol);
	window->console->setCharForeground(position.x, position.y, color);
}
