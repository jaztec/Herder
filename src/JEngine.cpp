
#include "ErrHandling.hpp"
#include "JEngine.h"
#include "JGameState.h"

void JEngine::init( const char* title, int width, int height, int bpp, unsigned int flags )
{
	PRINT( "JEngine::init" );
	unsigned int flag;	

	i_settings = flags;

	// Initieër SDL en gooi een error als dit mislukt
	if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
		throw( ERR::SDL_Err() );
	if( TTF_Init() == -1 )
		throw( ERR::SDL_Err() );
	if( Mix_OpenAudio( 44100, AUDIO_S16SYS, 2, 1024 ) == -1 )
		throw( ERR::SDL_Err() );

	// Stel de caption in
	SDL_WM_SetCaption( title, title );
	
	// Stel de vlaggen in
	if( (i_settings & ISFULLSCREEN) == ISFULLSCREEN )
		flag = SDL_HWSURFACE | SDL_HWACCEL | SDL_DOUBLEBUF | SDL_FULLSCREEN;
	else
		flag = SDL_HWSURFACE | SDL_HWACCEL | SDL_DOUBLEBUF | SDL_RESIZABLE;

	// Maak het scherm
	screen = SDL_SetVideoMode( width, height, bpp, flag );

	start_running();

	printf( "Game engine fired up\n" );

}

void JEngine::clean_up()
{
	PRINT( "JEngine::clean_up" );
	// Zorg ervoor dat SDL verkleind voor afsluiten zodat andere programmaś veilig zijn wanneer er iets fout gaat
	screen = SDL_SetVideoMode( 640, 480, 0, SDL_RESIZABLE );
	SDL_Flip( screen );

	// Maak de vector met gamestates leeg en zorg dat deze netjes opruimen.
	while( !gameStates.empty() )
	{
		gameStates.back()->clean_up();
		gameStates.pop_back();
	}


	// Sluit SDL
	Mix_CloseAudio();
	TTF_Quit();
	SDL_Quit();

	printf( "Game engine terminated\n" );
}

void JEngine::change_state( JGameState* gameState )
{
	PRINT( "JEngine::change_state" );
	// Als er een gamestate draait sluit deze dan netjes af
	if( !gameStates.empty() )
	{
		gameStates.back()->clean_up();
		gameStates.pop_back();
	}

	// Zet de nieuwe state in en init die
	gameStates.push_back( gameState );
	gameStates.back()->init( this );

}

void JEngine::push_state( JGameState* gameState )
{
	PRINT( "JEngine::push_state" );
	// Als er een gamestate draait pauzeer die dan
	if( !gameStates.empty() )
	{
		gameStates.back()->pause();
	}

	// Zet de nieuwe state in en init die
	gameStates.push_back( gameState );
	gameStates.back()->init( this );

}

void JEngine::pop_state()
{
	PRINT( "JEngine::pop_state" );
	// Als er een gamestate draait sluit deze dan netjes af
	if( !gameStates.empty() )
	{
		gameStates.back()->clean_up();
		gameStates.pop_back();
	}

	// Als er een gameState aanwezig is hervat deze dan
	if( !gameStates.empty() )
	{
		gameStates.back()->resume();
	}

}

void JEngine::handle_events()
{
	if( !gameStates.empty() )
		gameStates.back()->handle_events( this );
}

void JEngine::do_think()
{
	if( !gameStates.empty() )
		gameStates.back()->do_think( this );
}

void JEngine::update()
{
	if( !gameStates.empty() )
		gameStates.back()->update( this );
}

void JEngine::show()
{
	if( !gameStates.empty() )
		gameStates.back()->show( this );
}

void JEngine::toggle_fullscreen()
{
	unsigned int flag = 0x00000000;

	if( !is_fullscreen() )
	{
		i_settings |= ISFULLSCREEN; // Zet de bitwise boolean fullscreen aan.
		flag = SDL_HWSURFACE | SDL_HWACCEL | SDL_DOUBLEBUF | SDL_FULLSCREEN;
	}
	else
	{
		i_settings &= ~ISFULLSCREEN; // Zet de bitwise boolean fullscreen uit.
		flag = SDL_HWSURFACE | SDL_HWACCEL | SDL_DOUBLEBUF | SDL_RESIZABLE;
	}

	// Maak het scherm
	screen = SDL_SetVideoMode( screen->clip_rect.w, screen->clip_rect.h, 0, flag );

}

