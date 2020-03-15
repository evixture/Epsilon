#include "main.hpp"

struct Armor;

struct Tool //base class for the holdable component to items
{
	TCODColor color; //foreground color of tool
	unsigned char ch; //character representation of tool

	bool isHeld; //if the tool is being held by a creature

	MagazineData::AmmoType ammoType; //type of ammo used for weapons
	
	Position4 mapPosition; //the position of the tool on the map
	Position4 sourcePosition; //the position of the creature that is holding the tool

	int dx; //the delta x of the player and the mouse
	int dy; //the delta y of the player and the mouse
	double angle;

	std::string name; //string name of tool

	enum FireType { SAFE = 0x01, SEMI = 0x02, FULL = 0x04 } fireMode; //fire mode for the firearm

	Tool(std::string name, TCODColor color, int ch); //constuctor used for generic tool that takes string name, color, and character
	Tool(std::string name, TCODColor color, MagazineData::AmmoType ammoType, FireType fireMode, char availibleFireModeFlag); 

	virtual void updateToolPosition(int targetX, int targetY); //updates tool position
	virtual void updatePositions(Position4& sourcePosition, int& targetX, int& targetY); //updates the position of the tool

	virtual void update(Position4& sourcePosition, int& targetX, int& targetY, bool& isHeld); //virtual updates tool
	virtual void render(const std::shared_ptr<Pane>& pane) const; //virtual renders tool

	virtual std::shared_ptr<MagazineData> getMagData();				//returns the magazine component, only useful for magazines
	virtual void reload(std::shared_ptr<MagazineData>& magazine);	//does nothing in tool
	virtual void changeFireMode();									//does nothing in tool
	virtual void useMelee();										//does nothing in tool
	virtual void changeBarColor(TCODColor& color);					//does nothing in tool
	virtual void equip(Armor& armor);								//does nothing in tool

protected: //derived has access
	Position4 renderPosition; //the position of the tool in the render window
	unsigned char availibleFireMode; //bit flag of the availible fire modes
};

struct Melee : public Tool
{
	int bluntDamage; //the amount of blunt damage done to a creature by something like a sledgehammer or a fist, best against armor but should be overall less
	int sharpDamage; //the amount of bladed damage done to a creature by something like a knife, not as good against body armor, but should overall be higher

	Melee(Tool tool, int bluntDamage, int sharpDamage);

	void useMelee(); //use the melee weapon
	
	virtual void update(Position4& sourcePosition, int& targetX, int& targetY, bool& isHeld);
	virtual void render(const std::shared_ptr<Pane>& pane) const;

private:
	void doMeleeDamage(std::shared_ptr<Creature>& creature); //deals damage against a creature based on the blunt and sharp damage
};

struct Bullet //bullet that is fired from firearm
{
	unsigned char ch; //character represetation of the bulley

	const short int mass; //the mass of the bullets in grains
	const short int baseVelocity; //tile movements per second
	short int currentVelocity; //the current speed of the bullet

	Position4 startPosition; //the starting position of the bullet on the map
	Position4 mapPosition; //the current position of the bullet on the map

	//check if use ref
	Bullet(int ch, Position4 startPosition, int dx, int dy, int xbound, int ybound, int velocity, int mass); //constructor that takes character, x and y to start, x and y destination, and map bounds

	void doBulletDamage(std::shared_ptr<Creature>& creature); //deals damage to a creature based on the armor, mass, and velocity of the bullet

	void update(); //updates bullet
	void render(const std::shared_ptr<Pane>& pane) const; //renders bullet

private:
	short int tox; //x destination of bullet
	short int toy; //y destination of bullet

	int xbound; //CHECK x map bound
	int ybound; //CHECK y map bound

	BLine travel; //bresanham line that the bullet travels along

	Position4 renderPosition; //the position of the bullet on the render window

	Clock moveClock;
	Clock fallClock;
};

struct Firearm : public Melee //firearm that fires bullets that interact with the world
{
	std::shared_ptr<MagazineData> selectedMagazine; //the magazine that is currently in the firearm, is a generic mag if no mag is in firearm

	int fireRPS; //maximum time in seconds that it takes for the next bullet to fire
	float reloadTime; //maximum time in seconds it takes to reload the magazine

	Firearm(std::string name, TCODColor color, int fireRPS, float reloadSpeed, MagazineData::AmmoType ammoType, FireType fireMode, char availibleFireModeFlag); //constructor of firearm that takes string name, foreground color, fire rate, reload speed, ammo type, and fire type

	std::shared_ptr<MagazineData> getMagData(); //returns the important data of the selected magazine

	void reload(std::shared_ptr<MagazineData>& magazine); //reloads firearm, use from player
	void changeFireMode(); //switches the fire mode

	void changeBarColor(TCODColor& color); //changes the bar color; red on no magazine, green when reloaded

	void updateToolPosition(int targetX, int targetY); //updates the weapon character //replace with derived updateposition from tool

	void update(Position4& sourcePosition, int& targetX, int& targetY, bool& isHeld); //updates firearm
	void render(const std::shared_ptr<Pane>& pane) const; //renders firearm

private:
	std::vector<std::shared_ptr<Bullet>> bulletList; //private list of bullets that the fire owns

	Clock fireClock;
	Clock reloadClock;

	void fireBullet(); //fires a bullet
};

struct Armor : public Tool
{
	short int defense; //the velocity of bullet that the armor can stop
	short int durability; //the amount of hits based on mass the armor can take

	Armor(std::string name, TCODColor color, int defense, int durability);

	void equip(Armor& armor); //applies the armor to the player
};

namespace ep
{
	struct tool
	{
		inline static const Tool smallBackpack =	Tool("Small Backpack", ep::color::smallBackpackFG, ep::character::backpack);
		inline static const Tool cal45magazine7 =	Tool("45 Magazine -7-", ep::color::pistolFG, ep::character::pistolMagazine);
		inline static const Tool cal556magazine30 = Tool("5.56 Magazine -30-", ep::color::rifleFG, ep::character::rifleMagazine);
		inline static const Melee hands =			Melee(Tool("Hands", ep::color::handFG, TCOD_CHAR_UMLAUT), 30, 20);
		inline static const Melee knife =			Melee(Tool("-Test Knife 34 damage-", TCODColor::silver, ep::character::knife), 0, 34);
		inline static const Firearm sip45 =			Firearm("SIP45", ep::color::pistolFG, 2, 1.0f, MagazineData::AmmoType::FOURTYFIVEACP, Firearm::FireType::SEMI, Firearm::FireType::SEMI | Firearm::FireType::SAFE);
		inline static const Firearm sir556 =		Firearm("SIR556", ep::color::rifleFG, 10, 2.0f, MagazineData::AmmoType::FIVEPOINTFIVESIX, Firearm::FireType::FULL, Firearm::FireType::FULL | Firearm::FireType::SEMI | Firearm::FireType::SAFE);
		inline static const Armor L1R3Armor =		Armor("test armor", TCODColor::black, 100, 300);
	};

	struct bullet
	{
		inline static Bullet cal45(int ch, Position4 startPosition, int dx, int dy, int xbound, int ybound)
		{
			return Bullet(ch, startPosition, dx, dy, xbound, ybound, 80, 230);
		}

		inline static Bullet cal556(int ch, Position4 startPosition, int dx, int dy, int xbound, int ybound)
		{
			return Bullet(ch, startPosition, dx, dy, xbound, ybound, 300, 55);
		}
	};
}