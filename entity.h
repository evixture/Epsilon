#include "main.hpp"

//Position Struct
struct Position
{
	int x;
	int y;
	int level;

	Position(int x, int y, int level);
};

//Default Entity Class
struct Entity
{
	Position mapPosition;
	Position renderPosition;
	int symbol;
	const TCODColor color;
	const char* name;

	int height;

	Entity(Position pos, int symbol, const char* name, TCODColor color);

	virtual void update();
	virtual void render(const std::shared_ptr<Pane> &pane) const;
};

struct Creature : public Entity
{
	double angle;

	int health;
	int armor;

	int inventorySelection;
	std::shared_ptr<std::vector<std::shared_ptr<Container>>> inventory;

	std::shared_ptr<Tool> hands;
	std::vector<std::shared_ptr<Tool>> toolList;
	std::shared_ptr<Tool> currentTool;

	Creature(Position pos, int symbol, const char* name, TCODColor color, int health, int armor);

	virtual void update();
	void virtual render(const std::shared_ptr<Pane>& pane) const;
};

class Player : public Creature
{
public:
	int weaponSelection;

	std::shared_ptr<Weapon> testWeapon;
	Player(Position pos);

	void update();
	void render(const std::shared_ptr<Pane>& pane) const;
};
