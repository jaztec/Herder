/*
 * Made by Jasper van Herpt
 *
 * Jan-2010
 *
*/

unsigned short MAP_SIZE = 0;
unsigned long MAP_WIDTH = 0;
unsigned long MAP_HEIGHT = 0;

#include "TileManager.h"

// Klasse Tile
//
//
Tile::Tile()
{
	inner = new SDL_Rect;
	if( inner == 0 )
		return;
}

Tile::~Tile()
{
	delete inner;
}

Uint16 Tile::get_type()const
{
	return typeTile;
}

void Tile::set_type( Uint16 type )
{
	typeTile = type;
}

SDL_Rect& Tile::get_inner()
{
	return *inner;
}

SDL_Rect* Tile::get_inner( int flag )
{
	return inner;
}

void Tile::set_inner( SDL_Rect* newbox )
{
	if( inner )
		delete inner;
	inner = new SDL_Rect;
	if( inner == 0 )
		return;
	*inner = *newbox;
}

void Tile::show( SDL_Surface* screen, SDL_Surface* clipSheet, Tile* tile )
{
	SDL_BlitSurface( clipSheet, tile->get_inner( 1 ), screen, this->inner );
}


// Klasse Camera
//
//

Camera::Camera()
{
	itsView.x = 0;
	itsView.y = 0;
	itsView.w = 0;
	itsView.h = 0;
}

void Camera::set_camera( Ssint xPos, Ssint yPos )
{
	if( itsView.w < MAP_WIDTH )
	{
		if( ( xPos > 0 ) && ( ( xPos + itsView.w ) <= MAP_WIDTH ) )
			itsView.x = xPos;
		else if( xPos < 0 )
			itsView.x = 0;
		else if( ( xPos + itsView.w ) > MAP_WIDTH )
			itsView.x = MAP_WIDTH - itsView.w;
	}
	else
		itsView.x = 0;

	if( itsView.h < MAP_HEIGHT )
	{		
		if( ( yPos > 0 ) && ( ( yPos + itsView.h ) <= MAP_HEIGHT ) )
			itsView.y = yPos;
		else if( yPos < 0 )
			itsView.y = 0;
		else if( ( yPos + itsView.h ) > MAP_HEIGHT )
			itsView.y = MAP_HEIGHT - itsView.h;
	}
	else
		itsView.y = 0;
}

void Camera::set_dimensions( Uint16 camW, Uint16 camH )
{
	itsView.w = camW;
	itsView.h = camH;
}


/*
	Start van klasse TileManager
*/
//
//


void TileManager::init( SDL_Surface* itsgraphs, int xTiles, int yTiles, int screenWidth, int screenHeigth )
{
	std::cout << "TileManager initiated\n";
	tilesLoaded = false;
	mapLoaded = false;

	clipSheet = itsgraphs;

	itsTiles = new Tile[ MAX_TILES ];
	if( itsTiles == 0 )
		return;

	MAP_SIZE = xTiles * yTiles;
	MAP_WIDTH = xTiles * TILE_WIDTH;
	MAP_HEIGHT = yTiles * TILE_HEIGHT;

	camera.set_dimensions( screenWidth, screenHeigth );

	init_graphics();

}

void TileManager::clean_up()
{
	std::cout << "Destroying TileManager\n";
	if( mapLoaded )
		delete [] loadedMap;
	if( tilesLoaded )
		delete [] itsTiles;
}

void TileManager::init_graphics()
{
	std::cout << "Initiating graphics\n";
	int depth = 0;
	int height = 0;

	for( int i = 0; i < MAX_TILES; i++ )
	{
		if( depth == 3*TILE_WIDTH )
		{				
			depth = 0;
			height += TILE_HEIGHT;
		}

		itsTiles[ i ].get_inner().x = depth;
		itsTiles[ i ].get_inner().y = height;
		itsTiles[ i ].get_inner().w = TILE_WIDTH;
		itsTiles[ i ].get_inner().h = TILE_HEIGHT;

		depth += TILE_WIDTH;
	}

	itsTiles[ GRASS_TILE ].set_type( GRASS_TILE );
	itsTiles[ DIRT_TILE ].set_type( DIRT_TILE );
	itsTiles[ FLOWER_TILE ].set_type( FLOWER_TILE );
	itsTiles[ FINISH_TILE ].set_type( FINISH_TILE );

	tilesLoaded = true;
	std::cout << "Graphics initiated\n";
}

