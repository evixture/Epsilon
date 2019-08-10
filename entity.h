#include "main.hpp"

struct Position
{
	int x;
	int y;
};

class Entity
{
public:

	int x;
	int y;
	int ch;

	TCODColor fgcol;

	const char* name;

	Entity(int x, int y, int ch, TCODColor fgcol, const char* name);
	~Entity();

	void update();
	void setPosition(int x, int y);

	Position getPosition(std::shared_ptr<Entity> entity);

	virtual void render(TCODConsole* console);
};

class Player : public Entity
{
public:

	Player(int x, int y);
};

//class Player : public Entity
//{
//};
//
//class Enemy : public Entity
//{
//};
//
//class Item : public Entity
//{
//};

//TODO : add set position and other functions; make get pos return position struct
//TODO : figure out how to make gun work
//TODO : make enemies and behavior
//TODO : create player class
//TODO : NPCs and Interaction