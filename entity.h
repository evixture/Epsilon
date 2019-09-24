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
	int height;

	Entity(Position pos, int symbol, const char* name, TCODColor color, int level);

	void virtual update();
	void virtual render(std::shared_ptr<Pane> window);
};

//change name
struct Weapon
{
	double angle;

	Weapon(double ang);
	void render(int x, int y, int mx, int my);
};

class Player : public Entity
{
public:
	int health;
	int armor;

	std::shared_ptr<Weapon> testWeapon;

	Player(Position pos, int symbol, const char* name, TCODColor color);
	//inventory (struct)

	void update();
	void render(std::shared_ptr<Pane> window);
};
