
#ifndef _JAS_GAME_ENGINE_H_
#define _JAS_GAME_ENGINE_H_

#ifdef __WIN32
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#else
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#endif /* __WIN32 */

#include <vector>
#include <deque>
#include <string>

#define	ISRUNNING		0x00000001
#define ISFULLSCREEN            0x00000002

class JGameState;

class JEngine {
private:
    std::vector<JGameState*> gameStates;

    unsigned int i_settings;

    void start_running() {
        i_settings |= ISRUNNING;
    }

public:
    SDL_Surface* screen;

    void init(const char* title, int width = 640, int height = 480, int bpp = 0, unsigned int flags = 0);
    void clean_up();

    void change_state(JGameState* gameState);
    void push_state(JGameState* gameState);
    void pop_state();

    void handle_events();
    void do_think();
    void update();
    void show();

    void toggle_fullscreen();

    unsigned int get_state_count() const {
        return gameStates.size();
    }

    bool check_running() {
        return ((i_settings & ISRUNNING) == ISRUNNING);
    }

    bool is_fullscreen() {
        return ((i_settings & ISFULLSCREEN) == ISFULLSCREEN);
    }

    void exit() {
        i_settings &= ~ISRUNNING;
    }

};

#endif
