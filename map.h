#include "main.hpp"

class Map
{
public:
	int mapW;
	int mapH;

	std::shared_ptr<Window> window;
	std::shared_ptr<Entity> player;
	std::vector<std::shared_ptr<Entity>> entityList;

	Map(int w, int h);

	void render();
};
