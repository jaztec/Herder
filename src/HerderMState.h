#ifndef _HERDER_MAIN_STATE_
#define _HERDER_MAIN_STATE_

#ifdef __WIN32
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#else
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#endif

#include "ErrHandling.hpp"
#include "JEngine.h"
#include "JGameState.h"
#include "JlibHerder.h"
#include "JlibSDL.h"

#include <iostream>
#include <sstream>

using namespace jasdlib;

class HMainState: public JGameState
{
private:
	// Het font om mee te schrijven
	TTF_Font* font;
	// De oppervlaktes voor de achtergrond en verschillende berichten
	SDL_Surface* background;
	
	SDL_Surface* logo;

	// De frame teller en wat integers om de belangrijke dingen uit de SheepManager over te nemen.
	unsigned long int frame;

	// Timer voor de frame-rate
	Timer fps;

	static HMainState HerderMainState;
protected:
	HMainState(){}

public:
	virtual ~HMainState(){}

	virtual void init( JEngine* game );
	virtual void clean_up();
	
	virtual void pause();
	virtual void resume();
	
	virtual void handle_events( JEngine* game );
	virtual void do_think( JEngine* game );
	virtual void update( JEngine* game );
	virtual void show( JEngine* game );

	// Zet bij het oproepen van de unieke state meteen de sheepmanager waar het om gaat vast.
	static HMainState* instance(){ return &HerderMainState; }

	Button* playButton;
	Button* quitButton;
	Button* resumeButton;
};



#endif
