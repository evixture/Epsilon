#include "main.hpp"

struct Tool
{
	TCODColor color;
	int ch;

	MagazineData::AmmoType ammoType;
	
	int toolx;
	int tooly;

	int sourcex;
	int sourcey;

	int dx;
	int dy;

	const char* name;

	Tool(const char* name, TCODColor color, int ch);
	Tool(const char* name, TCODColor color, int ch, MagazineData::AmmoType ammoType);

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

	int tox;
	int toy;

	int xbound;
	int ybound;

	Clock moveClock;

	bool hitWall;

	BLine travel;

	//Position renderPosition;

	Bullet(int ch, int startx, int starty, int dx, int dy, int xbound, int ybound);

	void update();
	void render(const std::shared_ptr<Pane>& pane) const;
};



//change name
struct Weapon : public Tool
{
	enum class FireType {FULL, SEMI, SAFE} fireType;

	/*enum class AmmoType { NONE, FOURTYFIVEACP, FIVEPOINTFIVESIX } ammoType;*/
	//MagazineData::AmmoType ammoType;

	//int numberMags;
	//
	//int ammoCap;
	//int ammoAmount;

	std::shared_ptr<MagazineData> selectedMagazine;

	float baseFireCap;
	Clock fireClock;

	float baseReloadTimer;
	Clock reloadClock;

	std::vector<std::shared_ptr<Bullet>> bulletList;

	Weapon(const char* name, TCODColor color, int ammoCap, int numberMags, float fireRate, float reloadSpeed, MagazineData::AmmoType ammoType, FireType fireType);

	void updateWeaponChar(double angle);

	//WEAPON BEHAVIOR
	std::shared_ptr<MagazineData> getMagData();

	void fireBullet();
	void reload(std::shared_ptr<MagazineData>& magazine);
	void update(int x, int y, int mx, int my, double angle);

	void render(const std::shared_ptr<Pane>& pane) const;
};

