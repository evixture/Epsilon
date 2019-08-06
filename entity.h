#include "main.hpp"

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

	void render();

private:
};

//TODO : add set position and other functions; make get pos return position struct
//TODO : figure out how to make gun work
//TODO : make enemies and behavior
//TODO : create player class
//TODO : NPCs and Interaction