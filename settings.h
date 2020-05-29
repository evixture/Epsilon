#include "main.hpp"

/*
	Settings
	|	Font
*/

struct Font
{
	std::string name;

	std::string filePath;
	unsigned char format;

	unsigned char charH;
	unsigned char charW;

	std::string xDim;
	std::string yDim;

	Font(std::string name, std::string xdim, std::string ydim, std::string filePath, int format, int charW, int charH);
};

struct Settings
{
	int screenCharWidth;
	int screenCharHeight;

	sf::Time lastFrameTime;

	unsigned int fpsCount;

	TCOD_fov_algorithm_t fovtype;
	unsigned int fovRad;
	bool lightWalls;

	std::shared_ptr<Input> input;

	std::shared_ptr<Font> terminal16x16;
	std::vector<std::shared_ptr<Font>> fontList;

	Settings(int screenCharWidth, int screenCharHeight);

	void setFullscreen();

	void update();
	void render() const;

private:
	sf::Clock systemClock;

	TCOD_renderer_t renderer;
	std::string windowTitle;
	bool fullscreen;

	void setfont(std::shared_ptr<Font> font);
	void printLogo() const;
	void printDebugStats() const;
};
