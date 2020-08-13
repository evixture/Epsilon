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
	enum class Type {PROJECTILE, BULLET} type;

	const Creature* owner;
	bool inFov;

	bool onGround;

	float mass;			//weight is in grams
	float velocity;		//velocity is in fps / 2

	Position4 nextPosition;

	Projectile(const Creature* owner, int ch, std::string name, TCODColor color, const Position4 startPosition, Position2 targetPosition, float velocity, float mass);

	virtual void doProjectileDamage(std::shared_ptr<Creature>& creature); //damage is calculated differently compared to bullet

	virtual void update();
	virtual void render(const Pane& pane) const;

protected:
	const Position4 startPosition;
	const Position2 targetPosition;

	FLine fTravel;

	Clock moveClock;
	Clock fallClock;
};

struct Bullet : public Projectile
{
	//check if use ref
	Bullet(const Creature* owner, std::string name, int ch, const Position4 startPosition, Position2 targetPosition, float velocity, float mass);

	void doProjectileDamage(std::shared_ptr<Creature>& creature); //damage is calculated differently compared to projectile

	void update();
	void render(const Pane& pane) const;
};