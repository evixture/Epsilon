#include "main.hpp"

struct MapPane : public Window
{
	std::shared_ptr<Pane> mapSidePanel;

	std::shared_ptr<World> map;
	std::shared_ptr<TCODMap> tcodMap;

	MapPane(int w, int h, int rx, int ry);

	void update();

	void render();
};
