#include "main.hpp"

//Entity Class
class Entity
{
public:
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

//Creature Class
class Creature : public Entity
{
public:
	double angle;

	int health;
	int armor;

	int inventoryIndex;
	std::vector<std::shared_ptr<Container>> inventory;
	std::shared_ptr<Container> currentContainer;

	Creature(Position pos, int symbol, const char* name, TCODColor color, int health, int armor);

	virtual void update();
	void virtual render(const std::shared_ptr<Pane>& pane) const;
};

//Player Class
class Player : public Creature
{
public:
	Player(Position pos);

	void update();
	void render(const std::shared_ptr<Pane>& pane) const;
};
