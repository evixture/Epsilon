#include "main.hpp"

class Entity
{
public:

	int x;
	int y;
	int ch;

	TCODColor fgcol;

	const char* disName;
	
	Entity(int x, int y, int ch, TCODColor fgcol);

	~Entity();

private:
};

