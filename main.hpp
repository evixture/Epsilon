#pragma once

/*
	FLOOR HEIGHTS

	4 :: Top    ^
	3 :: Stand  ^ 
	2 :: Crouch ^
	1 :: Prone  ^
	0 :: Floor & Window  ^

	if player height > floor height , visibility is true, else set to default vis
	stand can see over crouch
*/

//	DEFINES
//============TILES===============================CH===FOREGROUND=COLOR==========BACKGROUND=COLOR==========H==WALK===DEST |
//GROUND
#define TILE_BasicGrass    std::make_shared<Tile>('.', TCODColor::darkerGreen,   TCODColor::darkestGreen,  0, true,  false)
#define TILE_BasicFloor    std::make_shared<Tile>(' ', TCODColor::darkSepia,     TCODColor::darkerSepia,   0, true,  false)
#define TILE_BasicConcrete std::make_shared<Tile>('`', TCODColor::darkGrey,      TCODColor::grey,          0, true,  false)
//WALL
#define TILE_BasicWall     std::make_shared<Tile>('=', TCODColor::lightSepia,    TCODColor::lighterSepia,  4, false, true )
#define TILE_BasicDoor     std::make_shared<Tile>('#', TCODColor::darkSepia,     TCODColor::darkestSepia,  4, true,  false)
#define TILE_BasicWindow   std::make_shared<Tile>('_', TCODColor::lightCyan,     TCODColor::darkerSky,     0, false, true )
//PART HEIGHT
#define TILE_BasicTable    std::make_shared<Tile>('n', TCODColor::lighterSepia,  TCODColor::darkerSepia,   2, false, true )
//SKY
#define TILE_BasicSky      std::make_shared<Tile>(' ', TCODColor::lightestCyan,  TCODColor::sky,           0, false, false)
//ERROR
#define TILE_error         std::make_shared<Tile>('%', TCODColor::black,         TCODColor::pink,          4, true,  false)
//============COLORS===================================================================================================== |
#define RIBONBGCOLOR TCODColor::darkBlue
#define RIBONFGCOLOR TCODColor::lightestGrey

constexpr auto PI = 3.14159265;
//TODO : move defines to settings

//STD
#include <fstream>
#include <memory>
#include <vector>
#include <iostream>
#include <string>
#include <math.h>
//#include <stdexcept>

//Libraries
#include <libtcod.hpp>
#include <SFML/Window.hpp>

struct Pane;

//Class File Imports
#include "utility.h"
#include "entity.h"
#include "tile.h"
#include "map.h"
#include "input.h"
#include "settings.h"
#include "window.h"
#include "panel.h"
#include "gui.h"
#include "engine.h"

//TODO : NEED TO HAVE A VIRTUAL DESTRUCTOR FOR VIRTUAL CLASSES

//TODO : add more defs for colors in window
//TODO : optimize and test access modifiers
//TODO : fov is computed every update, try to optimize later

//!NOTE : create generic items for tiles and then push them