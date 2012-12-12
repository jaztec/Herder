#ifndef _HERDER_PLAY_STATE_H_
#define _HERDER_PLAY_STATE_H_

#ifdef __WIN32
#include "SDL.h"
#include "SDL_ttf.h"
#else
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#endif

#include "ErrHandling.hpp"
#include "JEngine.h"
#include "JGameState.h"
#include "JlibHerder.h"
#include "TileManager.h"
#include "JRoute.h"

using namespace jasdlib;

const int NUMBER_SHEEP = 30;
const int FRAMES_PER_SECOND = 100;

class HPlayState: public JGameState
{
private:
	// Het scherm van de gamestate
	SDL_Surface* background;

	// Het font wat gebruikt gaat worden
	TTF_Font* font;

	// De geladen bestanden voor deze gamestate
	SDL_Surface* charClips;
	SDL_Surface* dogClips;
	SDL_Surface* sheepClips;
	SDL_Surface* mapClips;
	SDL_Surface* waypointClip;

	// De geluiden die de dieren kunnen maken
	Mix_Chunk* snd_Dog01;
	Mix_Chunk* snd_Dog02;
	Mix_Chunk* snd_Sheep01;
	Mix_Chunk* snd_Sheep02;
	Mix_Chunk* snd_Misc01;

	// De routemanager om de route voor de hind uit te tekenen	
	JRoute* routeManager;

	// De pointers naar de hoofdrolspelers van het spel
	Character* character;
	Dog* dog;
	SheepManager* herd;
	TileManager* map;

	// Timer voor de frame-rate
	Timer fps;

	// Bool om te kijken of de game zich in een finishstadium bevindt
	bool isFinished;

	// Integer om te bepalen wat de camera moet volgen
	Usint cameraAim;

	// void om de waarden van de SheepManager eindscore over te brengen
	void write_score_to_file( SheepManager* finishedHerd );

	static HPlayState HerderPlayState;

protected:
	HPlayState(){}

public:
	static HPlayState* instance(){ return &HerderPlayState; }
	virtual ~HPlayState(){}

	virtual void init( JEngine* game );
	virtual void clean_up();

	virtual void pause();
	virtual void resume();

	void restart();

	virtual void handle_events( JEngine* game );
	virtual void do_think( JEngine* game );
	virtual void update( JEngine* game );
	virtual void show( JEngine* game );

};

#endif
