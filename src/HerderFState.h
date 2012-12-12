#ifndef _HERDER_FINISH_STATE_
#define _HERDER_FINISH_STATE_

#ifdef __WIN32
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif

#include "ErrHandling.hpp"
#include "JEngine.h"
#include "JGameState.h"
#include "JlibHerder.h"
#include "JlibSDL.h"

#include <iostream>
#include <sstream>

using namespace jasdlib;

class highScore;

class HFinishState: public JGameState
{
private:
	// Het font om mee te schrijven
	TTF_Font* font;
	// De oppervlaktes voor de achtergrond en verschillende berichten
	SDL_Surface* background;

	SDL_Surface* screenHeader;
	SDL_Surface* scoreSheep;
 	SDL_Surface* penaltyTime;
	SDL_Surface* totalScore;

	SDL_Surface* highScores;

	highScore* scores;

	// De frame teller en wat integers om de belangrijke dingen uit de SheepManager over te nemen.
	unsigned long int frame;

	int timePlayed;
	int herdScore;

	// Functies voor de uitvoer van de 2 stadia binnen de finish state. De scoreweergave en de highscores
	void showscore_part();
	void highscore_part();

	// Bool om meteen naar het einde van de score te skippen en één om aan te geven of er gefinished is of gepauzeerd
	bool skipScene;
	bool finished;

	// Timer voor de frame-rate
	Timer fps;

	// Functie voor het ophalen van de scores van een SheepManager bij het initiëren
	void read_score_from_file();

	// Functies om de highscores te lezen en schrijven vanuit het bestand.
	void read_highscores();
	void write_highscores();
	// Functie om de hishScore objecten te sorteren op scorehoogte.
	void sort_highscores();

	static HFinishState HerderFinishState;
protected:
	HFinishState(){}

public:
	virtual ~HFinishState(){}

	virtual void init( JEngine* game );
	virtual void clean_up();
	
	virtual void pause();
	virtual void resume();
	
	virtual void handle_events( JEngine* game );
	virtual void do_think( JEngine* game );
	virtual void update( JEngine* game );
	virtual void show( JEngine* game );

	// Zet bij het oproepen van de unieke state meteen de sheepmanager waar het om gaat vast.
	static HFinishState* instance(){ return &HerderFinishState; }

	// De knop om interactie mee te hebben
	Button* pressButton;
};


class highScore {
private:
	std::string i_name;
	long i_score;
public:
	highScore(){}
	~highScore(){}

	/**
	 * Set the highScore item.
	 */
	void set(std::string n, long s) {
		i_name = n;
		if( s > 0 ) i_score = s;
	}
	/**
	 * Return the owner of the score.
	 */
	std::string name() const {
		return i_name;
	}
	/**
	 * Return the score.
	 */
	long score() const {
		return i_score;
	}

};

#endif
