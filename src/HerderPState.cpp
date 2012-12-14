#ifdef __WIN32
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif

#include "ErrHandling.hpp"
#include "JGameState.h"
#include "HerderPState.h"
#include "HerderFState.h"
#include "HerderMState.h"
#include "JlibSDL.h"

#include <fstream>

using namespace jasdlib;

HPlayState HPlayState::HerderPlayState;


int PLS_SCREEN_WIDTH = 800;
int PLS_SCREEN_HEIGHT = 640;

void HPlayState::init(JEngine* game) {
    PRINT("HPlayState::init");
    font = TTF_OpenFont("obj/sans.ttf", 45);
    if (font == NULL) {
        printf("Thowing error from HPlayState::init after loading font\n");
        throw ( ERR::Out_Of_Memory());
    }

    routeManager = JRoute::instance();

    charClips = load_image_from_file("gfx/Character.png");
    dogClips = load_image_from_file("gfx/Dog.png");
    sheepClips = load_image_from_file("gfx/Sheep.png");
    mapClips = load_image_from_file("gfx/Background.png");
    waypointClip = load_image_from_file("gfx/Waypoint.png");

    snd_Dog01 = Mix_LoadWAV("sfx/13_26.WAV");
    snd_Dog02 = Mix_LoadWAV("sfx/13_27.WAV");
    snd_Sheep01 = Mix_LoadWAV("sfx/10_49.WAV");
    snd_Sheep02 = Mix_LoadWAV("sfx/10_50.WAV");
    snd_Misc01 = Mix_LoadWAV("sfx/misc140.wav");

    if ((charClips == NULL) || (dogClips == NULL) || (sheepClips == NULL) || (mapClips == NULL)) {
        printf("Thowing error from HPlayState::init after loading clips\n");
        throw ( ERR::Out_Of_Memory());
    }

    if ((snd_Dog01 == NULL) || (snd_Dog02 == NULL) || (snd_Sheep01 == NULL) || (snd_Sheep02 == NULL) || (snd_Misc01 == NULL)) {
        printf("Thowing error from HPlayState::init after loading sound effects\n");
        throw ( ERR::Out_Of_Memory());
    }

    map = new TileManager();
    if (map == NULL) {
        printf("Thowing error from HPlayState::init after loading TileManager\n");
        throw ( ERR::Out_Of_Memory());
    }

    PLS_SCREEN_WIDTH = game->screen->clip_rect.w;
    PLS_SCREEN_HEIGHT = game->screen->clip_rect.h;

    map->init(mapClips, 8, 6, PLS_SCREEN_WIDTH, PLS_SCREEN_HEIGHT);

    map->make_random_map();
    routeManager->init(map->get_map_width(), map->get_map_height(), game->screen->w, game->screen->h);


    character = new Character(map, PLS_SCREEN_WIDTH, PLS_SCREEN_HEIGHT);
    dog = new Dog(*character);

    character->init_graphics(charClips, 75, 50, 4);
    dog->init_graphics(dogClips, 60, 75, 4);
    routeManager->init_graphics(waypointClip);

    dog->init_sounds(snd_Dog01, snd_Dog02);

    herd = new SheepManager(character, dog, map->get_finish_tile(), sheepClips, snd_Sheep01, snd_Sheep02, snd_Misc01, NUMBER_SHEEP);
    isFinished = false;

    if ((character == NULL) || (dog == NULL) || (herd == NULL)) {
        printf("Thowing error from HPlayState::init after loading game objects\n");
        throw ( ERR::Out_Of_Memory());
    }

    cameraAim = 1;

}

void HPlayState::clean_up() {
    PRINT("HPlayState::clean_up");

    if (routeManager->route_loaded() || routeManager->loading_route())
        routeManager->clean_up();

    if (snd_Dog01)
        Mix_FreeChunk(snd_Dog01);
    if (snd_Dog02)
        Mix_FreeChunk(snd_Dog02);
    if (snd_Sheep01)
        Mix_FreeChunk(snd_Sheep01);
    if (snd_Sheep02)
        Mix_FreeChunk(snd_Sheep02);
    if (snd_Misc01)
        Mix_FreeChunk(snd_Misc01);
    if (font != NULL)
        TTF_CloseFont(font);
    if (charClips != NULL)
        SDL_FreeSurface(charClips);
    if (dogClips != NULL)
        SDL_FreeSurface(dogClips);
    if (sheepClips != NULL)
        SDL_FreeSurface(sheepClips);
    if (mapClips != NULL)
        SDL_FreeSurface(mapClips);
    if (waypointClip != NULL)
        SDL_FreeSurface(waypointClip);
    if (character != NULL)
        delete character;
    if (dog != NULL)
        delete dog;
    if (herd != NULL)
        delete herd;
    if (map != NULL)
        delete map;
}

