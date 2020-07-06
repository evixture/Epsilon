#include "main.hpp"

Button::Button(const std::string name)
	: isDown(false), isSwitched(false), buttonSwitch(false), keyName(name)
{
}

//----------------------------------------------------------------------------------------------------

KeyboardButton::KeyboardButton(sf::Keyboard::Key key, const std::string name)
	: Button(name), button(key)
{
}

void KeyboardButton::update()
{
	isDown = sf::Keyboard::isKeyPressed(button);
	
	if (sf::Keyboard::isKeyPressed(button))
	{
		if (!buttonSwitch)
		{
			isSwitched = true;
			buttonSwitch = true;
		}
		else if (buttonSwitch)
		{
			isSwitched = false;
		}
	}
	else
	{
		isSwitched = false;
		buttonSwitch = false;
	}
}

//----------------------------------------------------------------------------------------------------

MouseButton::MouseButton(sf::Mouse::Button button, const std::string name)
	: Button(name), button(button)
{
}

void MouseButton::update()
{
	isDown = sf::Mouse::isButtonPressed(button);

	if (sf::Mouse::isButtonPressed(button))
	{
		if (!buttonSwitch)
		{
			isSwitched = true;
			buttonSwitch = true;
		}
		else if (buttonSwitch)
		{
			isSwitched = false;
		}
	}
	else
	{
		isSwitched = false;
		buttonSwitch = false;
	}
}

//----------------------------------------------------------------------------------------------------

Mouse::Mouse()
	:screenPosition(Position2(0, 0)), mapPosition(Position2(0, 0))
{
	leftMB = std::make_shared<MouseButton>(sf::Mouse::Left, "LeftMB");
	rightMB = std::make_shared<MouseButton>(sf::Mouse::Right, "RightMB");
	MouseWheel = std::make_shared<MouseButton>(sf::Mouse::Middle, "MouseWheel");
}

void Mouse::update(TCOD_mouse_t TCODmouse)
{
	if (TCODConsole::hasMouseFocus()) TCODMouse::showCursor(false); //change later to use setting option

	leftMB	  ->update();
	rightMB	  ->update();
	MouseWheel->update();

	screenPosition = Position2(TCODmouse.cx, TCODmouse.cy);
	mapPosition = Position2(screenPosition.x + WORLD->xOffset - 1, screenPosition.y + WORLD->yOffset - 3);
}

//----------------------------------------------------------------------------------------------------

