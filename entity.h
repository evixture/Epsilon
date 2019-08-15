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
	const TCODColor color;
	const char* name;

	Entity(Position pos, int symbol, const char* name, TCODColor color);

	void render(std::shared_ptr<Window> window);
};