void HPlayState::pause() {
    PRINT("HPlayState::pause");
    herd->itsTimer.pause();
}

void HPlayState::resume() {
    PRINT("HPlayState::resume");
    herd->itsTimer.resume();
    if (isFinished) {
        restart();
    } else {
        character->reset();
    }
}

void HPlayState::restart() {
    PRINT("HPlayState::restart");
    map->make_random_map();

    if (herd != NULL)
        delete herd;
    herd = new SheepManager(character, dog, map->get_finish_tile(), sheepClips, snd_Sheep01, snd_Sheep02, snd_Misc01, NUMBER_SHEEP);
    character->reset();
    dog->reset();
    isFinished = false;
}

void HPlayState::handle_events(JEngine* game) {
    PRINT("HPlayState::handle_events");
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        character->handle_events(event);
        dog->handle_events(event, *character, *routeManager);
        routeManager->handle_events(&event, &map->camera);

        switch (event.type) {
            case( SDL_QUIT): game->exit();
                break;
            case( SDL_VIDEORESIZE): game->toggle_fullscreen();
                break;
            case( SDL_KEYDOWN):
            {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    write_score_to_file(herd);
                    game->push_state(HMainState::instance());
                }
                if (event.key.keysym.sym == SDLK_SPACE) {
                    switch (cameraAim) {
                        case(1): cameraAim = 2;
                            break;
                        case(2): cameraAim = 1;
                            break;
                        default: cameraAim = 1;
                            break;
                    }
                }
                if (event.key.keysym.sym == SDLK_F1) {
                    isFinished = true;
                    write_score_to_file(herd);
                    isFinished = false;
                    game->push_state(HFinishState::instance());
                }
                if (event.key.keysym.sym == SDLK_F4)
                    game->toggle_fullscreen();
                if (event.key.keysym.sym == SDLK_RETURN)
                    restart();
                break;
            }
            case( SDL_MOUSEBUTTONDOWN):
            {
                if (event.button.button == SDL_BUTTON_MIDDLE) {
                    if (cameraAim == 1) cameraAim = 2;
                    else cameraAim = 1;
                }
            }
            default: break;
        }
    }

}

void HPlayState::write_score_to_file(SheepManager* finishedHerd) {
    PRINT("HPlayState::write_score_to_file");
    std::ofstream fOut("obj/f_game.txt");

    if (isFinished)
        fOut << 1 << " ";
    else
        fOut << 0 << " ";

    unsigned int _score = finishedHerd->get_score();
    Uint32 _ticks = finishedHerd->itsTimer.get_ticks(); 
    
    fOut << _score;
    fOut << " ";
    fOut << _ticks;
    fOut << "\n";

    fOut.close();
}

void HPlayState::do_think(JEngine* game) {
    PRINT("HPlayState::do_think");
    dog->do_think(character, routeManager);
    herd->do_think();

    if (herd->get_sheep_count(1) == 0) {
        isFinished = true;
        write_score_to_file(herd);
        game->push_state(HFinishState::instance());
    }

}

void HPlayState::update(JEngine* game) {
    PRINT("HPlayState::update");
    character->move();
    dog->move();
    herd->move(character, dog);

    switch (cameraAim) {
        case( 1): character->set_camera(&map->camera);
            break;
        case( 2): dog->set_camera(&map->camera);
            break;
        default: character->set_camera(&map->camera);
            break;
    }
}

void HPlayState::show(JEngine* game) {
    PRINT("HPlayState::show");
    background = map->draw_map();

    SDL_Surface* sheeps = herd->draw_sheep();
    SDL_BlitSurface(sheeps, NULL, background, &background->clip_rect);

    character->show(background); //, &map->camera );
    dog->show(background);

    SDL_Surface* score = herd->draw_score(font, &map->camera);

    SDL_BlitSurface(score, NULL, background, &map->camera.get_view());

    SDL_Surface* waypoints = routeManager->draw_route();

    if (waypoints)
        if (SDL_BlitSurface(waypoints, &waypoints->clip_rect, background, &background->clip_rect) == -1)
            PRINT("SDL_BlitSurface( waypoints ) == -1");

    SDL_BlitSurface(background, &map->camera.get_view(), game->screen, &game->screen->clip_rect);

    if (SDL_Flip(game->screen) == -1)
        throw ( ERR::SDL_Err());

    if (fps.get_ticks() < 1000 / FRAMES_PER_SECOND) {
        SDL_Delay((1000 / FRAMES_PER_SECOND) - fps.get_ticks());
    }

    if (waypoints != NULL) SDL_FreeSurface(waypoints);
    SDL_FreeSurface(score);
    SDL_FreeSurface(sheeps);
    SDL_FreeSurface(background);
    background = NULL;

}