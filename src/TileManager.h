
#ifndef _JAS_TILE_MANAGER_H_
#define _JAS_TILE_MANAGER_H_

/*
 * Made by Jasper van Herpt
 *
 * Jan-2010
 *
*/

#ifdef __WIN32
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif

#include "ErrHandling.hpp"

#include <iostream>
#include <fstream>
#include <string>

#define		MAX_TILES		0x00000004
#define		TILE_WIDTH		0x00000096
#define		TILE_HEIGHT		0x00000096

#define		GRASS_TILE		0x0001
#define		DIRT_TILE		0x0002
#define		FLOWER_TILE		0x0003
#define		FINISH_TILE		0x0004

typedef signed short int Ssint;

class Tile
{
private:
	SDL_Rect* inner;
	Uint16 typeTile;

public:
	Tile();
	~Tile();

	Uint16 get_type()const;
	void set_type( Uint16 );
	SDL_Rect& get_inner();
	SDL_Rect* get_inner( int );
	void set_inner( SDL_Rect* );
	void show( SDL_Surface*, SDL_Surface*, Tile* );

};

class Camera
{
private:
	SDL_Rect itsView;

protected:

public:
	Camera();
	~Camera(){}

	void set_camera( Ssint xPos, Ssint yPos );
	void set_dimensions( Uint16 camW, Uint16 camH );

	SDL_Rect get_view() const { return itsView; }
};

class TileManager
{
private:
	Tile* itsTiles;
	Tile* loadedMap;

	bool tilesLoaded;
	bool mapLoaded;

	SDL_Surface* clipSheet;

	void init_graphics();
	void read_map();
	bool is_in_view( Tile* tile );

public:
	TileManager(){}
	~TileManager(){}
	
	void init( SDL_Surface* itsgraphs, int xTiles, int yTiles, int screenWidth, int screenHeigth );
	void clean_up();

	void make_random_map();
	void load_map( std::string );

	SDL_Surface* draw_map();
	Tile* get_finish_tile();

	bool check_tiles_loaded()const;
	bool check_map_loaded()const;

	unsigned int get_map_width();
	unsigned int get_map_height();

	Camera camera;
};

#endif
