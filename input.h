#include "main.hpp"

struct Key //used to get the input of a single key on the keyboard
{
	sf::Keyboard::Key sfKey; //sfml representation of a key
	bool isDown; //if the key is currently down, stays true while the key is down
	bool isSwitched; //if the key has been switched, active for one frame when pressed

	Key(sf::Keyboard::Key sfKey); //constructor for key that takes an sfml key
	Key(); //default constructor

	void update(); //checks for the status for the key

private:
	bool keySwitch; //used for calculating key switch
};

struct MouseButton //used to get the input of a button on the mouse
{
	sf::Mouse::Button sfMButton; //sfml representation of a mouse button
	bool isDown; //if the button is currently held down
	bool isSwitched; //if the key has been switched

	MouseButton(sf::Mouse::Button MButton); //mouse constructor that takes an sfml mouse button
	MouseButton(); //default constructor

	void update(); //checks for the status of the key

private:
	bool buttonSwitch; //used for calculating the mouse button switch
};

struct Input //handles all of the mouse and keyboard input
{
	TCOD_event_t keyEvent; //handles tcod events
	TCOD_key_t keyboard; //handles tcod key events
	TCOD_mouse_t mouse; //handles tcod mouse events

	//player movement cluster
	std::shared_ptr<Key> w; //w key on keyboard
	std::shared_ptr<Key> a; //a key on keyboard
	std::shared_ptr<Key> s; //s key on keyboard
	std::shared_ptr<Key> d; //d key on keyboard
	std::shared_ptr<Key> lctrl; //left control key on keyboard
	std::shared_ptr<Key> lshift; //left shift key on keyboard
	std::shared_ptr<Key> z; //z key on keyboard
	std::shared_ptr<Key> x; //x key on keyboard
	std::shared_ptr<Key> c; //c key on keyboard

	//item and world interaction cluster
	std::shared_ptr<Key> e; //e key on keyboard
	std::shared_ptr<Key> q; //q key on keyboard
	std::shared_ptr<Key> r; //r key on keyboard
	std::shared_ptr<Key> lalt; //left alt key on keyboard
	std::shared_ptr<Key> space; //space key on keyboard

	//menu and window cluster
	std::shared_ptr<Key> i; //i key on keyboard
	std::shared_ptr<Key> f11; //f11 key on keyboard
	std::shared_ptr<Key> escape; //escape key on keyboard

	//debug cluster
	std::shared_ptr<Key> num0; //0 key on keyboard 
	std::shared_ptr<Key> num9; //9 key on keyboard

	//mouse buttons
	std::shared_ptr<MouseButton> leftMouseButton; //left mouse button on the mouse
	std::shared_ptr<MouseButton> rightMouseButton; //right mouse button on the mouse

	Input(); //input constructor that takes no arguments

	void update(std::shared_ptr<Player> player); //updates all of the buttons and events that takes player


private:
	std::vector<std::shared_ptr<Key>> keyList; //list of all keys availible for input
	std::vector<std::shared_ptr<MouseButton>> mouseList; //list of all availible mouse buttons for input

	void updateMouseInput(); //checks for mouse events
	void updateKeyInput(); //checks for key events
	void updateInput(std::shared_ptr<Player> player); //checks for all inputs
};
