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
#define TILE_BasicGrass    std::make_shared<Tile>		 ('.', TCODColor(119, 161, 63),  TCODColor(32, 70, 19),  0, true, "static")
#define TILE_BasicFlower   std::make_shared<Tile>		 ('*', TCODColor::darkChartreuse,TCODColor::darkestSepia,  2, true, "static")
#define TILE_BasicFloor    std::make_shared<Tile>		 (' ', TCODColor::darkSepia  ,   TCODColor::darkerSepia ,  0, true, "static")
#define TILE_BasicConcrete std::make_shared<Tile>		 ('`', TCODColor::darkGrey   ,   TCODColor::grey        ,  0, true, "static")

//WALL
#define TILE_BasicDoor     std::make_shared<Tile>		 ('#', TCODColor::darkSepia,     TCODColor::darkestSepia,  4, true, "static")
#define TILE_BasicWall     std::make_shared<Destructible>('=', TCODColor::lightSepia,    TCODColor::lighterSepia,  4, false, 4 )
#define TILE_BasicWindow   std::make_shared<Destructible>('_', TCODColor::lightCyan,     TCODColor::darkerSky,     0, false, 1 )

//PART HEIGHT
#define TILE_BasicTable    std::make_shared<Destructible>('n', TCODColor::lighterSepia,  TCODColor::darkerSepia,   2, false, 2 )

//STAIR
#define TILE_UpStair	   std::make_shared<Stair>		 ('/', TCODColor::lightSepia, TCODColor::darkerSepia, 2, true, 1)
#define TILE_DownStair	   std::make_shared<Stair>		 ('\\', TCODColor::lightSepia, TCODColor::darkerSepia, 2, true, -1)

//SKY
#define TILE_BasicSky      std::make_shared<Tile>		 (' ', TCODColor::lightestCyan,  TCODColor::sky,           0, false, "static")

//ERROR
#define TILE_error         std::make_shared<Tile>		 ('%', TCODColor::black,         TCODColor::pink,          4, true , "static")
//============COLORS===================================================================================================== |
#define RIBONBGCOLOR TCODColor::darkBlue
#define RIBONFGCOLOR TCODColor::lightestGrey

constexpr auto PI = 3.14159265;
//TODO : move defines to settings
//TILE STRENGTH IS FROM 0

//STD
#include <fstream>
#include <memory>
#include <vector>
//#include <iostream>
#include <string>
#include <math.h>
//#include <thread>
//#include <stdexcept>

//Libraries
#include <libtcod.hpp>
#include <SFML/Window.hpp>

struct Pane;
struct Entity;

//Class File Imports
#include "utility.h"
#include "tool.h"
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

//TODO : BULETS STILL GET STUCK ON MAP BOUNDS

//TODO : add more defs for colors in window
//TODO : optimize and test access modifiers
//TODO : fov is computed every update, try to optimize later

//!NOTE : create generic items for tiles and then push them