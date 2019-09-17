#include "main.hpp"

//Position Struct
struct Position
{
	int x;
	int y;

	Position(int x, int y);

	Position getPosition();
	void setPosition(int x, int y);
};

//Default Entity Class
class Entity
{
public:
	Position position;
	int symbol;
	const TCODColor color;
	const char* name;

	int level;

	Entity(Position pos, int symbol, const char* name, TCODColor color, int level);

	void render(std::shared_ptr<Pane> window);
};

class Player : public Entity
{
public:
	int health;
	int height;

	Player(Position pos, int symbol, const char* name, TCODColor color);
	//inventory (struct)
};
