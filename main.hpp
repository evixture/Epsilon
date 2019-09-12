#pragma once

//	DEFINES
//============TILES=======================CH===FOREGROUND=COLOR======BACKGROUND=COLOR====HEIGHT==WALK==TRANS==DESTR==
#define TILE_grass std::make_shared<Tile>('.', TCODColor::darkerGreen, TCODColor::darkestGreen, 4, true, true)
#define TILE_wall  std::make_shared<Tile>('#', TCODColor::lightSepia, TCODColor::lighterSepia, 4, false, false)
#define TILE_floor std::make_shared<Tile>(' ', TCODColor::darkSepia, TCODColor::darkerSepia, 4, true, true)
//============COLORS=================================================================================================
#define RIBONBGCOLOR TCODColor::darkBlue

//STD Library Imports
#include <fstream>
#include <memory>
#include <vector>
#include <libtcod.hpp>
#include <iostream>
//sfml window headers
#include <SFML/Window.hpp>

struct Pane;

//Class File Imports
#include "entity.h"
#include "tile.h"
#include "map.h"
#include "input.h"
#include "settings.h"
#include "window.h"
#include "panel.h"
#include "gui.h"
#include "engine.h"

//TODO : add more defs for colors in window
//TODO : optimize and test access modifiers
//TODO : to create and push to vector use vector.push(create ent here)
//TODO : tcod consoles start at 0, 0
//TODO : Figure out what fov algo to use
//TODO : fov is computed every update, try to optimize later

//!NOTE : blitting consoles requires a background flag
//!NOTE : Documentation is done in declaration
//!NOTE : create generic items for tiles and then push them 

//need to change "length" vars to be more easily typed
//need to clean up mapfile code
//need to figure out good fov type;
