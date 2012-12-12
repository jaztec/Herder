#include "JlibSDL.h"


/******************************************************************************************
					Klasse functies
******************************************************************************************/

/**
 * Name space
 */
namespace jasdlib{

/**
 * Timer
 */
Timer::Timer(){
	i_settings = 0;
	startTicks = 0;
	pausedTicks = 0;
}

Timer::Timer(const Timer& rhs)
{
	startTicks = rhs.get_ticks( TIMER_TICKS_STARTED );
	pausedTicks = rhs.get_ticks( TIMER_TICKS_PAUSED );
	if( rhs.is_started() ) i_settings |= TIMER_RUNNING;
	if( rhs.is_paused() ) i_settings |= TIMER_PAUSED;
}
void Timer::start()
{
	i_settings |= TIMER_RUNNING;
	i_settings &= ~TIMER_PAUSED;
	startTicks = SDL_GetTicks();
}
void Timer::stop()
{
	i_settings = 0;
}
void Timer::pause()
{
	if( ( is_started() ) && ( !is_paused() ) )
	{
		i_settings |= TIMER_PAUSED;
		pausedTicks = SDL_GetTicks() - startTicks;
	}
}
void Timer::resume()
{
	if( is_paused() )
	{
		i_settings &= ~TIMER_PAUSED;
		startTicks = SDL_GetTicks() - pausedTicks;
		pausedTicks = 0;
	}
} 
void Timer::reset()
{
	stop();
	start();
}
Uint32 Timer::get_ticks( Uint32 tick )const
{
	if( tick != 0 )
	{
		switch( tick )
		{
			case( TIMER_TICKS_STARTED ): return startTicks;break;
			case( TIMER_TICKS_PAUSED ): return pausedTicks;break;
			default: return 0;break; // If the requesting param is other then the possible return 0
		}
	}

	if( is_started() )
	{
		if( is_paused() )
			return pausedTicks;
		else
			return SDL_GetTicks() - startTicks;
	}

	return 0;
}
bool Timer::is_started()const
{
	return (( i_settings & TIMER_RUNNING ) == TIMER_RUNNING);
}
bool Timer::is_paused()const
{
	return (( i_settings & TIMER_PAUSED ) == TIMER_PAUSED);
}

/**
 * Object classes
 */
/**
 * J2DGameObject
 */
J2DGameObject::J2DGameObject()
{
	inner = new SDL_Rect();
	updateInner();
}
J2DGameObject::J2DGameObject(int x, int y)
{
	inner = new SDL_Rect();
	x_pos = x;
	y_pos = y;
	updateInner();
}
J2DGameObject::J2DGameObject(int x, int y, int w, int h)
{
	inner = new SDL_Rect();
	x_pos = x;
	y_pos = y;
	width = w;
	height = h;
	updateInner();
}
J2DGameObject::~J2DGameObject()
{
	if( inner != 0 )
		delete inner;
}
SDL_Rect* J2DGameObject::get_inner()
{
		if( inner != 0 ) {
			return inner;
		}
		return 0;
	}
J2DGameObject* J2DGameObject::get_base()
{
	return this;
}
unsigned int J2DGameObject::get_settings() const
{
	return i_settings;
}
bool J2DGameObject::is_active() const
{
	return ((i_settings & JGOBJ_IS_ACTIVE) == JGOBJ_IS_ACTIVE);
}
bool J2DGameObject::is_alive() const
{
	return ((i_settings & JGOBJ_IS_ALIVE) == JGOBJ_IS_ALIVE);
}
bool J2DGameObject::is_facing_up() const
{
	return ((i_settings & JGOBJ_IS_FACING_UP) == JGOBJ_IS_FACING_UP);
}
bool J2DGameObject::is_facing_down() const
{
	return ((i_settings & JGOBJ_IS_FACING_DOWN) == JGOBJ_IS_FACING_DOWN);
}
bool J2DGameObject::is_facing_left() const
{
	return ((i_settings & JGOBJ_IS_FACING_LEFT) == JGOBJ_IS_FACING_LEFT);
}
bool J2DGameObject::is_facing_right() const
{
	return ((i_settings & JGOBJ_IS_FACING_RIGHT) == JGOBJ_IS_FACING_RIGHT);
}
void J2DGameObject::set_active()
{
	i_settings |= JGOBJ_IS_ACTIVE;
}
void J2DGameObject::set_unactive()
{
	i_settings &= ~JGOBJ_IS_ACTIVE;
}
void J2DGameObject::set_dead()
{
	i_settings &= ~JGOBJ_IS_ALIVE;
	i_settings |= JGOBJ_IS_DEAD;
}
void J2DGameObject::set_inner( SDL_Rect* other )
{
	if( inner )
		delete inner;
	inner = new SDL_Rect;
	*inner = *other;
}
void J2DGameObject::set_facing(unsigned int facing)
{
	i_settings &= ~(JGOBJ_IS_FACING_LEFT | JGOBJ_IS_FACING_RIGHT | JGOBJ_IS_FACING_UP | JGOBJ_IS_FACING_DOWN);

	if(facing == JGOBJ_IS_FACING_LEFT)
		i_settings |= JGOBJ_IS_FACING_LEFT;
	else if(facing == JGOBJ_IS_FACING_RIGHT)
		i_settings |= JGOBJ_IS_FACING_RIGHT;
	else if(facing == JGOBJ_IS_FACING_UP)
		i_settings |= JGOBJ_IS_FACING_UP;
	else if(facing == JGOBJ_IS_FACING_DOWN)
		i_settings |= JGOBJ_IS_FACING_DOWN;
	else
		i_settings |= JGOBJ_IS_FACING_DOWN;
}
void J2DGameObject::set_pos( int x, int y)
{
	x_pos = x;
	y_pos = y;
	updateInner();
}
void J2DGameObject::set_size( int w, int h ){
	width = w;
	height = h;
	updateInner();
}
void J2DGameObject::updateInner()
{
	inner->x = x_pos;
	inner->y = y_pos;
	inner->w = width;
	inner->h = height;
}

/**
 * J2DMovGameObject
 */
J2DMovGameObject::J2DMovGameObject()
{}
J2DMovGameObject::J2DMovGameObject(int x, int y ):
	J2DGameObject(x, y)
{}
J2DMovGameObject::J2DMovGameObject(int x, int y, int w, int h ):
	J2DGameObject(x, y, w, h)
{}
J2DMovGameObject::J2DMovGameObject(int x, int y, int w, int h, float xs, float ys):
	J2DGameObject(x, y, w, h)
{
	x_speed = xs;
	y_speed = ys;
}
J2DMovGameObject::~J2DMovGameObject()
{}

void J2DMovGameObject::update()
{
	x_pos += x_speed;
	y_pos += y_speed;
	updateInner();
}

float J2DMovGameObject::get_xspeed() const
{
	return x_speed;
}
float J2DMovGameObject::get_yspeed() const
{
	return y_speed;
}

bool J2DMovGameObject::is_moving() const
{
	return ((i_settings & JMGOBJ_IS_MOVING) == JMGOBJ_IS_MOVING);
}
void J2DMovGameObject::start_moving()
{
	i_settings |= JMGOBJ_IS_MOVING;
}
void J2DMovGameObject::stop_moving()
{
	i_settings &= ~JMGOBJ_IS_MOVING;
}

/**
 * J2DPhysicalGameObject
 */
J2DPhysicalGameObject::J2DPhysicalGameObject()
{
	weight = 1.0f;
}
J2DPhysicalGameObject::J2DPhysicalGameObject(int x, int y ):
	J2DMovGameObject(x, y)
{
	weight = 1.0f;
}
J2DPhysicalGameObject::J2DPhysicalGameObject(int x, int y, int w, int h ):
	J2DMovGameObject(x, y, w, h)
{
	weight = 1.0f;
}
J2DPhysicalGameObject::J2DPhysicalGameObject(int x, int y, int w, int h, float we ):
		J2DMovGameObject(x, y, w, h){
		weight = we;
	}
J2DPhysicalGameObject::~J2DPhysicalGameObject()
{}
float J2DPhysicalGameObject::get_weight() const
{
	return weight;
}
void J2DPhysicalGameObject::set_weight(float w)
{
	weight = w;
}
void J2DPhysicalGameObject::update()
{
	if( JFGOBJ_IS_FALLING )
		y_speed += weight/20;

	J2DMovGameObject::update();
}
bool J2DPhysicalGameObject::check_supported_by_object(J2DGameObject* target)
{
	unsigned int dir = is_collision(*target->get_inner(), *this->get_inner());
	if(  dir == JLIB_COLLIDE_BOTTOM ||
             dir == JLIB_COLLIDE_BOTTOMLEFT ||
             dir == JLIB_COLLIDE_BOTTOMRIGHT ||
             dir == JLIB_COLLIDE_RIGHT ||
             dir == JLIB_COLLIDE_LEFT)
		return true;
	else
		return false;
}
void J2DPhysicalGameObject::start_falling()
{
	i_settings &= ~JFGOBJ_IS_SUPPORTED;
	i_settings |= JFGOBJ_IS_FALLING;
}
void J2DPhysicalGameObject::stop_falling()
{
	i_settings &= ~JFGOBJ_IS_FALLING;
	i_settings |= JFGOBJ_IS_SUPPORTED;
	y_speed = 0;
}
void J2DPhysicalGameObject::start_jump()
{}
void J2DPhysicalGameObject::stop_jump()
{}

/**
 * Button
 */
Button::Button( std::string text, TTF_Font* font, int x_pos, int y_pos, Uint32 flags )
{
	visual = SDL_CreateRGBSurface( SDL_HWSURFACE, BUTTON_DEFAULT_WIDTH, BUTTON_DEFAULT_HEIGHT, 32, 0, 0, 0, 0 );
	click_sound = NULL;

	set_inner( &visual->clip_rect );
	set_pos( x_pos, y_pos );
	set_size( BUTTON_DEFAULT_WIDTH, BUTTON_DEFAULT_HEIGHT );

	set_active();					// Set the inner object active
	if( flags) i_settings |= flags;	// Set the button with the given flags
	set_caption( text, font );
}
Button::~Button()
{
	if( visual ) SDL_FreeSurface( visual );
	if( click_sound )Mix_FreeChunk( click_sound );
}
bool Button::handle_events( SDL_Event* event )
{
	i_settings &= ~BUTTON_CLICKED;
	PRINT((i_settings & BUTTON_ENABLED));
	if( i_settings & BUTTON_ENABLED )
	{
		if( event->type == SDL_MOUSEBUTTONUP )
		{
			if( event->button.button == SDL_BUTTON_LEFT )
			{
				if( in_range( event->button.x,
							  event->button.y,
							  *get_inner() ) )
				{
					i_settings |= BUTTON_CLICKED;
					if( click_sound ) Mix_PlayChannel( -1, click_sound, 0 );
				}
			}
		}
	}
	return ( ( i_settings & BUTTON_CLICKED ) == BUTTON_CLICKED );
}

void Button::draw( SDL_Surface* surface )
{
	if( ( i_settings & BUTTON_ENABLED ) == BUTTON_ENABLED )
		SDL_BlitSurface( visual, NULL, surface, inner );
}

void Button::set_caption( std::string cap, TTF_Font* font )
{
	SDL_Color whiteText = { 0xFF, 0xFF, 0xFF };

	caption = cap;

	SDL_FillRect( visual, &visual->clip_rect, SDL_MapRGB( visual->format, 0xFF, 0, 0 ) );

	SDL_Surface* tempSurface = TTF_RenderText_Solid( font, caption.c_str(), whiteText );
	tempSurface->clip_rect.x = ( visual->clip_rect.w - tempSurface->clip_rect.w ) / 2;
	tempSurface->clip_rect.y = ( visual->clip_rect.h - tempSurface->clip_rect.h ) / 2;

	SDL_BlitSurface( tempSurface, NULL, visual, &tempSurface->clip_rect );

	SDL_FreeSurface( tempSurface );
}
bool Button::enabled() const
{
	return ((i_settings & BUTTON_ENABLED ) == BUTTON_ENABLED );
}
void Button::enabled( bool flag )
{
	if( flag ) i_settings |= BUTTON_ENABLED;
	else i_settings &= ~BUTTON_ENABLED;
}
std::string Button::get_caption() const
{
	return caption;
}
int Button::get_x() const
{
	return inner->x;
}
int Button::get_y() const
{
	return inner->y;
}

/******************************************************************************
						Algemene functies
******************************************************************************/

SDL_Surface* load_image_from_file( const char* fileName )
{
	// Maak 2 SDL_Surfaces, 1 om het bestand in te lezen, de ander om een geoptimaliseerd beeld terug te geven
	SDL_Surface* loadedIMG = NULL;
	SDL_Surface* optimizedIMG = NULL;

	loadedIMG = IMG_Load( fileName );

	// Als het bestand is geladen
	if( loadedIMG != NULL )
	{
		// Maak een geoptimaliseerd beeld en maak het oude leeg
		optimizedIMG = SDL_DisplayFormat( loadedIMG );
		SDL_FreeSurface( loadedIMG );

		// check of het geoptimaliseerde beeld geladen is en zet dan de colorkey om de achtergrond weg te halen
		if( optimizedIMG != NULL )
			SDL_SetColorKey( optimizedIMG, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedIMG->format, 0, 0xFF, 0xFF ) );
	}
	
	return optimizedIMG;
}

double distance( int x1, int y1, int x2, int y2 ) 
{
	//Return the distance between two points
	return sqrt( pow( x2 - x1, 2 ) + pow( y2 - y1, 2 ) ); 
} 

bool is_collision(SDL_Rect& rect1, SDL_Rect& rect2 ) {
	return false;
}
bool is_collision(SDL_Rect& rect1, SDL_Rect& rect2, int side ) {
	return false;
}

bool in_range( int x_c, int y_c, SDL_Rect& rect )
{
	if( ( x_c >= rect.x ) &&
		( x_c <= ( rect.x + rect.w ) ) &&
		( y_c >= rect.y ) &&
		( y_c <= ( rect.y + rect.h ) )
		)
			return true;
	return false;
}

} // End of name space jasdlib
