#ifndef _JAS_GAME_STATE_H_
#define _JAS_GAME_STATE_H_

#include "JEngine.h"

class JGameState {
protected:

    JGameState() {
    }

public:

    virtual ~JGameState() {
    }

    virtual void init(JEngine* game) = 0;
    virtual void clean_up() = 0;

    virtual void pause() = 0;
    virtual void resume() = 0;

    virtual void handle_events(JEngine* game) = 0;
    virtual void do_think(JEngine* game) = 0;
    virtual void update(JEngine* game) = 0;
    virtual void show(JEngine* game) = 0;

    void change_state(JEngine* game, JGameState* gameState) {
        game->change_state(gameState);
    }

};

#endif
