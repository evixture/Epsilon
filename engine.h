#include "main.hpp"

//Engine Class
class Engine
{
public:
	std::shared_ptr<Settings> settings;
	std::shared_ptr<Gui> gui;

	enum Gamestate { INIT, MAIN, MENU, GAMEOVER, EXIT } gamestate;

	Engine(int screenCharWidth, int screenCharHeight);

	void update();
	void render() const;
};

extern std::shared_ptr<Engine> engine;
