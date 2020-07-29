#include "main.hpp"

//things that dont need to be created on map creation or in the map file ??

struct Entity
{
	Position4 mapPosition;
	unsigned char ch;
	TCODColor color;
	std::string name;

	Entity(Position4 pos, int ch, std::string name, TCODColor color);
	virtual ~Entity() {};

	virtual void update();
	virtual void render(const Pane& pane) const;

protected:
	Position2 renderPosition;
};

struct Projectile : public Entity
{
	const Creature* owner;
	bool inFov;

	bool onGround;

	const short int mass;			//weight is in grams??
	const short int baseVelocity;	//velocity is in fps / 2
	short int currentVelocity;		//velocity is in fps / 2

	const Position4 startPosition;

	Projectile(const Creature* owner, int ch, std::string name, TCODColor color, const Position4 startPosition, Position2 targetPosition, int velocity, int mass);

	virtual void doProjectileDamage(std::shared_ptr<Creature>& creature);

	virtual void update();
	virtual void render(const Pane& pane) const;

protected:
	Position2 targetPosition;
	Position4 nextPosition;

	FLine fTravel;

	Clock moveClock;
	Clock fallClock;
};

struct Bullet : public Projectile
{
	//check if use ref
	Bullet(const Creature* owner, std::string name, int ch, const Position4 startPosition, Position2 targetPosition, int velocity, int mass);

	void doProjectileDamage(std::shared_ptr<Creature>& creature);

	void update();
	void render(const Pane& pane) const;
};