ButtonList::ButtonList()
{
	buttonList.push_back(escape			= std::make_shared<KeyboardButton>(sf::Keyboard::Escape		, "Escape"));
	buttonList.push_back(f1				= std::make_shared<KeyboardButton>(sf::Keyboard::F1			, "F1"));
	buttonList.push_back(f2				= std::make_shared<KeyboardButton>(sf::Keyboard::F2			, "F2"));
	buttonList.push_back(f3				= std::make_shared<KeyboardButton>(sf::Keyboard::F3			, "F3"));
	buttonList.push_back(f4				= std::make_shared<KeyboardButton>(sf::Keyboard::F4			, "F4"));
	buttonList.push_back(f5				= std::make_shared<KeyboardButton>(sf::Keyboard::F5			, "F5"));
	buttonList.push_back(f6				= std::make_shared<KeyboardButton>(sf::Keyboard::F6			, "F6"));
	buttonList.push_back(f7				= std::make_shared<KeyboardButton>(sf::Keyboard::F7			, "F7"));
	buttonList.push_back(f8				= std::make_shared<KeyboardButton>(sf::Keyboard::F8			, "F8"));
	buttonList.push_back(f9				= std::make_shared<KeyboardButton>(sf::Keyboard::F9			, "F9"));
	buttonList.push_back(f10			= std::make_shared<KeyboardButton>(sf::Keyboard::F10		, "F10"));
	buttonList.push_back(f11			= std::make_shared<KeyboardButton>(sf::Keyboard::F11		, "F11"));
	buttonList.push_back(f12			= std::make_shared<KeyboardButton>(sf::Keyboard::F12		, "F12"));
	buttonList.push_back(pause			= std::make_shared<KeyboardButton>(sf::Keyboard::Pause		, "Pause"));
																		   							   			
	buttonList.push_back(tilde			= std::make_shared<KeyboardButton>(sf::Keyboard::Tilde		, "~"));
	buttonList.push_back(k1				= std::make_shared<KeyboardButton>(sf::Keyboard::Num1		, "1"));
	buttonList.push_back(k2				= std::make_shared<KeyboardButton>(sf::Keyboard::Num2		, "2"));
	buttonList.push_back(k3				= std::make_shared<KeyboardButton>(sf::Keyboard::Num3		, "3"));
	buttonList.push_back(k4				= std::make_shared<KeyboardButton>(sf::Keyboard::Num4		, "4"));
	buttonList.push_back(k5				= std::make_shared<KeyboardButton>(sf::Keyboard::Num5		, "5"));
	buttonList.push_back(k6				= std::make_shared<KeyboardButton>(sf::Keyboard::Num6		, "6"));
	buttonList.push_back(k7				= std::make_shared<KeyboardButton>(sf::Keyboard::Num7		, "7"));
	buttonList.push_back(k8				= std::make_shared<KeyboardButton>(sf::Keyboard::Num8		, "8"));
	buttonList.push_back(k9				= std::make_shared<KeyboardButton>(sf::Keyboard::Num9		, "9"));
	buttonList.push_back(k0				= std::make_shared<KeyboardButton>(sf::Keyboard::Num0		, "0"));
	buttonList.push_back(minus			= std::make_shared<KeyboardButton>(sf::Keyboard::Hyphen		, "-"));
	buttonList.push_back(equals			= std::make_shared<KeyboardButton>(sf::Keyboard::Equal		, "="));
	buttonList.push_back(backspace		= std::make_shared<KeyboardButton>(sf::Keyboard::BackSpace	, "BackSpace"));
	buttonList.push_back(insert			= std::make_shared<KeyboardButton>(sf::Keyboard::Insert		, "Insert"));
	buttonList.push_back(home			= std::make_shared<KeyboardButton>(sf::Keyboard::Home		, "Home"));
	buttonList.push_back(pageUp			= std::make_shared<KeyboardButton>(sf::Keyboard::PageUp		, "PageUp"));
	buttonList.push_back(numLock		= std::make_shared<KeyboardButton>(sf::Keyboard::PageDown	, "PageDown"));
	buttonList.push_back(kpDivide		= std::make_shared<KeyboardButton>(sf::Keyboard::Divide		, "k/"));
	buttonList.push_back(kpMultiply		= std::make_shared<KeyboardButton>(sf::Keyboard::Multiply	, "k*"));
	buttonList.push_back(kpMinus		= std::make_shared<KeyboardButton>(sf::Keyboard::Dash		, "k-"));
																		   							   			
	buttonList.push_back(tab			= std::make_shared<KeyboardButton>(sf::Keyboard::Tab		, "Tab"));
	buttonList.push_back(q				= std::make_shared<KeyboardButton>(sf::Keyboard::Q			, "Q"));
	buttonList.push_back(w				= std::make_shared<KeyboardButton>(sf::Keyboard::W			, "W"));
	buttonList.push_back(e				= std::make_shared<KeyboardButton>(sf::Keyboard::E			, "E"));
	buttonList.push_back(r				= std::make_shared<KeyboardButton>(sf::Keyboard::R			, "R"));
	buttonList.push_back(t				= std::make_shared<KeyboardButton>(sf::Keyboard::T			, "T"));
	buttonList.push_back(y				= std::make_shared<KeyboardButton>(sf::Keyboard::Y			, "Y"));
	buttonList.push_back(u				= std::make_shared<KeyboardButton>(sf::Keyboard::U			, "U"));
	buttonList.push_back(i				= std::make_shared<KeyboardButton>(sf::Keyboard::I			, "I"));
	buttonList.push_back(o				= std::make_shared<KeyboardButton>(sf::Keyboard::O			, "O"));
	buttonList.push_back(p				= std::make_shared<KeyboardButton>(sf::Keyboard::P			, "P"));
	buttonList.push_back(leftBrace		= std::make_shared<KeyboardButton>(sf::Keyboard::LBracket	, "["));
	buttonList.push_back(rightBrace		= std::make_shared<KeyboardButton>(sf::Keyboard::RBracket	, "]"));
	buttonList.push_back(backSlash		= std::make_shared<KeyboardButton>(sf::Keyboard::BackSlash	, "\\"));
	buttonList.push_back(del			= std::make_shared<KeyboardButton>(sf::Keyboard::Delete		, "Delete"));
	buttonList.push_back(end			= std::make_shared<KeyboardButton>(sf::Keyboard::End		, "End"));
	buttonList.push_back(pageDown		= std::make_shared<KeyboardButton>(sf::Keyboard::PageDown	, "PageDown"));
	buttonList.push_back(kp7			= std::make_shared<KeyboardButton>(sf::Keyboard::Numpad7	, "k7"));
	buttonList.push_back(kp8			= std::make_shared<KeyboardButton>(sf::Keyboard::Numpad8	, "k8"));
	buttonList.push_back(kp9			= std::make_shared<KeyboardButton>(sf::Keyboard::Numpad9	, "k9"));
	buttonList.push_back(kpAdd			= std::make_shared<KeyboardButton>(sf::Keyboard::Add		, "k+"));
																		   							   			
	buttonList.push_back(a				= std::make_shared<KeyboardButton>(sf::Keyboard::A			, "A"));
	buttonList.push_back(s				= std::make_shared<KeyboardButton>(sf::Keyboard::S			, "S"));
	buttonList.push_back(d				= std::make_shared<KeyboardButton>(sf::Keyboard::D			, "D"));
	buttonList.push_back(f				= std::make_shared<KeyboardButton>(sf::Keyboard::F			, "F"));
	buttonList.push_back(g				= std::make_shared<KeyboardButton>(sf::Keyboard::G			, "G"));
	buttonList.push_back(h				= std::make_shared<KeyboardButton>(sf::Keyboard::H			, "H"));
	buttonList.push_back(j				= std::make_shared<KeyboardButton>(sf::Keyboard::J			, "J"));
	buttonList.push_back(k				= std::make_shared<KeyboardButton>(sf::Keyboard::K			, "K"));
	buttonList.push_back(l				= std::make_shared<KeyboardButton>(sf::Keyboard::L			, "L"));
	buttonList.push_back(semiColon		= std::make_shared<KeyboardButton>(sf::Keyboard::SemiColon	, ";"));
	buttonList.push_back(quote			= std::make_shared<KeyboardButton>(sf::Keyboard::Quote		, "'"));
	buttonList.push_back(enter			= std::make_shared<KeyboardButton>(sf::Keyboard::Enter		, "Enter"));
	buttonList.push_back(kp4			= std::make_shared<KeyboardButton>(sf::Keyboard::Numpad4	, "k4"));
	buttonList.push_back(kp5			= std::make_shared<KeyboardButton>(sf::Keyboard::Numpad5	, "k5"));
	buttonList.push_back(kp6			= std::make_shared<KeyboardButton>(sf::Keyboard::Numpad6	, "k6"));
																		   							   			
	buttonList.push_back(leftShift		= std::make_shared<KeyboardButton>(sf::Keyboard::LShift		, "LShift"));
	buttonList.push_back(z				= std::make_shared<KeyboardButton>(sf::Keyboard::Z			, "Z"));
	buttonList.push_back(x				= std::make_shared<KeyboardButton>(sf::Keyboard::X			, "X"));
	buttonList.push_back(c				= std::make_shared<KeyboardButton>(sf::Keyboard::C			, "C"));
	buttonList.push_back(v				= std::make_shared<KeyboardButton>(sf::Keyboard::V			, "V"));
	buttonList.push_back(b				= std::make_shared<KeyboardButton>(sf::Keyboard::B			, "B"));
	buttonList.push_back(n				= std::make_shared<KeyboardButton>(sf::Keyboard::N			, "N"));
	buttonList.push_back(m				= std::make_shared<KeyboardButton>(sf::Keyboard::M			, "M"));
	buttonList.push_back(comma			= std::make_shared<KeyboardButton>(sf::Keyboard::Comma		, ","));
	buttonList.push_back(period			= std::make_shared<KeyboardButton>(sf::Keyboard::Period		, "."));
	buttonList.push_back(slash			= std::make_shared<KeyboardButton>(sf::Keyboard::Slash		, "/"));
	buttonList.push_back(rightShift		= std::make_shared<KeyboardButton>(sf::Keyboard::RShift		, "RShift"));
	buttonList.push_back(kpUp			= std::make_shared<KeyboardButton>(sf::Keyboard::Up			, "Up"));
	buttonList.push_back(kp1			= std::make_shared<KeyboardButton>(sf::Keyboard::Numpad1	, "k1"));
	buttonList.push_back(kp2			= std::make_shared<KeyboardButton>(sf::Keyboard::Numpad2	, "k2"));
	buttonList.push_back(kp3			= std::make_shared<KeyboardButton>(sf::Keyboard::Numpad3	, "k3"));
																		   							   			
	buttonList.push_back(leftControl	= std::make_shared<KeyboardButton>(sf::Keyboard::LControl	, "LControl"));
	buttonList.push_back(leftAlt		= std::make_shared<KeyboardButton>(sf::Keyboard::LAlt		, "LAlt	"));
	buttonList.push_back(space			= std::make_shared<KeyboardButton>(sf::Keyboard::Space		, "Space"));
	buttonList.push_back(rightAlt		= std::make_shared<KeyboardButton>(sf::Keyboard::RAlt		, "RAlt"));
	buttonList.push_back(rightControl	= std::make_shared<KeyboardButton>(sf::Keyboard::LControl	, "LControl"));
	buttonList.push_back(kpLeft			= std::make_shared<KeyboardButton>(sf::Keyboard::Left		, "Left"));
	buttonList.push_back(kpDown			= std::make_shared<KeyboardButton>(sf::Keyboard::Down		, "Down"));
	buttonList.push_back(kpRight		= std::make_shared<KeyboardButton>(sf::Keyboard::Right		, "Right"));
	buttonList.push_back(kp0			= std::make_shared<KeyboardButton>(sf::Keyboard::Numpad0	, "k0"));
}

