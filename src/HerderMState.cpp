/***********************************************************************************
                                                Includes en Globalen
 ***********************************************************************************/

#include "ErrHandling.hpp"
#include "HerderMState.h"
#include "HerderPState.h"

#include <fstream>
#include <sstream>

int MAS_SCREEN_WIDTH = 800;
int MAS_SCREEN_HEIGHT = 640;

using namespace jasdlib;

/***********************************************************************************
                                                Klasse
 ***********************************************************************************/


HMainState HMainState::HerderMainState;

/*

        SheepManager* finishedHerd;
 */

void HMainState::init(JEngine* game) {
    PRINT("HMainState::init");
    font = TTF_OpenFont("obj/sans.ttf", 30);
    if (font == NULL) {
        printf("Thowing error from HFinishState::init after loading font\n");
        throw ( ERR::Out_Of_Memory());
    }

    frame = 0;

    MAS_SCREEN_WIDTH = game->screen->clip_rect.w;
    MAS_SCREEN_HEIGHT = game->screen->clip_rect.h;

    playButton = new Button("Speel demo", font, 250, 150, BUTTON_ENABLED);
    quitButton = new Button("Stop demo", font, 250, 240, BUTTON_ENABLED);
    resumeButton = new Button("Hervat demo", font, 250, 150, BUTTON_ENABLED);

    fps.start();

    background = SDL_CreateRGBSurface(SDL_HWSURFACE, MAS_SCREEN_WIDTH, MAS_SCREEN_HEIGHT, 32, 0, 0, 0, 0);
    logo = load_image_from_file("gfx/MainHeader.png");

    PRINT("Main state initiated\n");

}

void HMainState::clean_up() {
    PRINT("HMainState::clean_up");
    if (font != NULL)
        TTF_CloseFont(font);
    if (background != NULL)
        SDL_FreeSurface(background);

    if (playButton)
        delete playButton;
    if (quitButton)
        delete quitButton;
    if (resumeButton)
        delete resumeButton;
}

void HMainState::pause() {
    PRINT("HMainState::pause");
}

void HMainState::resume() {
    PRINT("HMainState::resume");
}

void HMainState::handle_events(JEngine* game) {
    PRINT("HMainState::handle_events");
    SDL_Event event;

    //if( frame > 1000 ) SDL_WaitEvent( &event );

    while (SDL_PollEvent(&event)) {
        if (playButton->handle_events(&event)) {
            PRINT("playButton pressed");
            game->change_state(HPlayState::instance());
            return;
        }
        if (quitButton->handle_events(&event)) {
            PRINT("quitButton pressed");
            game->exit();
            return;
        }
        if (resumeButton->handle_events(&event)) {
            PRINT("resumeButton pressed");
            game->pop_state();
            return;
        }

        switch (event.type) {
            case( SDL_QUIT): game->exit();
                break;
            case( SDL_VIDEORESIZE): game->toggle_fullscreen();
                break;
            case( SDL_KEYDOWN):
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    game->exit();
                if (event.key.keysym.sym == SDLK_F4)
                    game->toggle_fullscreen();
                break;
            }
        }
    }
}

void HMainState::do_think(JEngine* game) {
    PRINT("HMainState::do_think");
}

void HMainState::update(JEngine* game) {
    PRINT("HMainState::update");
    SDL_FillRect(background, &background->clip_rect, SDL_MapRGB(background->format, 0x00, 0x00, 0x00));

    SDL_Rect position;
    position.x = (MAS_SCREEN_WIDTH - logo->clip_rect.w) / 2;
    position.y = 30;
    position.w = logo->clip_rect.w;
    position.h = logo->clip_rect.h;

    SDL_BlitSurface(logo, NULL, background, &position);

    if (game->get_state_count() == 1) {
        playButton->enabled(true);
        quitButton->enabled(true);
        resumeButton->enabled(false);
    } else {
        playButton->enabled(false);
        quitButton->enabled(true);
        resumeButton->enabled(true);
    }

    if (playButton->enabled()) {
        playButton->draw(background);
    }
    if (quitButton->enabled()) {
        quitButton->draw(background);
    }
    if (resumeButton->enabled()) {
        resumeButton->draw(background);
    }
}

void HMainState::show(JEngine* game) {
    PRINT("HMainState::show");
    if (SDL_BlitSurface(background, NULL, game->screen, &game->screen->clip_rect) == -1)
        PRINT("SDL_BlitSurface( background ) == -1\n");

    if (SDL_Flip(game->screen) == -1)
        throw ( ERR::SDL_Err());

    if (fps.get_ticks() < 1000 / FRAMES_PER_SECOND) {
        SDL_Delay((1000 / FRAMES_PER_SECOND) - fps.get_ticks());
    }

    frame++;
}



