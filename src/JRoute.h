#ifndef _JAS_ROUTE_MANAGER_H_
#define _JAS_ROUTE_MANAGER_H_

/************************************************************************************************************
                                                                                        Globalen en includes
 *************************************************************************************************************/
#ifdef __WIN32
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif

#include "ErrHandling.hpp"
#include "JlibSDL.h"

#include <deque>

#include "TileManager.h"

// Route specifieke integers
const unsigned int MAX_WAYPOINTS = 500;
const unsigned int WAYPOINT_DISTANCE = 100;
const unsigned int EDGE_DELIMITER = 30;

/*******************************************************************************************
                                                        Begin van klasse headers
 *******************************************************************************************/

// De route klasse, deze klasse kan een route laden en deze weer 'afspelen'.
//
//

class JRoute {
private:
    // Deque om de route waypoints op te slaan
    std::deque<SDL_Rect> itsRoute;

    // Bools om de staat van de route te monitoren
    bool routeLoaded;
    bool loadingRoute;

    // Bool om te kijken of er een Surface ingeladen is voor de waypoints.
    // Als dit waar is gebruik dan de Surface, anders rode SDL_Rects
    SDL_Surface* clipSheet;
    bool clipSheetLoaded;

    // functie om een waypoint te maken en de route te verwijderen, daarnaast een om te bepalen of de nieuwe waypoint ver genoeg van de vorige verwijderd is
    bool make_waypoint(int x, int y);
    bool val_waypoint(int x, int y);
    bool clear_route();

    // functies voor het starten en stoppen van het laden van routes
    void start_route_loading(int x, int y);
    void stop_route_loading();

    // integers om bij te houden hoeveel frames de route al draait
    unsigned long frame;

    // integers om vast te houden waar de muis zich momenteel bevind. Hier kan de camera dan op focussen
    unsigned short lastXpos;
    unsigned short lastYpos;

    static JRoute routeInstance;

protected:

    JRoute() {
    }

public:

    ~JRoute() {
    }

    static JRoute* instance() {
        return &routeInstance;
    }

    void init(int mapWidth = 800, int mapHeight = 640, int screenWidth = 840, int screenHeigth = 640);
    void clean_up();

    void handle_events(SDL_Event* event, Camera* cam);
    void set_camera(Camera* cam);

    SDL_Surface* draw_route();

    SDL_Rect get_current_waypoint();

    int set_next_waypoint(); // Functie geeft 0 terug wanneer er geen waypoints meer zijn

    void init_graphics(SDL_Surface* sheet);

    int get_waypoint_count();

    bool route_loaded() const {
        return routeLoaded;
    }

    bool loading_route() const {
        return loadingRoute;
    }

    // Bool om te kijken of er met een ingedrukte muis bewogen is deze frame.
    bool handledMovement;

};


#endif
