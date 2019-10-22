#include "main.hpp"

//Engine Class
class Engine
{
public:
	enum Gamestate { INIT, STARTUP, MAIN, MENU, GAMEOVER, EXIT } gamestate;

	std::shared_ptr<Settings> settings;
	std::shared_ptr<Gui> gui;

	Engine(int screenCharWidth, int screenCharHeight);

	void update();
	void render() const;
};

extern std::shared_ptr<Engine> engine;
