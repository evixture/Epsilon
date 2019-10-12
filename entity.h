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
	std::shared_ptr<Weapon> testWeapon;
	//, int symbol, const char* name, TCODColor color
	Player(Position pos);
	//inventory (struct)

	void update();
	void render(const std::shared_ptr<Pane>& pane) const;
};
