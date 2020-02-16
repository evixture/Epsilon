#include "main.hpp"

struct Position3 //position struct with x, y, and z dimensions
{
	int x; //x coordinate
	int y; //y coordinate
	int floor; //z / height coordinate

	Position3() {}
	Position3(int x, int y, int floor); //position constructor that takes x, y, and floor ints

	bool operator == (const Position3& compPosition);
};

struct Position4 : public Position3
{
	char height;

	Position4() {}
	Position4(int x, int y, int height, int floor); //position constructor that takes x, y, and floor ints

	bool operator == (const Position4& compPosition);
};

Position4 offsetPosition(Position4 mapPosition, int xOffset, int yOffset); //used to calculate render position from map offsets

struct BLine //line algorithm used for bullet paths
{
	int x; //x position on the line
	int y; //y position on the line

	BLine(int ix, int iy, int tx, int ty); //bline constructor that takes start x and y, and target x and y

	void step(); //steps forward on the line

	bool end(); //checks if the line step has reached the end

private:
	int stepx,  stepy,  e, //used for line calculations
		deltax, deltay, origx,
		origy,  destx,  desty;
};

struct Clock
{
	float numCalls;
	float timeBetweenUpdates;

	Clock(float timeBetweenUpdates);

	void tickUp();
	void addTime(float seconds);
};

unsigned char	heightToBitFlag(int height);
double			getAngle(int ix, int iy, int tx, int ty); //gets the angle from 2 sets of coordinates
double			getDistance(int ix, int iy, int tx, int ty); //gets the distance from 2 sets of coordinates
float			getFallTime(int height);

struct MagazineData //contains all of the important data for magazines
{
	enum class AmmoType { NONE, FOURTYFIVEACP, FIVEPOINTFIVESIX } ammoType; //the type of ammo that the magazine uses

	unsigned short int ammoCapacity; //the total capacity of the magazine
	int availableAmmo; //how much ammo is availible for use in the magazine

	bool isValid; //if the magazine is valid - actually contains ammo

	MagazineData(AmmoType ammoType, int ammoCapacity, int availableAmmo, bool isValid = true); //magazine data constructor that takes ammo type, capacity, availible ammo, and validity
};

struct Menu
{
	std::string menuSelection;

	Menu(std::vector<std::string> menuList);

	void update();
	void render(const std::shared_ptr<Pane>& pane, const int rx, const int ry) const;

private:
	unsigned int menuIndex;
	std::vector<std::string> menuList;
};
