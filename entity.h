#include "main.hpp"

class Input
{
public:

	//events
	TCOD_key_t key;
	TCOD_mouse_t mouse;
	TCOD_event_t event;

	Input();

	//handles input
	void getInp(std::shared_ptr<Entity> entity);
};

class Entity
{
public:

	Position position;
	int ch;

	TCODColor fgcol;

	const char* name;

	Entity(int x, int y, int ch, TCODColor fgcol, const char* name);

	void update();

	void setPosition(int x, int y);

	Position getPosition(std::shared_ptr<Entity> entity);

	virtual void render(TCODConsole* console);
};

class Weapon : public Entity
{
};

class Player : public Entity
{
public:
	int health;

	Player(int x, int y);
};

//TODO : figure out how to make gun work
//TODO : make enemies and behavior
//TODO : create player class
//TODO : NPCs and Interaction