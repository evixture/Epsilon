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
	void render(std::shared_ptr<Pane> pane);
};

//change name
struct Weapon
{
	TCODColor weaponColor;

	double angle;

	int wx;
	int wy;

	int fireWait;
	int fireCap;

	std::vector<std::shared_ptr<Bullet>> bulletList;

	//int bx;
	//int by;

	Weapon(TCODColor color, int fireWait);

	//void weaponFire(int endx, int endy, std::shared_ptr<Pane> pane, int xbound, int ybound);

	void update(int x, int y, int mx, int my);
	void render(std::shared_ptr<Entity> entity, std::shared_ptr<Pane> window);

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
	void render(std::shared_ptr<Entity> entity, std::shared_ptr<Pane> window);
};
