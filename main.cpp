#include "src/ErrHandling.hpp"
#include "src/JEngine.h"
#include "src/JlibSDL.h"
#include "src/HerderMState.h"
#include "src/JlibHerder.h"

#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {

    // Engine aanzwengelen.
    JEngine game;
    game.init("Herder game", 800, 640, 32, 0);

    try {
        // Startmenu initieren
        game.change_state(HMainState::instance());

        // Main loop.
        while (game.check_running()) {
            game.handle_events();
            game.do_think();
            game.update();
            game.show();
        }
    } catch( ... ) {
        // Er is iets naar de tering gegaan, error teruggeven.
        game.clean_up();
        return -1;
    }

    // Opruimen en geen error teruggeven
    game.clean_up();
    return 0;
}
