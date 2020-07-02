#include "main.hpp"

//things that dont need to be created on map creation or in the map file ??

//projectiles (how to implement for items????????)
//move entities here?
//move bullets to here
//particles
//smoke effects
//lights??
//boxes (for triggers and whatnot)

/*
PROJECTILE IMPLEMENTATION
	projectile
	L	Bullet
	L	Tool (how does it work if not in inventory?

	ITEM
		tool		//when held
		tile		//when on world
		projectile	//if thrown, if not moving, use tile
*/

/*

struct Tool
{
	const Creature* owner;

	enum class Type { TOOL, MELEE, FIREARM, ARMOR } type;

	TCODColor color;
	unsigned char ch;

	bool isHeld;

	MagazineData::AmmoType ammoType;
	int effectiveRange;

	Position4 mapPosition;
	Position4 sourcePosition;

	int dx;
	int dy;
	double angle;

	std::string name;

	enum FireType { SAFE = 0x01, SEMI = 0x02, FULL = 0x04 } fireMode;

	Tool(const Creature* owner, std::string name, TCODColor color, int ch);
	Tool(const Creature* owner, std::string name, TCODColor color, MagazineData::AmmoType ammoType, FireType fireMode, char availibleFireModeFlag);
	virtual ~Tool() {};

	virtual void update(Position4& sourcePosition, int& targetX, int& targetY, bool& isHeld);
	virtual void render(const Pane& pane) const;

	virtual	std::pair<bool, MagazineData&>	getMagazine();
	virtual bool			reload(MagazineData& magazine);
	virtual void			changeFireMode();
	virtual void			useMelee();
	virtual void			changeBarColor(TCODColor& color);
	virtual void			equip(Armor& armor);
	virtual void			use(bool hold, bool swtch);

protected:
	Position2 renderPosition; //replace with position2
	unsigned char availibleFireMode;

	virtual void updatePositions(Position4& sourcePosition, int& targetX, int& targetY);

private:
	virtual void updateToolPosition(int targetX, int targetY);
};
*/

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

	const short int mass;
	const short int baseVelocity;
	short int currentVelocity;

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