
#ifndef _JAS_HERDER_LIB_H_
#define _JAS_HERDER_LIB_H_

/*
 * Gemaakt door Jasper van Herpt in Jan-10.
 *
 * Betreft header voor alle bestuurbare characters en de NPC's in het spel 'Herder'.
 *
 */

/*******************************************************************************
                                                Includes en globalen sectie
 ********************************************************************************/
#ifdef __WIN32
#include "SDL.h"
#include "SDL_ttf.h"
#else
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#endif

#include "JlibSDL.h"
#include "ErrHandling.hpp"
#include "TileManager.h"
#include "JRoute.h"
#include "JEngine.h"

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <stdio.h>

using namespace jasdlib;

typedef unsigned short int Usint;
typedef signed short int Ssint;
typedef unsigned long int Ulint;

/**
 * Enum voor het bepalen van welke kant een object nadert.
 */
enum CLOSURE {
    ALL,
    UP,
    LEFT,
    RIGHT,
    DOWN
};

/** 
 * Enum voor het bepalen welke types de BaseClass kan vertegenwoordigen
 */
enum TYPECHAR {
    CHARACTER,
    DOG,
    SHEEP
};

/**
 * Enum voor de states waarin de BaseClass kan verkeren. Wordt gebruikt bij het bepalen van de animatie.
 */
enum STATE {
    FACE_LEFT,
    FACE_RIGHT,
    FACE_UP,
    FACE_DOWN,
    WALK_LEFT,
    WALK_RIGHT,
    WALK_UP,
    WALK_DOWN
};

// Struct voor het doorgeven van botsingen aan diverse functies.

struct COLLIDEDIRECTION {

    COLLIDEDIRECTION() : collision(0) {
    }

    ~COLLIDEDIRECTION() {
    }

    unsigned int collision;
};

// Constante integers voor het aanduiden van welke kant een botsing komt.
const Usint NO_COLLISION = 0;
const Usint COLLISION_TOPLEFT = 1;
const Usint COLLISION_TOPRIGHT = 2;
const Usint COLLISION_BOTTOMLEFT = 3;
const Usint COLLISION_BOTTOMRIGHT = 4;
const Usint COLLISION_TOP = 5;
const Usint COLLISION_BOTTOM = 6;
const Usint COLLISION_LEFT = 7;
const Usint COLLISION_RIGHT = 8;
const Usint COLLISION_FULL = 9;

// Constante integers om de afmetingen van de klassen vast te leggen
const Usint CHARACTER_WIDTH = 50;
const Usint CHARACTER_HEIGHT = 75;

const Usint DOG_VERTICAL_WIDTH = 75;
const Usint DOG_VERTICAL_HEIGHT = 60;

const Usint DOG_HORIZONTAL_WIDTH = 75;
const Usint DOG_HORIZONTAL_HEIGHT = 60;

const Usint SHEEP_VERTICAL_WIDTH = 75;
const Usint SHEEP_VERTICAL_HEIGHT = 60;

const Usint SHEEP_HORIZONTAL_WIDTH = 75;
const Usint SHEEP_HORIZONTAL_HEIGHT = 60;

// Constante integers om de snelheid vast te leggen.
const Usint X_SPEED = 10;
const Usint Y_SPEED = 10;

// Constante integer om het aantal animaties vast te leggen
const Usint MAX_ANIMATIONS = 4;

// Schaap specifieke constanten
const Usint SCARSED_SHEEP = 3;
const Usint NERVOUS_SHEEP = 2;
const Usint BRAVE_SHEEP = 1;
const Usint SCARSE_REGION = 60;
const Usint FINISH_SCORE = 40;
const Usint LEADER_DISTANCE = 300;

// Integer waarin de afstand tussen de waypoints vast gelegd wordt waarbij de waypoint weggaat 
const Usint WAYPOINT_REACHED = 40;


/*******************************************************************************************
                                                        Begin van klassen headers
 *******************************************************************************************/


// Virtuele basisklasse om de andere characters van te deriven
//

