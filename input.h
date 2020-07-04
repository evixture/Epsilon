#include "main.hpp"

/*
	Input
	|	Button
	|	L	KeyboardButton
	|	L	MouseButton
*/

/*
make input list for all keys and mbuttons

	all inputs in list
	L	binds list?

	all input list
	bind list

	binds take in a button for input

BUTTON
	string name

	char bind??

BIND
	button bind

	bind name

*/

struct Button
{
	Button(const std::string name);

	bool isDown;
	bool isSwitched;
	const std::string keyName;

	virtual void update() = 0;
	
protected:
	bool buttonSwitch;
};

struct KeyboardButton : public Button
{
	KeyboardButton(sf::Keyboard::Key key, const std::string name);
	KeyboardButton() = default;

	void update();

private:
	sf::Keyboard::Key button;
};

struct MouseButton : public Button
{
	MouseButton(sf::Mouse::Button button, const std::string name);
	MouseButton() = default;

	void update();

private:
	sf::Mouse::Button button;
};

struct Bind
{
	const std::string name;

	std::shared_ptr<Button> bind;

	Bind(std::shared_ptr<Button> bind, const std::string name);
};

struct Mouse
{
	Position2 screenPosition;
	Position2 mapPosition;

	std::shared_ptr<MouseButton> leftMB;
	std::shared_ptr<MouseButton> rightMB;
	std::shared_ptr<MouseButton> MouseWheel;

	Mouse();

	void update(TCOD_mouse_t TCODmouse);
};

struct ButtonList
{
	TCOD_mouse_t TCODmouse;

	std::shared_ptr<KeyboardButton> escape;
	std::shared_ptr<KeyboardButton> f1;
	std::shared_ptr<KeyboardButton> f2;
	std::shared_ptr<KeyboardButton> f3;
	std::shared_ptr<KeyboardButton> f4;
	std::shared_ptr<KeyboardButton> f5;
	std::shared_ptr<KeyboardButton> f6;
	std::shared_ptr<KeyboardButton> f7;
	std::shared_ptr<KeyboardButton> f8;
	std::shared_ptr<KeyboardButton> f9;
	std::shared_ptr<KeyboardButton> f10;
	std::shared_ptr<KeyboardButton> f11;
	std::shared_ptr<KeyboardButton> f12;
	std::shared_ptr<KeyboardButton> pause;

	std::shared_ptr<KeyboardButton> tilde;
	std::shared_ptr<KeyboardButton> k1;
	std::shared_ptr<KeyboardButton> k2;
	std::shared_ptr<KeyboardButton> k3;
	std::shared_ptr<KeyboardButton> k4;
	std::shared_ptr<KeyboardButton> k5;
	std::shared_ptr<KeyboardButton> k6;
	std::shared_ptr<KeyboardButton> k7;
	std::shared_ptr<KeyboardButton> k8;
	std::shared_ptr<KeyboardButton> k9;
	std::shared_ptr<KeyboardButton> k0;
	std::shared_ptr<KeyboardButton> minus;
	std::shared_ptr<KeyboardButton> equals;
	std::shared_ptr<KeyboardButton> backspace;
	std::shared_ptr<KeyboardButton> insert;
	std::shared_ptr<KeyboardButton> home;
	std::shared_ptr<KeyboardButton> pageUp;
	std::shared_ptr<KeyboardButton> numLock;
	std::shared_ptr<KeyboardButton> kpDivide;
	std::shared_ptr<KeyboardButton> kpMultiply;
	std::shared_ptr<KeyboardButton> kpMinus;

	std::shared_ptr<KeyboardButton> tab;
	std::shared_ptr<KeyboardButton> q;
	std::shared_ptr<KeyboardButton> w;
	std::shared_ptr<KeyboardButton> e;
	std::shared_ptr<KeyboardButton> r;
	std::shared_ptr<KeyboardButton> t;
	std::shared_ptr<KeyboardButton> y;
	std::shared_ptr<KeyboardButton> u;
	std::shared_ptr<KeyboardButton> i;
	std::shared_ptr<KeyboardButton> o;
	std::shared_ptr<KeyboardButton> p;
	std::shared_ptr<KeyboardButton> leftBrace;
	std::shared_ptr<KeyboardButton> rightBrace;
	std::shared_ptr<KeyboardButton> backSlash;
	std::shared_ptr<KeyboardButton> del;
	std::shared_ptr<KeyboardButton> end;
	std::shared_ptr<KeyboardButton> pageDown;
	std::shared_ptr<KeyboardButton> kp7;
	std::shared_ptr<KeyboardButton> kp8;
	std::shared_ptr<KeyboardButton> kp9;
	std::shared_ptr<KeyboardButton> kpAdd;

	std::shared_ptr<KeyboardButton> a;
	std::shared_ptr<KeyboardButton> s;
	std::shared_ptr<KeyboardButton> d;
	std::shared_ptr<KeyboardButton> f;
	std::shared_ptr<KeyboardButton> g;
	std::shared_ptr<KeyboardButton> h;
	std::shared_ptr<KeyboardButton> j;
	std::shared_ptr<KeyboardButton> k;
	std::shared_ptr<KeyboardButton> l;
	std::shared_ptr<KeyboardButton> semiColon;
	std::shared_ptr<KeyboardButton> quote;
	std::shared_ptr<KeyboardButton> enter;
	std::shared_ptr<KeyboardButton> kp4;
	std::shared_ptr<KeyboardButton> kp5;
	std::shared_ptr<KeyboardButton> kp6;