std::vector<std::shared_ptr<Button>> ButtonList::getButtonsDown()
{
	std::vector<std::shared_ptr<Button>> ret;

	for (int i = 0; i < buttonList.size(); i++)
	{
		if (buttonList[i]->isDown)
		{
			ret.push_back(buttonList[i]);
		}
	}

	return ret;
}

std::vector<std::shared_ptr<Button>> ButtonList::getButtonsSwitched()
{
	std::vector<std::shared_ptr<Button>> ret;

	for (int i = 0; i < buttonList.size(); i++)
	{
		if (buttonList[i]->isSwitched)
		{
			ret.push_back(buttonList[i]);
		}
	}

	return ret;
}

std::string ButtonList::getButtonsSwitchedText()
{
	std::vector<std::shared_ptr<Button>> buttons = getButtonsSwitched();
	if (buttons.size() > 0)
	{
		if (buttons[0]->keyName.size() == 1)
		{
			return buttons[0]->keyName; //normal letter/symbol
		}
		else if (buttons[0]->keyName == "Space")
		{
			return " "; //space
		}
	}
	return "";
}

void ButtonList::update(TCOD_mouse_t mouse)
{
	for (auto& button : buttonList)
	{
		if		(TCODConsole::hasMouseFocus()) button->update();
		else	button->isDown = false;
	}
}