class BaseClass {
protected:
    // Protected constructor zodat de klasse enkel vanuit zijn derived klasses aangeroepen kan worden
    BaseClass(TYPECHAR tC);

    // Functie om te bepalen in welke staat het object is
    virtual void calc_state() = 0;

    // Algemene bools welke voor alle klasses gelden
    bool isWalking;
    bool clipSheetLoaded;

    // De SDL_Rect welke als collision box gaat dienen voor de verheven klasse
    SDL_Rect* box;

    // De verschillende animatieclips, deze worden per klasse opgeslagen
    SDL_Rect* clipsStanding;
    SDL_Rect* clipsWalkingLeft;
    SDL_Rect* clipsWalkingRight;
    SDL_Rect* clipsWalkingUp;
    SDL_Rect* clipsWalkingDown;

    // Een pointer naar de ingeladen animatie sheet (.png bestand)
    SDL_Surface* clipSheet;

    // 2 state variabelen om de huidige en vorige staat op te slaan
    STATE state;
    STATE lastState;

    // 3 geluideffect pointers om geluid te kunnen maken
    Mix_Chunk* sound1;
    Mix_Chunk* sound2;
    Mix_Chunk* sound3;

    // Bool voor het checken van een botsing
    bool calc_collision(SDL_Rect* base, SDL_Rect* target);

public:
    virtual ~BaseClass();

    virtual SDL_Rect& get_box() = 0;
    virtual SDL_Rect* get_box(int) = 0;
    virtual void show(SDL_Surface*) = 0;

    void init_graphics(SDL_Surface* itsGraphics, Usint height = 0, Usint width = 0, Usint loops = 0);

    virtual void init_graphics(SDL_Surface*) {
    }
    virtual void init_sounds(Mix_Chunk* snd1, Mix_Chunk* snd2 = NULL, Mix_Chunk* snd3 = NULL);

    unsigned int make_number_positive(int number);

    TYPECHAR itsCHAR;
    unsigned int frame;

    /**
     * 
     * @param index
     */
    void play_sound(int index);

};



// Character klasse, dit is de user-controlled Herder in het spel
//

class Character : public BaseClass {
private:
    // Integers voor zijn snelheid op de x en y-as
    int xVel;
    int yVel;

    // Integer welke loopt tot MAX_ANIMATIONS om in de ::show( SDL_Surface* ) de juiste animatieclip te pakken
    int actionFrame;


protected:
    void calc_state();

public:
    Character(TileManager* map, int screenWidth, int screenHeight);
    virtual ~Character();

    SDL_Rect& get_box();
    SDL_Rect* get_box(int flag);

    void handle_events(SDL_Event& event);
    void move();
    void show(SDL_Surface* screen);
    void show(SDL_Surface* screen, Camera* cam);

    void set_camera(Camera* cam);

    // Zet de snelheden van de Character op 0
    void reset();

    // Bool om te kijken of het karakter bewogen heeft deze frame.
    bool handledMovement;

};



// Dog klasse, de grote vriend van de Herder
//

class Dog : public BaseClass {
private:
    int xVel;
    int yVel;

    int actionFrame;

    // 2 functies voor de AI van de hond
    void follow_character(Character* character);
    void follow_route(JRoute* route);

    // Bool om te kijken of de hond stil moet staan of iets moet volgen
    bool stopped;
    bool followCharacter;
    bool followRoute;

    // Deze functie past de snelheden aan van de hond ten opzichte van 2 coördinaten. Wanneer de coördinaten
    // dicht bij elkaar liggen geeft de functie true terug. De laatste integer is om de afstand waarin de 
    // waypoint bereikt wordt manueel te veranderen. Standaard is dit de WAYPOINT_REACHED integer in de globalen
    bool set_velocity(int xA, int yA, int xB, int yB, int wp_distance = WAYPOINT_REACHED);

protected:
    void calc_state();

public:
    Dog(Character& character);
    virtual ~Dog();

    void handle_events(SDL_Event& event, Character& character, JRoute& route);
    void do_think(Character* character, JRoute* route);
    void move();
    void show(SDL_Surface* screen);

