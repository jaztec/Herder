/***********************************************************************************
                                                Includes en Globalen
 ***********************************************************************************/

#include "ErrHandling.hpp"
#include "HerderFState.h"
#include "HerderPState.h"
#include "HerderMState.h"

#include <fstream>
#include <sstream>

int FIS_SCREEN_WIDTH = 800;
int FIS_SCREEN_HEIGHT = 640;

using namespace jasdlib;

/***********************************************************************************
                                                Klasse
 ***********************************************************************************/


HFinishState HFinishState::HerderFinishState;

/*

        SheepManager* finishedHerd;
 */

void HFinishState::init(JEngine* game) {
    font = TTF_OpenFont("obj/sans.ttf", 30);
    if (font == NULL) {
        printf("Thowing error from HFinishState::init after loading font\n");
        throw ( ERR::Out_Of_Memory());
    }

    background = NULL;
    screenHeader = NULL;
    scoreSheep = NULL;
    penaltyTime = NULL;
    totalScore = NULL;

    pressButton = new Button("Doorgaan", font, 250, 480, BUTTON_ENABLED);
    pressButton->enabled(true);

    read_score_from_file();
    read_highscores();
    skipScene = false;

    frame = 0;

    FIS_SCREEN_WIDTH = game->screen->clip_rect.w;
    FIS_SCREEN_HEIGHT = game->screen->clip_rect.h;

    fps.start();

    background = SDL_CreateRGBSurface(SDL_HWSURFACE, FIS_SCREEN_WIDTH, FIS_SCREEN_HEIGHT, 32, 0, 0, 0, 0);

    PRINT("Finish and pauze state initiated\n");

}

void HFinishState::clean_up() {
    if (font != NULL)
        TTF_CloseFont(font);
    if (background != NULL)
        SDL_FreeSurface(background);

    if (pressButton != NULL)
        delete pressButton;
}

void HFinishState::read_score_from_file() {
    std::ifstream fIn("obj/f_game.txt");
    if (fIn != NULL) {
        int value; 
        fIn >> value;
        finished = value;
        fIn >> value;
        herdScore = value;
        fIn >> value;
        timePlayed = value / 1000;

        fIn.close();
    }
}

void HFinishState::read_highscores() {
    if (scores)
        delete [] scores;
    scores = new highScore[5];

    std::ifstream fIn("obj/hsc");
    if (fIn) {
        std::string n;
        long s;

        for (int i = 0; i < 5; i++) {
            fIn >> n;
            fIn >> s;
            scores[i].set(n, s);
        }
    }
}

void HFinishState::write_highscores() {

}

void HFinishState::sort_highscores() {

}

void HFinishState::pause() {
}

void HFinishState::resume() {
}

void HFinishState::handle_events(JEngine* game) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {

        if (pressButton->handle_events(&event)) {
            if (frame < 1000) {
                if (!skipScene)
                    skipScene = true;
            } else {
                game->change_state(HMainState::instance());
            }
        }
        switch (event.type) {
            case( SDL_QUIT): game->exit();
                break;
            case( SDL_KEYDOWN):
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    game->exit();
            }

        }
    }
}

void HFinishState::do_think(JEngine* game) {
}

void HFinishState::update(JEngine* game) {
    SDL_FillRect(background, &background->clip_rect, SDL_MapRGB(background->format, 0x00, 0x00, 0x00));

    if (!skipScene)
        showscore_part();
    else if (skipScene)
        highscore_part();

}

