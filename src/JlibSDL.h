#ifndef _JAS_LIB_SDL_H_
#define _JAS_LIB_SDL_H_

/*
 * Gemaakt door Jasper van Herpt in Jan-10.
 *
 * Betreft header voor algemene SDL functies.
 *
*/

/******************************************************************
					Includes en globalen sectie
******************************************************************/
#ifdef __WIN32
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#else
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_ttf.h"
#endif

#include <cmath>
#include <string>

#include "ErrHandling.hpp"

/**
 * Name space start
 */
namespace jasdlib {

/**
 * Basic functions defines, basic 0x0...
 * Prefix excluded when necessary for indicating values
 */
#define		JLIB_COLLIDE_NONE			0x00000000
#define 	JLIB_COLLIDE_TOP 			0x00000001
#define 	JLIB_COLLIDE_LEFT 			0x00000002
#define 	JLIB_COLLIDE_RIGHT 			0x00000004
#define 	JLIB_COLLIDE_BOTTOM                     0x00000008
#define		JLIB_COLLIDE_TOPLEFT                    0x00000010
#define		JLIB_COLLIDE_TOPRIGHT                   0x00000020
#define		JLIB_COLLIDE_BOTTOMLEFT                 0x00000040
#define		JLIB_COLLIDE_BOTTOMRIGHT                0x00000080
/**
 * Class Timer defines, Timer 0x1...
 * Prefix excluded when necessary for indicating values
 */
#define		TIMER_TICKS_NONE			0x00000000
#define		TIMER_TICKS_STARTED			0x10000001
#define		TIMER_TICKS_PAUSED			0x10000002
#define		TIMER_RUNNING				0x10000004
#define		TIMER_PAUSED				0x10000008
/**
 * Class Button defines, Button 0x2...
 * Prefix excluded when necessary for indicating values
 */
#define		BUTTON_ENABLED				0x20000001
#define		BUTTON_CLICKED				0x20000002
#define		BUTTON_DEFAULT_WIDTH                    0x0000012C
#define		BUTTON_DEFAULT_HEIGHT                   0x0000003C
/**
 * 2DObject classes defines, 2DObject 0x4...
 * Prefix excluded when necessary for indicating values
 */
#define		JGOBJ_IS_ACTIVE				0x40000001
#define		JGOBJ_IS_ALIVE				0x40000002
#define		JGOBJ_IS_DEAD				0x40000004
#define		JGOBJ_IS_FACING_LEFT                    0x40000008
#define		JGOBJ_IS_FACING_RIGHT                   0x40000010
#define		JGOBJ_IS_FACING_UP			0x40000020
#define		JGOBJ_IS_FACING_DOWN                    0x40000040

#define		JMGOBJ_IS_MOVING			0x40000100
#define		JMGOBJ_MOVING_UP			0x40000200
#define		JMGOBJ_MOVING_DOWN			0x40000400
#define		JMGOBJ_MOVING_LEFT			0x40000800
#define		JMGOBJ_MOVING_RIGHT			0x40001000

#define		JFGOBJ_IS_FALLING			0x40010000
#define		JFGOBJ_IS_RISING			0x40020000
#define		JFGOBJ_IS_SUPPORTED			0x40040000

/**
 * Internal class Timer, uses SDL millisecond calculations for precision frame counting
 */
class Timer {
public:
	Timer();
	Timer(const Timer& rhs);
	virtual ~Timer(){}

	virtual void start();
	virtual void stop();
	virtual void pause();
	virtual void resume();
	virtual void reset();

	virtual Uint32 get_ticks( Uint32 = TIMER_TICKS_NONE )const;

	virtual bool is_started()const;
	virtual bool is_paused()const;
protected:
	Uint32 startTicks;
	Uint32 pausedTicks;

	Uint32 i_settings;
};

/**
 * Basis of all objects in the game, all to be used object must derive from these base classes.
 */
class J2DGameObject {
public:
	J2DGameObject();
	J2DGameObject(int x, int y);
	J2DGameObject(int x, int y, int w, int h);
	virtual ~J2DGameObject();
	/**
	 * Abstract function so class must be overridden.
	 */
	virtual void draw( SDL_Surface* screen ) = 0;

	virtual SDL_Rect* get_inner();
	J2DGameObject* get_base();
	unsigned int get_settings() const;
	/**
	 * Functions for getting info about the status of the object.
	 */
	bool is_active() const;
	bool is_alive() const;
	bool is_facing_up() const;
	bool is_facing_down() const;
	bool is_facing_left() const;
	bool is_facing_right() const;
	/**
	 * Functions for setting the status of the object.
	 */
	void set_active();
	void set_unactive();
	void set_dead();
	void set_inner( SDL_Rect* other );
	void set_facing(unsigned int facing);
	/**
	 * Functions for setting the different internal parameters.
	 */
	void set_pos( int x, int y);
	void set_size( int w, int h );

protected:
	Uint16 x_pos, y_pos;
	Uint16 width, height;
	SDL_Rect* inner;

	Uint32 i_settings;

	void updateInner();
};

/**
 * J2DMovGameObject
 */
class J2DMovGameObject : public J2DGameObject {
public:
	J2DMovGameObject();
	J2DMovGameObject(int x, int y );
	J2DMovGameObject(int x, int y, int w, int h );
	J2DMovGameObject(int x, int y, int w, int h, float xs, float ys);
	virtual ~J2DMovGameObject();

	virtual void update();

	float get_xspeed() const;
	float get_yspeed() const;

	bool is_moving() const;
	void start_moving();
	void stop_moving();

protected:
	float x_speed, y_speed;
};

/**
 * J2DPhysicalGameObject
 */
class J2DPhysicalGameObject : public J2DMovGameObject {
public:
	J2DPhysicalGameObject();
	J2DPhysicalGameObject(int x, int y );
	J2DPhysicalGameObject(int x, int y, int w, int h );
	J2DPhysicalGameObject(int x, int y, int w, int h, float we );
	virtual ~J2DPhysicalGameObject();

	float get_weight() const;
	void set_weight(float w);

	/**
	 * Overloading the update function for physics.
	 */
	virtual void update();
	/**
	 * Check if this object is supported by another object.
	 */
	bool check_supported_by_object(J2DGameObject* target);
	/**
	 *
	 */
	void start_falling();
	void stop_falling();
	void start_jump();
	void stop_jump();

protected:
	float weight;
};

class Button : public J2DGameObject {
public:
	Button( std::string text, TTF_Font* font, int x_pos = 0, int y_pos = 0, Uint32 flags = 0 );
	~Button();

	bool handle_events( SDL_Event* event );
	void draw( SDL_Surface* surface );

	bool enabled() const;
	void enabled( bool flag );

	std::string get_caption() const;
	void set_caption( std::string cap, TTF_Font* font );

	int get_x() const;
	int get_y() const;

protected:
	SDL_Surface* visual;
	Mix_Chunk* click_sound;
	std::string caption;
};

/**
 * General functions
 */

SDL_Surface* load_image_from_file( const char* fileName );

double distance( int x1, int y1, int x2, int y2 ) ;

bool is_collision( SDL_Rect& rect1, SDL_Rect& rect2 );
bool is_collision( SDL_Rect& rect1, SDL_Rect& rect2, int side );

bool in_range( int x_c, int y_c, SDL_Rect& rect );

} // End name space jasdlib

#endif