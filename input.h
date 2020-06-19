#include "main.hpp"

/*
	Input
	|	Button
	|	L	KeyboardButton
	|	L	MouseButton
*/

struct Button
{
	Button();

	bool isDown;
	bool isSwitched;

	virtual void update() = 0;
	
protected:
	bool buttonSwitch;
};

struct KeyboardButton : public Button
{
	KeyboardButton(sf::Keyboard::Key key);
	KeyboardButton();

	void update();

private:
	sf::Keyboard::Key button;
};

struct MouseButton : public Button
{
	MouseButton(sf::Mouse::Button button);
	MouseButton();

	void update();

private:
	sf::Mouse::Button button;
};

struct Mouse
{
	Position2 screenPosition;
	Position2 mapPosition;

	Mouse();

	void update(TCOD_mouse_t TCODmouse);
};

struct Input
{
	std::shared_ptr<Mouse> mouse;
	TCOD_mouse_t TCODmouse;

	std::shared_ptr<KeyboardButton> moveUpKey; //key to move the player / menu selector up
	std::shared_ptr<KeyboardButton> moveDownKey; //key to move the player / menu selector down
	std::shared_ptr<KeyboardButton> moveLeftKey; //key to move the player left
	std::shared_ptr<KeyboardButton> moveRightKey; //key to move the player right

	std::shared_ptr<KeyboardButton> moveSlowKey; //key to make the player move slower
	std::shared_ptr<KeyboardButton> moveFastKey; //key to make the player move faster

	std::shared_ptr<KeyboardButton> stanceDownKey; //key to make the player go prone
	std::shared_ptr<KeyboardButton> stanceUpKey; //key to make the player crouch
	std::shared_ptr<KeyboardButton> debug3Key; //key to make the player stand up

	std::shared_ptr<KeyboardButton> pickUpKey; //key for the player to pick up an item on the ground
	std::shared_ptr<KeyboardButton> dropKey; //key for the player to drop the selected item

	std::shared_ptr<KeyboardButton> reloadKey; //key for the player to reload the selected item

	std::shared_ptr<KeyboardButton> deepInteractKey; //key for the player to initiate the selection of alternate actions
	std::shared_ptr<KeyboardButton> worldInteractKey; //key to interact with the world

	std::shared_ptr<KeyboardButton> inventoryKey; //key to open the full menu
	std::shared_ptr<KeyboardButton> fullscreenKey; //key to put the window in full screen
	std::shared_ptr<KeyboardButton> infoKey;
	std::shared_ptr<KeyboardButton> menuKey; //key to open the pause menu

	std::shared_ptr<KeyboardButton> highlightKey;

	std::shared_ptr<KeyboardButton> debug1Key; //key to do a debug action
	std::shared_ptr<KeyboardButton> debug2Key; //key to do a debug action

	std::shared_ptr<MouseButton> primaryUseButton; //key to use the primary action of the selected item
	std::shared_ptr<MouseButton> alternateUseButton; //key to use the alternate use of the selected item

	Input();

	void update();

private:
	TCOD_event_t keyEvent; //handles tcod events
	TCOD_key_t keyboard; //handles tcod key events

	std::vector<std::shared_ptr<Button>> buttonList;
};