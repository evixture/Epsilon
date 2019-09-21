#include "main.hpp"

struct MapPane : public Window
{
	std::shared_ptr<Pane> mapSidePanel;

	std::shared_ptr<World> world;
	//std::shared_ptr<TCODMap> tcodMap;

	MapPane(int w, int h, int rx, int ry);

	void update();

	void render();
};

struct StatusPane : public Window
{
	int displayHealth;
	int displayArmor;

	StatusPane(int w, int h, int rx, int ry);

	void drawBar(std::shared_ptr<Pane> window, int x, int y, const char* barTitle, int barValue, int barMax);

	void update();

	void render();
};
