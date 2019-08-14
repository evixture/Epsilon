#include "main.hpp"

class Map
{
public:
	int mapW;
	int mapH;

	std::shared_ptr<Window> window;
	std::shared_ptr<Entity> player;

	Map(int w, int h);
};

