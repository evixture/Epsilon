#include "main.hpp"

struct Font //font class that holds the needed data to set the tcod font
{
	std::string filePath; //string file path to the font
	std::string fontName; //name of the font

	int format; //bit mask format of the font

	int charH; //height of font in characters
	int charW; //width of font in characters
	std::string xDim; //x dimension of font
	std::string yDim; //y dimension of font

	Font(std::string fontName, std::string xdim, std::string ydim, std::string filePath, int format, int charW, int charH); //font constructor that takes string name, x and y dimension, file path, bitmask format, and char width and height
};

struct Settings //settings class that hold most of the settings for the game and tcod
{
	int screenCharWidth; //total width of root window in characters
	int screenCharHeight; //total height of root window in characters

	int fpsCount; //the amount of frames pushed in the last second increment

	TCOD_fov_algorithm_t fovtype; //the type of fov algorithm used for the map
	int fovRad; //the radius of tiles that the fov is calculated for
	bool lightWalls; //if the fov algorithm lights the tiles

	std::shared_ptr<Input> input; //input component for keyboard and mouse

	std::shared_ptr<Font> terminal16x16; //default font
	std::vector<std::shared_ptr<Font>> fontList; //list of fonts availible for selection

	Settings(int screenCharWidth, int screenCharHeight); //settings constructor that takes width and height

	void setFullscreen(); //sets window fullscreen on or off

	void update(std::shared_ptr<Player> player); //updates settings
	void render() const; //renders settings to root

private:
	int maxFps; //fps cap fo tcod

	TCOD_renderer_t renderer; //type of library used to render the window
	std::string windowTitle; //string title of the main window
	bool fullscreen; // if the main window is fullscreen

	void setfont(std::shared_ptr<Font> font); //sets the font used
	void printLogo() const; //prints epsilon to the top left corner
	void printDebugStats() const; //prints debug data to the top of the screen
};
