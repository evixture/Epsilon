#include "main.hpp"

/*
	Window
	|	Pane
	|	Ribbon
*/

struct Pane
{
	const unsigned short int consoleWidth;
	const unsigned short int consoleHeight;
	TCODConsole* console; 

	TCODColor backgroundColor;
	TCODColor foregroundColor;

	Pane(int consoleWidth, int consoleHeight, TCODColor backgroundColor, TCODColor foregroundColor);
	~Pane();

	void render() const;
};

struct Ribbon
{
	const unsigned short int consoleWidth;
	Pane ribonWindow;

	Ribbon(std::string windowName, int consoleWidth);

	void render() const;

private:
	std::string windowName;
};

struct Window
{
	const unsigned short int consoleWidth;
	const unsigned short int consoleHeight;
	Position3 renderpos;

	Pane drawPane;

	Window(int consoleWidth, int consoleHeight, std::string panelName, int rx, int ry);
	virtual ~Window() {};

	virtual void update() = 0;
	virtual void render() const = 0;

	void clearWindow() const;
	void pushWindow() const;

private:
	Ribbon ribbon;
	Pane mainPane;
};
