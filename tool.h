#include "main.hpp"

/*
	Firearm
	|	Bullet
	L	Melee
		L	Tool

	Armor
	L	Tool
*/

/*
	TOOL RANGES
	Melee	: 0
	Firearm	: Depends on ammo type

	General rule, depends on ammo and aggression :
		Pistols : 10
		SMG		: 15
		Rifle	: 20
		DMR		: 25
		Sniper	: 30

		in general, neutral range ~15??
*/

struct Armor;

struct Tool	//needs cleanup
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

	enum FireType { SAFE = 0x01, SEMI = 0x02, FULL = 0x04 } fireMode; //move to firearm only??

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
	Position2 renderPosition;
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
	virtual ~Melee() {};

	void useMelee();														

	virtual void update(Position4& sourcePosition, int& targetX, int& targetY, bool& isHeld);
	virtual void render(const Pane& pane) const;

private:
	void doMeleeDamage(std::shared_ptr<Creature>& creature);					
};

struct Firearm : public Melee													
{
	MagazineData usedMag;														

	int fireRPS;																
	float reloadTime;															

	Firearm(const Creature* owner, std::string name, TCODColor color, int fireRPS, float reloadSpeed, MagazineData::AmmoType ammoType, FireType fireMode, char availibleFireModeFlag);

	std::pair<bool, MagazineData&> getMagazine();

	bool reload(MagazineData& magazine);										
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
	float defense;
	float durability;

	Armor(const Creature* owner, std::string name, TCODColor color, float defense, float durability);

	void equip(Armor& armor);
};

namespace ep
{
	struct tool
	{
		inline static Tool smallBackpack(const Creature* owner)
		{
			return Tool(owner, "Small Backpack", ep::color::smallBackpackFG, ep::character::backpack);
		}
		inline static Tool cal45magazine7(const Creature* owner)
		{
			return Tool(owner, "45 Magazine -7-", ep::color::pistolFG, ep::character::pistolMagazine);
		}
		inline static Tool cal556magazine30(const Creature* owner)
		{
			return Tool(owner, "5.56 Magazine -30-", ep::color::rifleFG, ep::character::rifleMagazine);
		}
		inline static Melee hands(const Creature* owner)
		{
			return Melee(Tool(owner, "Hands", ep::color::handFG, TCOD_CHAR_UMLAUT), 30, 20);
		}
		inline static Melee knife(const Creature* owner)
		{
			return Melee(Tool(owner, "Knife", TCODColor::silver, ep::character::knife), 0, 34);
		}
		inline static Firearm sip45(const Creature* owner)
		{
			return Firearm(owner, "SIP45", ep::color::pistolFG, 5, 1.0f, MagazineData::AmmoType::FOURTYFIVEACP, Firearm::FireType::SEMI, Firearm::FireType::SEMI | Firearm::FireType::SAFE);
		}
		inline static Firearm sir556(const Creature* owner)
		{
			return Firearm(owner, "SIR556", ep::color::rifleFG, 10, 2.0f, MagazineData::AmmoType::FIVEPOINTFIVESIX, Firearm::FireType::FULL, Firearm::FireType::FULL | Firearm::FireType::SEMI | Firearm::FireType::SAFE);
		}
		inline static Armor L1R3Armor(const Creature* owner)
		{
			return Armor(owner, "L1R3 Armor", TCODColor::black, 500.0f, 100.0f);
		}
	};

	struct bullet
	{
		inline static Bullet cal45(const Creature* owner, int ch, Position4 startPosition, Position2 targetPosition)
		{
			return Bullet(owner, "cal45", ch, startPosition, targetPosition, 400.0f, 15.0f);
		}
		inline static Bullet cal556(const Creature* owner, int ch, Position4 startPosition, Position2 targetPosition)
		{
			return Bullet(owner, "cal556", ch, startPosition, targetPosition, 1500.0f, 3.5f);
		}
	};
}