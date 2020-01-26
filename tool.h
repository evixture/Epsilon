#include "main.hpp"

struct Armor;

struct Tool //base class for the holdable component to items
{
	TCODColor color; //foreground color of tool
	unsigned char ch; //character representation of tool

	MagazineData::AmmoType ammoType; //type of ammo used for weapons
	
	Position4 mapPosition;
	Position4 sourcePosition;

	short int dx; //the delta x of the player and the mouse
	short int dy; //the delta y of the player and the mouse

	std::string name; //string name of tool

	enum FireType { SAFE = 0x01, SEMI = 0x02, FULL = 0x04 } fireMode; //fire mode for the firearm

	Tool(std::string name, TCODColor color, int ch); //constuctor used for generic tool that takes string name, color, and character
	Tool(std::string name, TCODColor color, MagazineData::AmmoType ammoType, FireType fireMode, char availibleFireModeFlag); 

	virtual std::shared_ptr<MagazineData> getMagData(); //returns the magazine component, only useful for magazines

	virtual void reload(std::shared_ptr<MagazineData>& magazine); //does nothing in tool
	virtual void changeFireMode();
	virtual void updateToolPosition(double angle); //takes angle and updates tool position

	virtual void equip(Armor& armor);

	virtual void update(Position4 sourcePosition, int mx, int my, double angle); //virtual updates tool
	virtual void render(const std::shared_ptr<Pane>& pane) const; //virtual renders tool

protected:
	Position4 renderPosition;
	unsigned char availibleFireMode;
};

struct Bullet //bullet that is fired from firearm
{
	unsigned char ch; //character represetation of the bulley

	const short int mass;
	const short int baseVelocity; //tile movements per second
	short int currentVelocity;

	Position4 startPosition;
	Position4 mapPosition;

	bool hitWall; //if the bullet has made contact with the wall yet

	Bullet(int ch, Position4 startPosition, int dx, int dy, int xbound, int ybound, int velocity, int mass); //constructor that takes character, x and y to start, x and y destination, and map bounds

	void update(); //updates bullet
	void render(const std::shared_ptr<Pane>& pane) const; //renders bullet

private:
	short int tox; //x destination of bullet
	short int toy; //y destination of bullet

	int xbound; //CHECK x map bound
	int ybound; //CHECK y map bound

	BLine travel; //bresanham line that the bullet travels along
	Clock moveClock; //clock that determines when bullet moves
	Clock fallClock;

	Position4 renderPosition;
};

struct Firearm : public Tool //firearm that fires bullets that interact with the world
{
	std::shared_ptr<MagazineData> selectedMagazine; //the magazine that is currently in the firearm, is a generic mag if no mag is in firearm

	float maxFireTime; //maximum time in seconds that it takes for the next bullet to fire
	float maxReloadTime; //maximum time in seconds it takes to reload the magazine

	Firearm(std::string name, TCODColor color, float fireRate, float reloadSpeed, MagazineData::AmmoType ammoType, FireType fireMode, char availibleFireModeFlag); //constructor of firearm that takes string name, foreground color, fire rate, reload speed, ammo type, and fire type

	std::shared_ptr<MagazineData> getMagData(); //returns the important data of the selected magazine

	void reload(std::shared_ptr<MagazineData>& magazine); //reloads firearm, use from player
	void changeFireMode();

	void update(Position4 sourcePosition, int mx, int my, double angle); //updates firearm
	void render(const std::shared_ptr<Pane>& pane) const; //renders firearm

private:
	std::vector<std::shared_ptr<Bullet>> bulletList; //private list of bullets that the fire owns

	Clock fireClock; //clock that determines when the firearm fires
	Clock reloadClock; //clock that determines how long it takes for the firearm to reload

	void updateWeaponChar(double angle); //updates the weapon character
	void fireBullet(); //fires a bullet
};

struct Armor : public Tool
{
	unsigned short int defense;
	unsigned short int durability;

	Armor(std::string name, TCODColor color, int defense, int durability);

	void equip(Armor& armor);
};

/*

BODY ARMOR
	int defense value
			based on the tier that the armor is in
	int durability
			based on the quality of the armor

	void update()

	void render()
		uses the tool render function

	void equip armor()

*/