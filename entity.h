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
	int dx;
	int dy;

	Weapon(double ang);

	void update(int x, int y, int mx, int my);
	void render(std::shared_ptr<Entity> entity, std::shared_ptr<Pane> window);
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
	void render(std::shared_ptr<Entity> entity, std::shared_ptr<Pane> window);
};
