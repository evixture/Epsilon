#include "main.hpp"

struct Position
{
	int x;
	int y;

	Position(int x, int y);
};

class Entity
{
public:
	Position position;
	int symbol;
	TCODColor color;
	const char* name;

	Entity(Position pos, int symbol, const char* name, TCODColor color);

	void render(TCODConsole console);
};