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

class Player : public Entity
{
};