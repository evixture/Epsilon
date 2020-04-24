#include "main.hpp"

struct Engine //engine class that contains everything
{
	enum class Gamestate { MAIN, EXIT } gamestate; //the main state tht the game is in

	std::shared_ptr<Settings> settings; //contains all of the settings and mainly non-graphical things
	std::shared_ptr<Gui> gui; //contains nearly all of the things that need to be rendered to the screen
	//std::shared_ptr<SoLoud::Soloud> audio;

	Engine(int screenCharWidth, int screenCharHeight); //engine constructor that takes screen width in characters and screen height in characters
	~Engine();

	void update(); //runs all of the other update functions of the things in this class
	void render() const; //runs all of the other render functions for the things in this class
};

extern std::shared_ptr<Engine> engine; //states that this is used initially in main
