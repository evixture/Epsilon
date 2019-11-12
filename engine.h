#include "main.hpp"

//Engine Class
class Engine
{
public:
	enum Gamestate { MAIN, EXIT } gamestate;

	std::shared_ptr<Settings> settings;
	std::shared_ptr<Gui> gui;

	Engine(int screenCharWidth, int screenCharHeight);

	void update();
	void render() const;
};

extern std::shared_ptr<Engine> engine;
