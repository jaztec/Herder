#include "src/ErrHandling.hpp"
#include "src/JEngine.h"
#include "src/JlibSDL.h"
#include "src/HerderMState.h"
#include "src/JlibHerder.h"

#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
    JEngine game;

    game.init("Herder game", 800, 640, 32, 0);

    try {
        game.change_state(HMainState::instance());
    } catch (ERR::Out_Of_Memory()) {
        printf("Mem Error received\n");
        game.clean_up();
        return 1;
    } catch (...) {
        printf("Undefined Error received\n");
        game.clean_up();
        return 1;
    }

    while (game.check_running()) {
        game.handle_events();
        game.do_think();
        game.update();
        game.show();
    }

    game.clean_up();

    return 0;
}
