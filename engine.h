#include "main.hpp"

struct Engine //engine class that contains everything
{
	enum class Gamestate { MAIN, EXIT } gamestate; //the main state tht the game is in

	std::shared_ptr<Input> input;
	std::shared_ptr<Settings> settings;
	std::shared_ptr<Audio> audio;
	std::shared_ptr<Gui> gui;

	int screenCharWidth;
	int screenCharHeight;

	sf::Time lastFrameTime;
	unsigned int fpsCount;

	TCOD_fov_algorithm_t fovtype;
	const unsigned int fovRad;
	const bool lightWalls;

	//std::vector<std::shared_ptr<Bind>> test();

	Engine(int screenCharWidth, int screenCharHeight); //engine constructor that takes screen width in characters and screen height in characters
	~Engine();

	void setFullscreen();

	void update(); //runs all of the other update functions of the things in this class
	void render() const; //runs all of the other render functions for the things in this class
private:
	sf::Clock systemClock;

	TCOD_renderer_t renderer;
	std::string windowTitle;
	bool fullscreen;

	void printLogo() const;
	void printDebugStats() const;
};

extern std::shared_ptr<Engine> engine; //states that this is used initially in main