Bind::Bind(std::shared_ptr<Button> bind, const std::string name)
	:bind(bind), name(name)
{
}

Input::Input()
	:buttonList(std::make_shared<ButtonList>()), TCODmouse(), mouse(std::make_shared<Mouse>())
{
	bindList.push_back(moveUp			= std::make_shared<Bind>(buttonList->w			, "Move Up"		));
	bindList.push_back(moveDown			= std::make_shared<Bind>(buttonList->s			, "Move Down"	));
	bindList.push_back(moveLeft			= std::make_shared<Bind>(buttonList->a			, "Move Left"	));
	bindList.push_back(moveRight		= std::make_shared<Bind>(buttonList->d			, "Move Right"	));
									   							 								
	bindList.push_back(moveSlow			= std::make_shared<Bind>(buttonList->leftControl, "Move Slow"	));
	bindList.push_back(moveFast			= std::make_shared<Bind>(buttonList->leftShift	, "Move Fast"	));
									   							 									
	bindList.push_back(stanceDown		= std::make_shared<Bind>(buttonList->z			, "Stance Down"	));
	bindList.push_back(stanceUp			= std::make_shared<Bind>(buttonList->x			, "Stance Up"	));
									  							 									
	bindList.push_back(pickUp			= std::make_shared<Bind>(buttonList->e			, "Pick Up"		));
	bindList.push_back(drop				= std::make_shared<Bind>(buttonList->q			, "Drop"		));
									   							 									
	bindList.push_back(reload			= std::make_shared<Bind>(buttonList->r			, "Reload"		));
									   							 									
	bindList.push_back(deepInteract		= std::make_shared<Bind>(buttonList->leftAlt	, "Deep Interact"));
	bindList.push_back(worldInteract	= std::make_shared<Bind>(buttonList->space		, "World Interact"));
									   							 									
	bindList.push_back(inventory		= std::make_shared<Bind>(buttonList->i			, "Inventory"	));
	bindList.push_back(fullscreen		= std::make_shared<Bind>(buttonList->f11		, "Fullscreen"	));
	bindList.push_back(info				= std::make_shared<Bind>(buttonList->n			, "Info"		));
	bindList.push_back(menu				= std::make_shared<Bind>(buttonList->escape		, "Menu"		));
									   							 								
	bindList.push_back(highlight		= std::make_shared<Bind>(buttonList->h			, "Highlight"	));
									  							 									
	bindList.push_back(debug1			= std::make_shared<Bind>(buttonList->k1			, "debug1"		));
	bindList.push_back(debug2			= std::make_shared<Bind>(buttonList->k2			, "debug2"		));
	bindList.push_back(debug3			= std::make_shared<Bind>(buttonList->k3			, "debug3"		));
	bindList.push_back(console			= std::make_shared<Bind>(buttonList->tilde		, "Toggle Console"));
									   							 									
	bindList.push_back(primaryUse		= std::make_shared<Bind>(mouse->leftMB			, "Primary Use"	));
	bindList.push_back(alternateUse		= std::make_shared<Bind>(mouse->rightMB			, "Alternate Use"));
}

void Input::update()
{
	keyEvent = TCODSystem::checkForEvent(TCOD_EVENT_ANY, NULL, &TCODmouse);

	buttonList->update(TCODmouse);
	mouse->update(TCODmouse);
}