void HFinishState::showscore_part() {

    if (skipScene || !finished) {
        pressButton->set_caption("Naar menu", font);
        frame = 1000;
    }


    SDL_Color whiteText = {0xFF, 0xFF, 0xFF};

    std::stringstream showHeader;
    std::stringstream showScore;
    std::stringstream showTime;
    std::stringstream showTotalScore;

    int activeTimeScore = frame - 250;
    if (activeTimeScore > timePlayed)
        activeTimeScore = timePlayed;
    int activeTotalScore = herdScore - (activeTimeScore * 2);
    if (activeTotalScore < 0)
        activeTotalScore = 0;

    if (frame > 50) {
        if (finished)
            showHeader << "Gefeliciteerd! Je hebt alle schapen binnengehaald!";
        else
            showHeader << "Spel gepauzeerd";

        screenHeader = TTF_RenderText_Solid(font, showHeader.str().c_str(), whiteText);
        screenHeader->clip_rect.y = FIS_SCREEN_HEIGHT / 50;
        screenHeader->clip_rect.x = (FIS_SCREEN_WIDTH - screenHeader->clip_rect.w) / 2;

        SDL_BlitSurface(screenHeader, NULL, background, &screenHeader->clip_rect);
        SDL_FreeSurface(screenHeader);
        screenHeader = NULL;
    }
    if (frame > 150) {
        if (finished) {
            showScore << "Score: " << herdScore << " punten";

            scoreSheep = TTF_RenderText_Solid(font, showScore.str().c_str(), whiteText);
            scoreSheep->clip_rect.y = (FIS_SCREEN_HEIGHT / 2) - (scoreSheep->clip_rect.h * 2);
            scoreSheep->clip_rect.x = (FIS_SCREEN_WIDTH - scoreSheep->clip_rect.w) / 2;

            SDL_BlitSurface(scoreSheep, NULL, background, &scoreSheep->clip_rect);
            SDL_FreeSurface(scoreSheep);
            scoreSheep = NULL;
        }
    }
    if (frame > 250) {
        if (finished) {
            showTime << "Genomen tijd: " << activeTimeScore << " x 2 strafpunten";
            showTotalScore << "Totale score: " << activeTotalScore << " punten";
        } else
            showTotalScore << "Score: " << herdScore << " punten";

        if (finished) penaltyTime = TTF_RenderText_Solid(font, showTime.str().c_str(), whiteText);
        totalScore = TTF_RenderText_Solid(font, showTotalScore.str().c_str(), whiteText);
        if (finished) penaltyTime->clip_rect.y = (FIS_SCREEN_HEIGHT / 2) - (penaltyTime->clip_rect.h);
        totalScore->clip_rect.y = (FIS_SCREEN_HEIGHT / 2) + (totalScore->clip_rect.h * 2);
        if (finished) penaltyTime->clip_rect.x = (FIS_SCREEN_WIDTH - penaltyTime->clip_rect.w) / 2;
        totalScore->clip_rect.x = (FIS_SCREEN_WIDTH - totalScore->clip_rect.w) / 2;

        if (finished) SDL_BlitSurface(penaltyTime, NULL, background, &penaltyTime->clip_rect);
        SDL_BlitSurface(totalScore, NULL, background, &totalScore->clip_rect);
        if (finished) SDL_FreeSurface(penaltyTime);
        SDL_FreeSurface(totalScore);
        if (finished) penaltyTime = NULL;
        totalScore = NULL;
    }
    /*if( frame > 350 )
    {
            skipScene = true;
    }*/
}

void HFinishState::highscore_part() {
    if (scores) {
        SDL_Color whiteText = {0xFF, 0xFF, 0xFF};
        std::stringstream* txt = 0;
        int h = FIS_SCREEN_HEIGHT / 8;
        for (int i = 0; i < 5; i++) {
            if (txt)
                delete txt;
            txt = new std::stringstream;
            *txt << scores[i].name() << " " << scores[i].score();
            highScores = TTF_RenderText_Solid(font, txt->str().c_str(), whiteText);
            highScores->clip_rect.y = h * (i + 1);
            highScores->clip_rect.x = (FIS_SCREEN_WIDTH / 2) - (highScores->clip_rect.w / 2);
            SDL_BlitSurface(highScores, NULL, background, &highScores->clip_rect);
            SDL_FreeSurface(highScores);
        }
    }
}

void HFinishState::show(JEngine* game) {
    pressButton->draw(background);

    if (SDL_BlitSurface(background, NULL, game->screen, &game->screen->clip_rect) == -1)
        PRINT("SDL_BlitSurface( background ) == -1\n");

    if (SDL_Flip(game->screen) == -1)
        throw ( ERR::SDL_Err());

    if (fps.get_ticks() < 1000 / FRAMES_PER_SECOND) {
        SDL_Delay((1000 / FRAMES_PER_SECOND) - fps.get_ticks());
    }

    frame++;
}


