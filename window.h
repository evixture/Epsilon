#include "main.hpp"

//Window Struct
struct Pane
{
	int consoleW;
	int consoleH;
	TCODConsole* console;

	TCODColor bgColor;
	TCODColor fgColor;

	Pane(int consoleW, int consoleH, TCODColor bgColor, TCODColor fgColor);
	~Pane();

	void render() const;
};

//Ribon Struct
struct Ribon
{
	std::string windowName;
	int windowW;

	std::shared_ptr<Pane> ribonWindow;

	Ribon(std::string windowName, int windowW);

	void updateRibonName(std::string ribonName);

	void render() const;
};

//GuiWindow Struct
struct Window
{
public:

	int windowW;
	int windowH;
	std::string panelName;
	Position renderpos;

	std::shared_ptr<Pane> drawWindow;

	Window(int windowW, int windowH, std::string panelName, int rx, int ry);

	virtual void update();
	virtual void render() const;

	void setRibonName(std::string ribonName);
	void clearWindow() const;
	void pushWindow() const;

private:
	std::shared_ptr<Ribon> ribon;
	std::shared_ptr<Pane> mainWindow;
};

//GuiMap Struct
