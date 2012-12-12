#include "JRoute.h"

int ROS_MAP_WIDTH = 0;
int ROS_MAP_HEIGHT = 0;

int ROS_SCREEN_WIDTH = 0;
int ROS_SCREEN_HEIGHT = 0;

// initieëren van de JRoute klasse
JRoute JRoute::routeInstance;

using namespace jasdlib;

// De route klasse,
//
//

void JRoute::init( int mapWidth, int mapHeight, int screenWidth, int screenHeigth )
{
	ROS_MAP_WIDTH = mapWidth;
	ROS_MAP_HEIGHT = mapHeight;

	ROS_SCREEN_WIDTH = screenWidth;
	ROS_SCREEN_HEIGHT = screenHeigth;

	routeLoaded = false;
	loadingRoute = false;

	frame = 0;

	handledMovement = false;
}

void JRoute::clean_up()
{
	PRINT( "JRoute::clean_up" );
	clear_route();
}

bool JRoute::make_waypoint( int x, int y )
{
	if( itsRoute.size() <= MAX_WAYPOINTS )
	{
		if( val_waypoint( x, y ) )
		{
			SDL_Rect temp = { x - 4, y - 4, 8, 8 };
			itsRoute.push_back( temp );
			if( itsRoute.size() == MAX_WAYPOINTS )
				stop_route_loading();
			return true;
		}
	}
	return false;
}

bool JRoute::val_waypoint( int x, int y ) // Valideerd of de nieuwe waypoint ver genoeg verwijderd is van de vorige. 
{
	// Error handling om te voorkomen dat er niets gezocht wordt.
	if( !itsRoute.empty() )
	{	
		if( distance( itsRoute.back().x, itsRoute.back().y, x, y ) > WAYPOINT_DISTANCE )
			if( ( x < ( ROS_MAP_WIDTH - EDGE_DELIMITER ) ) && ( y < ( ROS_MAP_HEIGHT - EDGE_DELIMITER ) ) )
				return true;
	}
	else
		if( ( x < ( ROS_MAP_WIDTH - EDGE_DELIMITER ) ) && ( y < ( ROS_MAP_HEIGHT - EDGE_DELIMITER ) ) )
			return true;

	return false;
}

bool JRoute::clear_route()
{
	while( !( itsRoute.empty() ) )	
		itsRoute.pop_front();

	routeLoaded = false;
	loadingRoute = false;

	return true;
}

void JRoute::start_route_loading( int x, int y )
{
	loadingRoute = true;
	make_waypoint( x, y );
}

void JRoute::stop_route_loading()
{
	loadingRoute = false;
	routeLoaded = true;
}

void JRoute::handle_events( SDL_Event* event, Camera* cam )
{
	frame++;

	if( event->type == SDL_MOUSEBUTTONDOWN )
	{
		if( event->button.button == SDL_BUTTON_LEFT )
			if( !loadingRoute )
				start_route_loading( ( cam->get_view().x + event->button.x ), (cam->get_view().y +  event->button.y ) );						
		if( event->button.button == SDL_BUTTON_RIGHT )
		{
			if( routeLoaded )
				clear_route();
		}
		handledMovement = true;
	}
	if( event->type == SDL_MOUSEBUTTONUP )
	{
		if( event->button.button == SDL_BUTTON_LEFT )
			stop_route_loading();	
		handledMovement = false;					
	}
	if( event->type == SDL_MOUSEMOTION )
	{
		if( ( event->motion.state == SDL_PRESSED ) ) 
		{
			if( loadingRoute )
				make_waypoint( ( cam->get_view().x + event->button.x ), ( cam->get_view().y + event->button.y ) );
			handledMovement = true;
		}
		lastXpos = cam->get_view().x + event->button.x;
		lastYpos = cam->get_view().y + event->button.y;
	}		 		
}

void JRoute::set_camera( Camera* cam )
{
	int xP = lastXpos - ROS_SCREEN_WIDTH / 2;
	int yP = lastYpos - ROS_SCREEN_HEIGHT / 2;

	cam->set_camera( xP, yP );
}

SDL_Surface* JRoute::draw_route()
{
	if( routeLoaded || loadingRoute )
	{
		SDL_Surface* screen = SDL_CreateRGBSurface( SDL_HWSURFACE, ROS_MAP_WIDTH, ROS_MAP_HEIGHT, 32, 0, 0, 0, 0 );

		if( screen == NULL )
		{
			printf( "Throwing error from JRoute::draw_route\n" );
			throw( ERR::Out_Of_Memory() );
		}

		SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0x00, 0xFF, 0xFF ) );

		if( clipSheetLoaded )
		{
			for( unsigned int i = 0; i < itsRoute.size(); i++ )
				SDL_BlitSurface( clipSheet, NULL, screen, &itsRoute[ i ] );
		}
		else
		{
			for( unsigned int i = 0; i < itsRoute.size(); i++ )
				SDL_FillRect( screen, &itsRoute[ i ], SDL_MapRGB( screen->format, 0xFF, 0x00, 0x00 ) );
		}

		SDL_SetColorKey( screen, SDL_SRCCOLORKEY, SDL_MapRGB( screen->format, 0, 0xFF, 0xFF ) );
	
		return screen;
	}
	
	return NULL;	
}

SDL_Rect JRoute::get_current_waypoint()
{
	if( !itsRoute.empty() )
		return itsRoute.front();
	else
		routeLoaded = false;
}

int JRoute::set_next_waypoint()
{
	if( !itsRoute.empty() )
	{
		itsRoute.pop_front();
		return 1;
	}
	else
		routeLoaded = false;
	return 0;
}

void JRoute::init_graphics( SDL_Surface* sheet )
{
	if( sheet)
	{
		clipSheet = sheet;
		clipSheetLoaded = true;
		return;
	}

	clipSheetLoaded = false;
}

int JRoute::get_waypoint_count()
{
	return itsRoute.size();
}


