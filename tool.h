#include "main.hpp"

/*
	Firearm
	|	Bullet
	L	Melee
		L	Tool

	Armor
	L	Tool
*/

struct Armor;

struct Tool																		
{
	enum class Type { TOOL, MELEE, FIREARM, ARMOR } type;

	TCODColor color;															
	unsigned char ch;															

	bool isHeld;																

	MagazineData::AmmoType ammoType;											
	
	Position4 mapPosition;														
	Position4 sourcePosition;													

	int dx;																		
	int dy;																		
	double angle;

	std::string name;															

	enum FireType { SAFE = 0x01, SEMI = 0x02, FULL = 0x04 } fireMode;			

	Tool(std::string name, TCODColor color, int ch);							
	Tool(std::string name, TCODColor color, MagazineData::AmmoType ammoType, FireType fireMode, char availibleFireModeFlag); 

	virtual void update(Position4& sourcePosition, int& targetX, int& targetY, bool& isHeld);
	virtual void render(const Pane& pane) const;								

	virtual	MagazineData&	getMagazine();										
	virtual void			reload(MagazineData& magazine);						
	virtual void			changeFireMode();									
	virtual void			useMelee();											
	virtual void			changeBarColor(TCODColor& color);					
	virtual void			equip(Armor& armor);								
	virtual void			use(bool hold, bool swtch);							

protected:
	Position4 renderPosition;													
	unsigned char availibleFireMode;											

	virtual void updatePositions(Position4& sourcePosition, int& targetX, int& targetY);

private:
	virtual void updateToolPosition(int targetX, int targetY);
};

struct Melee : public Tool
{
	int bluntDamage;														
	int sharpDamage;														

	Melee(Tool tool, int bluntDamage, int sharpDamage);

	void useMelee();														

	virtual void update(Position4& sourcePosition, int& targetX, int& targetY, bool& isHeld);
	virtual void render(const Pane& pane) const;

private:
	void doMeleeDamage(std::shared_ptr<Creature>& creature);					
};

struct Bullet																	
{
	unsigned char ch;															

	const short int mass;														
	const short int baseVelocity;												
	short int currentVelocity;													

	Position4 startPosition;													
	Position4 mapPosition;														

	//check if use ref
	Bullet(int ch, Position4 startPosition, int dx, int dy, int xbound, int ybound, int velocity, int mass);

	void doBulletDamage(std::shared_ptr<Creature>& creature);					

	void update();																
	void render(const Pane& pane) const;										

private:
	short int tox;																
	short int toy;																

	int xbound;																	
	int ybound;																	

	BLine travel;																

	Position4 renderPosition;													

	Clock moveClock;
	Clock fallClock;
};

struct Firearm : public Melee													
{
	MagazineData usedMag;														

	int fireRPS;																
	float reloadTime;															

	Firearm(std::string name, TCODColor color, int fireRPS, float reloadSpeed, MagazineData::AmmoType ammoType, FireType fireMode, char availibleFireModeFlag);

	MagazineData& getMagazine();												

	void reload(MagazineData& magazine);										
	void changeFireMode();														
	void use(bool hold, bool swtch);											

	void changeBarColor(TCODColor& color);

	void updateToolPosition(int xTarget, int yTarget);

	void update(Position4& sourcePosition, int& xTarget, int& yTarget, bool& isHeld);
	void render(const Pane& pane) const;

private:
	std::vector<std::shared_ptr<Bullet>> bulletList;

	Clock fireClock;
	Clock reloadClock;

	void fireBullet();
};

struct Armor : public Tool
{
	short int defense;
	short int durability;

	Armor(std::string name, TCODColor color, int defense, int durability);

	void equip(Armor& armor);
};

namespace ep
{
	struct tool
	{
		inline static const Tool smallBackpack =	Tool("Small Backpack",		ep::color::smallBackpackFG, ep::character::backpack);
		inline static const Tool cal45magazine7 =	Tool("45 Magazine -7-",		ep::color::pistolFG, ep::character::pistolMagazine);
		inline static const Tool cal556magazine30 = Tool("5.56 Magazine -30-",	ep::color::rifleFG, ep::character::rifleMagazine);
		inline static const Melee hands =			Melee(Tool("Hands",			ep::color::handFG, TCOD_CHAR_UMLAUT), 30, 20);
		inline static const Melee knife =			Melee(Tool("-Test Knife 34 damage-", TCODColor::silver, ep::character::knife), 0, 34);
		inline static const Firearm sip45 =			Firearm("SIP45",			ep::color::pistolFG, 5, 1.0f, MagazineData::AmmoType::FOURTYFIVEACP, Firearm::FireType::SEMI, Firearm::FireType::SEMI | Firearm::FireType::SAFE);
		inline static const Firearm sir556 =		Firearm("SIR556",			ep::color::rifleFG, 10, 2.0f, MagazineData::AmmoType::FIVEPOINTFIVESIX, Firearm::FireType::FULL, Firearm::FireType::FULL | Firearm::FireType::SEMI | Firearm::FireType::SAFE);
		inline static const Armor L1R3Armor =		Armor("test armor",			TCODColor::black, 100, 300);
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