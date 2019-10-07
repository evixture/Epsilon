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
	void virtual render(const std::shared_ptr<Pane> &pane) const;
};

struct Bullet
{
	int bx;
	int by;

	int tox;
	int toy;

	int xbound;
	int ybound;

	bool hitWall;

	BLine travel;

	Bullet(int startx, int starty, int dx, int dy, int xbound, int ybound);

	void update();
	void render(const std::shared_ptr<Pane>& pane) const;
};

//change name
struct Weapon
{
	TCODColor weaponColor;

	double angle;

	//later make magazine class
	int numberMags;

	int ammoCap;
	int ammoAmount;

	int fireWait;
	int fireCap;

	//display
	int wx;
	int wy;
	int ch;

	std::vector<std::shared_ptr<Bullet>> bulletList;

	Weapon(TCODColor color, int ammoCap, int numberMags, int fireRate);

	//void weaponFire(int endx, int endy, std::shared_ptr<Pane> pane, int xbound, int ybound);

	void update(int x, int y, int mx, int my);
	void render(std::shared_ptr<Entity> entity, const std::shared_ptr<Pane>& pane) const;

private:

	int dx;
	int dy;

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
	void render(std::shared_ptr<Entity> entity, const std::shared_ptr<Pane>& pane) const;
};
