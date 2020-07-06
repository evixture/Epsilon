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
	std::shared_ptr<Font> terminal16x16;


	bool showAIMind;
	bool showWalkable;

	Settings();

	void update();
	void render() const;

private:
	void setfont(std::shared_ptr<Font> font);
};