void TileManager::read_map()
{
	std::cout << "Reading map...\n" << std::endl << std::endl;
	std::cout << "Map size is " << MAP_SIZE << std::endl;

	for( int i = 0; i < MAP_SIZE; i++ )
	{
		std::cout << "**Reading tile " << i << "**\n";
		std::cout << "\ttype:\t" << loadedMap[ i ].get_type() << std::endl;
		std::cout << "\twidth:\t" << loadedMap[ i ].get_inner().w << std::endl;
		std::cout << "\theight:\t" << loadedMap[ i ].get_inner().h << std::endl;
		std::cout << "\tx:\t" << loadedMap[ i ].get_inner().x << std::endl;
		std::cout << "\ty:\t" << loadedMap[ i ].get_inner().y << std::endl;
	}

	std::cout << "Map reading completed...\n";
}

void TileManager::make_random_map()
{
	std::cout << "Random map making started\n";
	if( mapLoaded )
		delete [] loadedMap;
	
	loadedMap = new Tile[ MAP_SIZE ];
	if( loadedMap == 0 )
	{
		std::cout << "Throwing in TileManager::make_random_map after making new tile-array\n";
		throw( ERR::Out_Of_Memory() );
	}

	bool finishPlaced = false;
	unsigned int finish = rand() % MAP_SIZE;
	std::cout << "Finish tile is set on " << finish << std::endl;

	unsigned int numberExtraTiles = MAP_SIZE / 5;

	unsigned int dirtTiles[ numberExtraTiles ];
	unsigned int flowerTiles[ numberExtraTiles ];

	for( unsigned int j = 0; j < numberExtraTiles; j++ )
	{
		dirtTiles[ j ] = rand() % MAP_SIZE;
		flowerTiles[ j ] = rand() % MAP_SIZE;
	}

	unsigned int depth = 0;
	unsigned int height = 0;

	for( unsigned int i = 0; i < MAP_SIZE; i++ )
	{
		bool tile_placed = false;
		
		if( depth == MAP_WIDTH )
		{
			height += TILE_HEIGHT;
			depth = 0;
		}
		for( unsigned int k = 0; k < numberExtraTiles; k++ )
		{
			if( i == dirtTiles[ k ] )
			{
				loadedMap[ i ].set_type( DIRT_TILE );
				tile_placed = true;
			}
			if( i == flowerTiles[ k ] )
			{
				loadedMap[ i ].set_type( FLOWER_TILE );
				tile_placed = true;
			}
		}
		if( i == finish )
		{
			loadedMap[ i ].set_type( FINISH_TILE );
			finishPlaced = true;
			tile_placed = true;
		}
		if( tile_placed == false )
			loadedMap[ i ].set_type( GRASS_TILE );

		loadedMap[ i ].get_inner().x = depth;
		loadedMap[ i ].get_inner().y = height;
		loadedMap[ i ].get_inner().w = TILE_WIDTH;
		loadedMap[ i ].get_inner().h = TILE_HEIGHT;

		depth += TILE_WIDTH;
	}		
	
	//read_map(); // Uncomment voor het uitschrijven van de map per tegel naar het scherm

	mapLoaded = true;
	std::cout << "Made random map\n";
}

void TileManager::load_map( std::string )
{
}

bool TileManager::is_in_view( Tile* tile )
{
	SDL_Rect cam = camera.get_view();
	SDL_Rect& tileB = tile->get_inner();

	if( ( tileB.y + tileB.h ) < cam.y ||
		tileB.y > ( cam.y + cam.h ) ||
		( tileB.x + tileB.w ) < cam.x ||
		tileB.x > ( cam.x + cam.w ) )
		return false;
		
	return true;
}

SDL_Surface* TileManager::draw_map()
{
	if( mapLoaded )
	{
		SDL_Surface* screen = SDL_CreateRGBSurface( SDL_HWSURFACE, MAP_WIDTH, MAP_HEIGHT, 32, 0, 0, 0, 0 );
		if( screen == 0 )
			return NULL;
		for( int i = 0; i < MAP_SIZE; i++ )
		{
			if( is_in_view( &loadedMap[ i ] ) )
				loadedMap[ i ].show( screen, clipSheet, &itsTiles[ loadedMap[ i ].get_type() - 1 ] );
		}
		return screen;
	}
	return NULL;
}

Tile* TileManager::get_finish_tile()
{
	if( mapLoaded == false )
		return NULL;

	for( int i = 0; i < MAP_SIZE; i++ )
	{
		if( loadedMap[ i ].get_type() == FINISH_TILE )
			return &loadedMap[ i ];
	}
		
	return NULL;
}

bool TileManager::check_tiles_loaded()const
{
	return tilesLoaded;
}

bool TileManager::check_map_loaded()const
{
	return mapLoaded;
}

unsigned int TileManager::get_map_width()
{
	return MAP_WIDTH;
}

unsigned int TileManager::get_map_height()
{
	return MAP_HEIGHT;
}


