#include "main.hpp"

struct Tool //base class for the holdable component to items
{
	TCODColor color; //foreground color of tool
	int ch; //character representation of tool

	MagazineData::AmmoType ammoType; //type of ammo used for weapons
	
	//int toolx; //x coordinate of tool on the map
	//int tooly; //y coordinate of tool on the map
	Position mapPosition;

	//int sourcex; //player x position
	//int sourcey; //player y position
	Position sourcePosition;

	int dx; //the delta x of the player and the mouse
	int dy; //the delta y of the player and the mouse

	std::string name; //string name of tool

	Tool(std::string name, TCODColor color, int ch); //constuctor used for generic tool that takes string name, color, and character
	Tool(std::string name, TCODColor color, int ch, MagazineData::AmmoType ammoType); //constructor used for weapons that takes string name, color, character, and ammo type

	virtual std::shared_ptr<MagazineData> getMagData(); //returns the magazine component, only useful for magazines

	virtual void reload(std::shared_ptr<MagazineData>& magazine); //does nothing in tool
	virtual void changeFireMode();
	virtual void updateToolPosition(double angle); //takes angle and updates tool position

	virtual void update(int x, int y, int mx, int my, double angle); //virtual updates tool
	virtual void render(const std::shared_ptr<Pane>& pane) const; //virtual renders tool

protected:
	Position renderPosition;
};

struct Bullet //bullet that is fired from firearm
{
	int ch; //character represetation of the bulley

	//int x; //x coordinate of the bullet
	//int y; //y coordinate of the bullet
	Position startPosition;

	bool hitWall; //if the bullet has made contact with the wall yet

	Bullet(int ch, int startx, int starty, int dx, int dy, int xbound, int ybound); //constructor that takes character, x and y to start, x and y destination, and map bounds

	void update(); //updates bullet
	void render(const std::shared_ptr<Pane>& pane) const; //renders bullet

private:
	int tox; //x destination of bullet
	int toy; //y destination of bullet

	int xbound; //x map bound
	int ybound; //y map bound

	BLine travel; //bresanham line that the bullet travels along
	Clock moveClock; //clock that determines when bullet moves

	Position renderPosition;
};

struct Firearm : public Tool //firearm that fires bullets that interact with the world
{
	enum FireType {SAFE = 0x01, SEMI = 0x02, FULL = 0x04} fireMode; //fire mode for the firearm
	unsigned char availibleFireMode;

	std::shared_ptr<MagazineData> selectedMagazine; //the magazine that is currently in the firearm, is a generic mag if no mag is in firearm

	float maxFireTime; //maximum time in seconds that it takes for the next bullet to fire
	float maxReloadTime; //maximum time in seconds it takes to reload the magazine

	Firearm(std::string name, TCODColor color, float fireRate, float reloadSpeed, MagazineData::AmmoType ammoType, FireType fireMode, char availibleFireModeFlag); //constructor of firearm that takes string name, foreground color, fire rate, reload speed, ammo type, and fire type

	std::shared_ptr<MagazineData> getMagData(); //returns the important data of the selected magazine

	void reload(std::shared_ptr<MagazineData>& magazine); //reloads firearm, use from player
	void changeFireMode();

	void update(int x, int y, int mx, int my, double angle); //updates firearm
	void render(const std::shared_ptr<Pane>& pane) const; //renders firearm

private:
	std::vector<std::shared_ptr<Bullet>> bulletList; //private list of bullets that the fire owns

	Clock fireClock; //clock that determines when the firearm fires
	Clock reloadClock; //clock that determines how long it takes for the firearm to reload

	void updateWeaponChar(double angle); //updates the weapon character
	void fireBullet(); //fires a bullet
};

