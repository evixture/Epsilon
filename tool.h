#include "main.hpp"

struct Tool
{
	TCODColor color;
	int ch;

	MagazineData::AmmoType ammoType;
	
	int toolx;
	int tooly;

	int dx;
	int dy;

	int sourcex;
	int sourcey;

	std::string name;

	Tool(std::string name, TCODColor color, int ch);
	Tool(std::string name, TCODColor color, int ch, MagazineData::AmmoType ammoType);

	virtual std::shared_ptr<MagazineData> getMagData();

	virtual void reload(std::shared_ptr<MagazineData>& magazine);
	virtual void updateToolPosition(double angle);
	virtual void update(int x, int y, int mx, int my, double angle);

	virtual void render(const std::shared_ptr<Pane>& pane) const;
};

struct Bullet
{
	int ch;

	int bulletx;
	int bullety;

	bool hitWall;

	//Position renderPosition;

	Bullet(int ch, int startx, int starty, int dx, int dy, int xbound, int ybound);

	void update();
	void render(const std::shared_ptr<Pane>& pane) const;

private:
	int tox;
	int toy;

	int xbound;
	int ybound;

	BLine travel;
	Clock moveClock;
};



//change name
struct Weapon : public Tool
{
	enum class FireType {FULL, SEMI, SAFE} fireType;

	std::shared_ptr<MagazineData> selectedMagazine;

	float baseFireCap;
	float baseReloadTimer;

	Weapon(std::string name, TCODColor color, int ammoCap, int numberMags, float fireRate, float reloadSpeed, MagazineData::AmmoType ammoType, FireType fireType);

	std::shared_ptr<MagazineData> getMagData();

	void reload(std::shared_ptr<MagazineData>& magazine);
	void update(int x, int y, int mx, int my, double angle);

	void render(const std::shared_ptr<Pane>& pane) const;

private:
	std::vector<std::shared_ptr<Bullet>> bulletList;

	Clock fireClock;
	Clock reloadClock;

	void updateWeaponChar(double angle);
	void fireBullet();
};