    void reset();

    void toggle_stopped();
    void set_stopped();

    void set_following(int stance); // Character is 1, route is 2, stoppen is 3
    void clear_following(); // deze functie maakt het volgen leeg

    int get_x_vel() const {
        return xVel;
    }

    int get_y_vel() const {
        return yVel;
    }

    bool is_stopped() const {
        return stopped;
    }

    void set_camera(Camera* cam);

    SDL_Rect& get_box();
    SDL_Rect* get_box(int flag);
};


// Sheep klasse
//
// 

class SheepManager;

class Sheep : public BaseClass {
private:
    int xVel;
    int yVel;

    TYPECHAR typeChar;
    int inner;

    int itsNumber;

    int actionFrame;

    // bools voor checkwaarden
    bool loadedOK;
    bool isFinished;
    bool isMovingRandom;

    // Bool om te vinken of het schaap bang is
    bool isScarsed; // Scan de omgeving naar een belangrijker schaap dan deze
    bool find_leader();

    // Integer teller om te kijken hoelang het schaap op random movement loopt.
    unsigned short framesMovingRandom;

    // Check of het schaap de finish overlapt
    bool calc_finish(SDL_Rect& finish);

    void goto_leader(SDL_Rect& leader);
    void move_random();

    bool Sheep::check_values();

protected:
    void calc_state();

public:
    Sheep();
    Sheep(int xPos, int yPos);
    virtual ~Sheep();

    void set_start_square(int xPos, int yPos, int width);

    void handle_events(SDL_Event& event, Character& character);
    void do_think(Character& character, Dog& dog);
    void move(Tile& finish);

    void show(SDL_Surface* screen);

    void set_its_number(int number);
    int get_its_number() const;

    void set_state(STATE state);

    void set_xVel(int);
    void set_yVel(int);
    int get_xVel()const;
    int get_yVel()const;

    int get_inner()const;

    bool is_finished()const;

    bool is_scarsed() const {
        return isScarsed;
    }

    void set_scarsed() {
        isScarsed = true;
    }

    SDL_Rect& get_box();
    SDL_Rect* get_box(int flag);

    // Behandel de random movement van het schaap.
    // Met de vlag wordt actie bepaald. false om het uit te zetten en true om een random movement in gang te zetten en op te volgen
    void random_movement(SheepManager* manager, bool flag);

    bool handledMovement;

};

// SheepManager klasse
//

class SheepManager {
private:
    Sheep* itsSheep;
    int qtySheep;

    Character* character;
    Dog* dog;

    Usint scoreCounter;
    unsigned int score;
    Timer scoreTimer;

    Tile* finish;

    bool calc_collision(SDL_Rect* sheep, SDL_Rect* target);

    bool check_closure(Sheep*, TYPECHAR, CLOSURE = ALL);
    unsigned int check_collision(Sheep*, BaseClass*);
    void handle_collision(Sheep*, COLLIDEDIRECTION);


public:
    SheepManager(Character* character, Dog* hond, Tile* finish, SDL_Surface* clipSheet, Mix_Chunk* snd1 = NULL, Mix_Chunk* snd2 = NULL, Mix_Chunk* snd3 = NULL, int = 2);
    ~SheepManager();

    void do_think();
    void move(BaseClass* character, BaseClass* dog);
    int get_sheep_count(int flag = 0)const; // 0 = totaal, 1 = aantal schapen actief in het spel, 2 = aantal schapen gefinished

    SDL_Surface* draw_sheep();
    SDL_Surface* draw_score(TTF_Font* font, Camera* cam);

    unsigned int get_score()const {
        return score;
    }

    // Deze set_velocity() gebruikt een schaap en een SDL_Rect als doel. Wanneer het doel een bedreiging is wordt de tolerantieafstand intern verlaagd
    bool set_velocity(Sheep* sheep, SDL_Rect* target, bool thread = false);

    // Scan de omgeving naar een belangrijker schaap dan deze
    SDL_Rect find_leader(Sheep* sheep);


    Timer itsTimer;

};
#endif