	std::shared_ptr<KeyboardButton> leftShift;
	std::shared_ptr<KeyboardButton> z;
	std::shared_ptr<KeyboardButton> x;
	std::shared_ptr<KeyboardButton> c;
	std::shared_ptr<KeyboardButton> v;
	std::shared_ptr<KeyboardButton> b;
	std::shared_ptr<KeyboardButton> n;
	std::shared_ptr<KeyboardButton> m;
	std::shared_ptr<KeyboardButton> comma;
	std::shared_ptr<KeyboardButton> period;
	std::shared_ptr<KeyboardButton> slash;
	std::shared_ptr<KeyboardButton> rightShift;
	std::shared_ptr<KeyboardButton> kpUp;
	std::shared_ptr<KeyboardButton> kp1;
	std::shared_ptr<KeyboardButton> kp2;
	std::shared_ptr<KeyboardButton> kp3;

	std::shared_ptr<KeyboardButton> leftControl;
	std::shared_ptr<KeyboardButton> leftAlt;
	std::shared_ptr<KeyboardButton> space;
	std::shared_ptr<KeyboardButton> rightAlt;
	std::shared_ptr<KeyboardButton> rightControl;
	std::shared_ptr<KeyboardButton> kpLeft;
	std::shared_ptr<KeyboardButton> kpDown;
	std::shared_ptr<KeyboardButton> kpRight;
	std::shared_ptr<KeyboardButton> kp0;
	
	std::vector<std::shared_ptr<Button>> buttonList;

	ButtonList();

	std::vector<std::shared_ptr<Button>> getButtonsDown();
	std::vector<std::shared_ptr<Button>> getButtonsSwitched();

	void update();

private:
	TCOD_event_t keyEvent; //handles tcod events
	TCOD_key_t keyboard; //handles tcod key events
};

struct Input
{
	std::shared_ptr<Mouse> mouse;
	TCOD_mouse_t TCODmouse;

	std::shared_ptr<Bind> moveUp; 
	std::shared_ptr<Bind> moveDown; 
	std::shared_ptr<Bind> moveLeft; 
	std::shared_ptr<Bind> moveRight;
					
	std::shared_ptr<Bind> moveSlow; 
	std::shared_ptr<Bind> moveFast; 
					
	std::shared_ptr<Bind> stanceDown;
	std::shared_ptr<Bind> stanceUp; 
					
	std::shared_ptr<Bind> pickUp; 
	std::shared_ptr<Bind> drop; 
					
	std::shared_ptr<Bind> reload; 
					
	std::shared_ptr<Bind> deepInteract; 
	std::shared_ptr<Bind> worldInteract; 
					
	std::shared_ptr<Bind> inventory; 
	std::shared_ptr<Bind> fullscreen; 
	std::shared_ptr<Bind> info;
	std::shared_ptr<Bind> menu; 
					
	std::shared_ptr<Bind> highlight;
					
	std::shared_ptr<Bind> debug1; 
	std::shared_ptr<Bind> debug2;
	std::shared_ptr<Bind> debug3; 

	std::shared_ptr<Bind> primaryUse;
	std::shared_ptr<Bind> alternateUse;

	std::shared_ptr<ButtonList> buttonList;

	Input();

	void update();

private:
	std::vector<std::shared_ptr<Bind>> bindList;
};

//struct Input
//{
//	std::shared_ptr<Mouse> mouse;
//	TCOD_mouse_t TCODmouse;
//
//	std::shared_ptr<KeyboardButton> moveUpKey; //key to move the player / menu selector up
//	std::shared_ptr<KeyboardButton> moveDownKey; //key to move the player / menu selector down
//	std::shared_ptr<KeyboardButton> moveLeftKey; //key to move the player left
//	std::shared_ptr<KeyboardButton> moveRightKey; //key to move the player right
//
//	std::shared_ptr<KeyboardButton> moveSlowKey; //key to make the player move slower
//	std::shared_ptr<KeyboardButton> moveFastKey; //key to make the player move faster
//
//	std::shared_ptr<KeyboardButton> stanceDownKey; //key to make the player go prone
//	std::shared_ptr<KeyboardButton> stanceUpKey; //key to make the player crouch
//	std::shared_ptr<KeyboardButton> debug3Key; //key to make the player stand up
//
//	std::shared_ptr<KeyboardButton> pickUpKey; //key for the player to pick up an item on the ground
//	std::shared_ptr<KeyboardButton> dropKey; //key for the player to drop the selected item
//
//	std::shared_ptr<KeyboardButton> reloadKey; //key for the player to reload the selected item
//
//	std::shared_ptr<KeyboardButton> deepInteractKey; //key for the player to initiate the selection of alternate actions
//	std::shared_ptr<KeyboardButton> worldInteractKey; //key to interact with the world
//
//	std::shared_ptr<KeyboardButton> inventoryKey; //key to open the full menu
//	std::shared_ptr<KeyboardButton> fullscreenKey; //key to put the window in full screen
//	std::shared_ptr<KeyboardButton> infoKey;
//	std::shared_ptr<KeyboardButton> menuKey; //key to open the pause menu
//
//	std::shared_ptr<KeyboardButton> highlightKey;
//
//	std::shared_ptr<KeyboardButton> debug1Key; //key to do a debug action
//	std::shared_ptr<KeyboardButton> debug2Key; //key to do a debug action
//
//	std::shared_ptr<MouseButton> primaryUseButton; //key to use the primary action of the selected item
//	std::shared_ptr<MouseButton> alternateUseButton; //key to use the alternate use of the selected item
//
//	Input();
//
//	void update();
//
//private:
//	TCOD_event_t keyEvent; //handles tcod events
//	TCOD_key_t keyboard; //handles tcod key events
//
//	std::vector<std::shared_ptr<Button>> buttonList;
//};