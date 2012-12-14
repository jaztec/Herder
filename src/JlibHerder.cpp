
#include "JlibHerder.h"

/*********************************************************************
                                                Globalen
 *********************************************************************/

// Tekstkleur voor tekst in de game
SDL_Color textColor = {0x13, 0x11, 0x7A};

int H_MAP_WIDTH = 0;
int H_MAP_HEIGHT = 0;
int H_SCREEN_WIDTH = 0;
int H_SCREEN_HEIGHT = 0;

using namespace jasdlib;
/*********************************************************************
                                                Klasses
 *********************************************************************/

// Klasse BaseClass
//

// Constructor, verzamel zoveel mogelijk geheugen voor het object

BaseClass::BaseClass(TYPECHAR tC) :
isWalking(false),
clipSheetLoaded(false),
itsCHAR(tC) {
    box = new SDL_Rect;
    clipsStanding = new SDL_Rect[ MAX_ANIMATIONS ];
    clipsWalkingLeft = new SDL_Rect[ MAX_ANIMATIONS ];
    clipsWalkingRight = new SDL_Rect[ MAX_ANIMATIONS ];
    clipsWalkingUp = new SDL_Rect[ MAX_ANIMATIONS ];
    clipsWalkingDown = new SDL_Rect[ MAX_ANIMATIONS ];
    clipSheet = NULL;

    sound1 = NULL;
    sound2 = NULL;
    sound3 = NULL;

    if ((box == 0) || (clipsStanding == 0) || (clipsWalkingLeft == 0) ||
            (clipsWalkingRight == 0) || (clipsWalkingUp == 0) ||
            (clipsWalkingDown == 0)) {
        printf("Throwing out of memory from BaseClass constructor\n");
        throw ( ERR::Out_Of_Memory());
    }
}

// Destructor, maak het verzamelde geheugen weer vrij

BaseClass::~BaseClass() {
    if (box)
        delete box;
    if (clipsStanding)
        delete [] clipsStanding;
    if (clipsWalkingLeft)
        delete [] clipsWalkingLeft;
    if (clipsWalkingRight)
        delete [] clipsWalkingRight;
    if (clipsWalkingUp)
        delete [] clipsWalkingUp;
    if (clipsWalkingDown)
        delete [] clipsWalkingDown;
}

// Extra functie van de BaseClass, maakt elke integer positief

unsigned int BaseClass::make_number_positive(int number) {
    if (number > 0)
        return number;
    else if (number < 0)
        return ( number -= (number * 2));
    else
        return 0;
}

bool BaseClass::calc_collision(SDL_Rect* base, SDL_Rect* target) {
    if ((target->y + target->h) < base->y ||
            target->y > (base->y + base->h) ||
            (target->x + target->w) < base->x ||
            target->x > (base->x + base->w))
        return false;

    return true;
}

void BaseClass::init_graphics(
        SDL_Surface* itsGraphics,
        Usint height = 0,
        Usint width = 0,
        Usint loops = 0
        ) {
    Usint i = 0;
    Usint line = 0;

    if (itsGraphics) {
        clipSheet = itsGraphics;
        clipSheetLoaded = true;
    }

    // Begin de loop om alles uit te lezen
    for (i = 0; i < loops; i++) {
        line = 0;
        clipsStanding[ i ].x = i * width;
        clipsStanding[ i ].y = line;
        clipsStanding[ i ].w = width;
        clipsStanding[ i ].h = height;

        line++;
        clipsWalkingRight[ i ].x = i * width;
        clipsWalkingRight[ i ].y = line * height;
        clipsWalkingRight[ i ].w = width;
        clipsWalkingRight[ i ].h = height;

        line++;
        clipsWalkingLeft[ i ].x = i * width;
        clipsWalkingLeft[ i ].y = line * height;
        clipsWalkingLeft[ i ].w = width;
        clipsWalkingLeft[ i ].h = height;

        line++;
        clipsWalkingDown[ i ].x = i * width;
        clipsWalkingDown[ i ].y = line * height;
        clipsWalkingDown[ i ].w = width;
        clipsWalkingDown[ i ].h = height;

        line++;
        clipsWalkingUp[ i ].x = i * width;
        clipsWalkingUp[ i ].y = line * height;
        clipsWalkingUp[ i ].w = width;
        clipsWalkingUp[ i ].h = height;
    }

}

void BaseClass::init_sounds(Mix_Chunk* snd1, Mix_Chunk* snd2, Mix_Chunk* snd3) {
    if (snd1 != NULL)
        sound1 = snd1;
    if (snd2 != NULL)
        sound2 = snd2;
    if (snd3 != NULL)
        sound3 = snd3;
}

void BaseClass::play_sound(int index) {
    if (index < 1)
        index = 1;
    if (index > 3)
        index = 3;

    switch (index) {
        case( 1): if (sound1) Mix_PlayChannel(-1, sound1, 0);
            break;
        case( 2): if (sound2) Mix_PlayChannel(-1, sound2, 0);
            break;
        case( 3): if (sound3) Mix_PlayChannel(-1, sound3, 0);
            break;
    }
}


// Klasse Character
//

// Constructor, vul de collisionbox met de juiste waarden

Character::Character(TileManager* map, int screenWidth, int screenHeight) :
BaseClass(CHARACTER) {
    box->x = 20;
    box->y = 20;
    box->w = CHARACTER_WIDTH;
    box->h = CHARACTER_HEIGHT;
    xVel = 0;
    yVel = 0;

    H_MAP_WIDTH = map->get_map_width();
    H_MAP_HEIGHT = map->get_map_height();
    H_SCREEN_WIDTH = screenWidth;
    H_SCREEN_HEIGHT = screenHeight;

    actionFrame = 0;

    lastState = FACE_DOWN;
    state = FACE_DOWN;
}

Character::~Character() {
}

SDL_Rect& Character::get_box() {
    return ( *box);
}

SDL_Rect* Character::get_box(int) {
    return box;
}

void Character::handle_events(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case( SDLK_w): yVel -= Y_SPEED;
                break;
            case( SDLK_s): yVel += Y_SPEED;
                break;
            case( SDLK_a): xVel -= X_SPEED;
                break;
            case( SDLK_d): xVel += X_SPEED;
                break;
            default: break;
        }
        handledMovement = true;
    }
    if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
            case( SDLK_w): yVel += Y_SPEED;
                break;
            case( SDLK_s): yVel -= Y_SPEED;
                break;
            case( SDLK_a): xVel += Y_SPEED;
                break;
            case( SDLK_d): xVel -= X_SPEED;
                break;
            default: break;
        }
        handledMovement = true;
    }
}

void Character::set_camera(Camera* cam) {
    Ssint xP = (box->x + box->w / 2) - H_SCREEN_WIDTH / 2;
    Ssint yP = (box->y + box->h / 2) - H_SCREEN_HEIGHT / 2;

    cam->set_camera(xP, yP);
}

void Character::calc_state() {
    short int xSpeed = xVel;
    short int ySpeed = yVel;

    lastState = state;

    if (xSpeed < 0 && make_number_positive(ySpeed) < make_number_positive(xSpeed))
        state = WALK_LEFT;
    else if (xSpeed > 0 && make_number_positive(ySpeed) < make_number_positive(xSpeed))
        state = WALK_RIGHT;
    else if (ySpeed < 0 && make_number_positive(xSpeed) < make_number_positive(ySpeed))
        state = WALK_UP;
    else if (ySpeed > 0 && make_number_positive(xSpeed) < make_number_positive(ySpeed))
        state = WALK_DOWN;
    else if (xSpeed == 0 && ySpeed == 0) {
        switch (lastState) {
            case( WALK_UP): state = FACE_UP;
                break;
            case( WALK_DOWN): state = FACE_DOWN;
                break;
            case( WALK_LEFT): state = FACE_LEFT;
                break;
            case( WALK_RIGHT): state = FACE_RIGHT;
                break;
            default: state = lastState;
                break;
        }
    } else
        state = lastState;
}

void Character::move() {

    box->x += xVel;

    if ((box->x < 0) || (box->x + box->w > H_MAP_WIDTH))
        box->x -= xVel;

    box->y += yVel;

    if ((box->y < 0) || (box->y + box->h > H_MAP_HEIGHT))
        box->y -= yVel;

    calc_state();

}

void Character::show(SDL_Surface* screen) {
    frame++;
    if (frame % 3 == 0)
        actionFrame++;
    SDL_Rect* clip = NULL;

    if (frame >= 1000)
        frame = 0;
    if (actionFrame >= 4)
        actionFrame = 0;

    switch (state) {
        case( FACE_DOWN): clip = &clipsStanding[ 0 ];
            break;
        case( FACE_UP): clip = &clipsStanding[ 1 ];
            break;
        case( FACE_RIGHT): clip = &clipsStanding[ 2 ];
            break;
        case( FACE_LEFT): clip = &clipsStanding[ 3 ];
            break;
        case( WALK_DOWN): clip = &clipsWalkingDown[ actionFrame ];
            break;
        case( WALK_UP): clip = &clipsWalkingUp[ actionFrame ];
            break;
        case( WALK_LEFT): clip = &clipsWalkingLeft[ actionFrame ];
            break;
        case( WALK_RIGHT): clip = &clipsWalkingRight[ actionFrame ];
            break;
        default: clip = &clipsStanding[ 0 ];
            break;
    }

    SDL_BlitSurface(clipSheet, clip, screen, box);

    handledMovement = false;
}

void Character::show(SDL_Surface* screen, Camera* cam) {
    SDL_Rect camRect = cam->get_view();
    SDL_Rect offset;

    frame++;
    if (frame % 2 == 0)
        actionFrame++;
    SDL_Rect* clip = NULL;

    if (frame >= 1000)
        frame = 0;
    if (actionFrame >= 4)
        actionFrame = 0;

    switch (state) {
        case( FACE_DOWN): clip = &clipsStanding[ 0 ];
            break;
        case( FACE_UP): clip = &clipsStanding[ 1 ];
            break;
        case( FACE_RIGHT): clip = &clipsStanding[ 2 ];
            break;
        case( FACE_LEFT): clip = &clipsStanding[ 3 ];
            break;
        case( WALK_DOWN): clip = &clipsWalkingDown[ actionFrame ];
            break;
        case( WALK_UP): clip = &clipsWalkingUp[ actionFrame ];
            break;
        case( WALK_LEFT): clip = &clipsWalkingLeft[ actionFrame ];
            break;
        case( WALK_RIGHT): clip = &clipsWalkingRight[ actionFrame ];
            break;
        default: clip = &clipsStanding[ 0 ];
            break;
    }

    offset.x = box->x - camRect.x;
    offset.y = box->y - camRect.y;

    SDL_BlitSurface(clipSheet, clip, screen, &offset);
}

void Character::reset() {
    xVel = 0;
    yVel = 0;
}



//
//
// Klasse Dog
//
//

// Constructor

Dog::Dog(Character& character) :
BaseClass(DOG) {
    box->x = (character.get_box().x + CHARACTER_WIDTH) + 5;
    box->y = (character.get_box().y + CHARACTER_HEIGHT) + 5;

    box->w = DOG_VERTICAL_WIDTH;
    box->h = DOG_VERTICAL_HEIGHT;

    lastState = FACE_DOWN;
    state = FACE_DOWN;

    actionFrame = 0;

    xVel = 0;
    yVel = 0;

    set_following(1);
}

// Destructor, doet niets.

Dog::~Dog() {
}

void Dog::handle_events(SDL_Event& event, Character& character, JRoute& route) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_RIGHT) {
            if ((stopped == true) && (route.route_loaded() == false))
                set_following(1);
            else if ((stopped == false) && (route.route_loaded() == false))
                set_following(3);
            play_sound(2);
        }
    }
}

void Dog::set_camera(Camera* cam) {
    int xP = (box->x + box->w / 2) - H_SCREEN_WIDTH / 2;
    int yP = (box->y + box->h / 2) - H_SCREEN_HEIGHT / 2;

    cam->set_camera(xP, yP);
}

void Dog::calc_state() {
    short int xSpeed = xVel;
    short int ySpeed = yVel;

    lastState = state;

    if (xSpeed < 0 && make_number_positive(ySpeed) < make_number_positive(xSpeed))
        state = WALK_LEFT;
    else if (xSpeed > 0 && make_number_positive(ySpeed) < make_number_positive(xSpeed))
        state = WALK_RIGHT;
    else if (ySpeed < 0 && make_number_positive(xSpeed) < make_number_positive(ySpeed))
        state = WALK_UP;
    else if (ySpeed > 0 && make_number_positive(xSpeed) < make_number_positive(ySpeed))
        state = WALK_DOWN;
    else if (xSpeed == 0 && ySpeed == 0) {
        switch (lastState) {
            case( WALK_UP): state = FACE_UP;
                break;
            case( WALK_DOWN): state = FACE_DOWN;
                break;
            case( WALK_LEFT): state = FACE_LEFT;
                break;
            case( WALK_RIGHT): state = FACE_RIGHT;
                break;
            default: state = lastState;
                break;
        }
    } else
        state = lastState;


    int width = 0;
    int height = 0;
    switch (state) {
        case( FACE_LEFT | WALK_LEFT | FACE_RIGHT | WALK_RIGHT):
        {
            width = DOG_HORIZONTAL_WIDTH;
            height = DOG_HORIZONTAL_HEIGHT;
            break;
        }
        case( FACE_UP | WALK_UP | FACE_DOWN | WALK_DOWN):
        {
            width = DOG_VERTICAL_WIDTH;
            height = DOG_VERTICAL_HEIGHT;
            break;
        }
    }

    box->w = width;
    box->h = height;

}

// Deze functie past de snelheden aan van de hond ten opzichte van 2 coördinaten. Wanneer de coördinaten
// dicht bij elkaar liggen geeft de functie true terug. De afstand om true terug te geven is de wp_distance
// welke een default waarde inneemt

bool Dog::set_velocity(int xA, int yA, int xB, int yB, int wp_distance) {
    /* 
            Kijk eerst of de coördinaten dicht bij elkaar in de buurt liggen aan de hand van de wp_distance
     */
    if (wp_distance > 200)
        wp_distance = 200;
    if (wp_distance < 1)
        wp_distance = 1;

    if (distance(xA, yA, xB, yB) <= wp_distance) {
        return true;
    }

    /* 
            De afstand is dus nog te groot en de waypoint moet nog opgezocht worden
            Eerst op de x as en daarna op de y as
     */
    // Kijk eerst of de nieuwe coördinaten loodrecht op lijn liggen met de huidige coördinaten
    if (xA == xB) // op de x as
    {
        if (yA > yB)
            xVel = 0 + (X_SPEED * 2);
        else if (yA < yB)
            xVel = 0 - (X_SPEED * 2);
        // De lijn is loodrecht op de x-as, de snelheid is dus ingesteld en de waypoint is nog niet bereikt. return false
        return false;
    }
    if (yA == yB) // op de y as
    {
        if (xA > xB)
            yVel = 0 + (Y_SPEED * 2);
        else if (xA < xB)
            yVel = 0 - (Y_SPEED * 2);
        // De lijn is loodrecht op de y-as, de snelheid is dus ingesteld en de waypoint is nog niet bereikt. return false
        return false;
    }

    /* 
            Nu we hier aangekomen zijn liggen de punten niet loodrecht langs elkaar en moet de hoek dus berekend worden om de 
            x en y snelheden in te stellen. Dit gaat om de beurt beginnend in de linkerbovenhoek, dan rechtsboven, linksonder 
            en uiteindelijk rechtsonder.
     */
    if ((xA > xB) && (yA > yB)) // Als het doel zich linksboven bevind
    {
        float xDiff = xA - xB;
        float yDiff = yA - yB;
        float totalDiff = xDiff + yDiff;
        float xPerc = (xDiff * 100) / totalDiff;
        float yPerc = (yDiff * 100) / totalDiff;
        xVel = 0 - float( (float( X_SPEED * 2) / 100) * xPerc);
        yVel = 0 - float( (float( Y_SPEED * 2) / 100) * yPerc);
        return false; // Het doel is nog niet bereikt en de snelheid is ingesteld dus return false
    } else if ((xA < xB) && (yA > yB)) // Als het doel zich rechtsboven bevind
    {
        float xDiff = xB - xA;
        float yDiff = yA - yB;
        float totalDiff = xDiff + yDiff;
        float xPerc = (xDiff * 100) / totalDiff;
        float yPerc = (yDiff * 100) / totalDiff;
        xVel = 0 + float( (float( X_SPEED * 2) / 100) * xPerc);
        yVel = 0 - float( (float( Y_SPEED + 5) / 100) * yPerc);
        return false; // Het doel is nog niet bereikt en de snelheid is ingesteld dus return false
    } else if ((xA > xB) && (yA < yB)) // Als het doel zich linksonder bevind
    {
        float xDiff = xA - xB;
        float yDiff = yB - yA;
        float totalDiff = xDiff + yDiff;
        float xPerc = (xDiff * 100) / totalDiff;
        float yPerc = (yDiff * 100) / totalDiff;
        xVel = 0 - float( (float( X_SPEED * 2) / 100) * xPerc);
        yVel = 0 + float( (float( Y_SPEED * 2) / 100) * yPerc);
        return false; // Het doel is nog niet bereikt en de snelheid is ingesteld dus return false
    } else if ((xA < xB) && (yA < yB)) // Als het doel zich rechtsonder bevind
    {
        float xDiff = xB - xA;
        float yDiff = yB - yA;
        float totalDiff = xDiff + yDiff;
        float xPerc = (xDiff * 100) / totalDiff;
        float yPerc = (yDiff * 100) / totalDiff;
        xVel = 0 + float( (float( X_SPEED * 2) / 100) * xPerc);
        yVel = 0 + float( (float( Y_SPEED * 2) / 100) * yPerc);
        return false; // Het doel is nog niet bereikt en de snelheid is ingesteld dus return false
    }
    // Mocht de functie het einde bereiken zonder getriggerd te zijn return dan false	
    return false;
}

void Dog::follow_character(Character* character) {
    set_following(1);
    if (set_velocity(box->x + (box->w / 2),
            box->y + (box->h / 2),
            character->get_box().x + (character->get_box().w / 2),
            character->get_box().y + (character->get_box().h / 2),
            WAYPOINT_REACHED * 3)) {
        xVel = 0;
        yVel = 0;
    }
}

void Dog::follow_route(JRoute* route) {
    set_following(2);
    if (set_velocity(box->x + (box->w / 2), box->y + (box->h / 2),
            route->get_current_waypoint().x + (route->get_current_waypoint().w / 2),
            route->get_current_waypoint().y + (route->get_current_waypoint().h / 2))) {
        route->set_next_waypoint();
        if (frame % 4 == 1) play_sound(1);
    }
}

void Dog::do_think(Character* character, JRoute* route) {
    if (!stopped) {
        if (route->route_loaded() || followRoute) {
            if (route->route_loaded() && followRoute)
                follow_route(route);
            else if (!route->route_loaded() && followRoute)
                set_following(3);
            else if (route->route_loaded() && !followRoute)
                follow_route(route);
        } else
            follow_character(character);
    } else {
        if (route->route_loaded() && !followRoute)
            follow_route(route);
    }
}

void Dog::move() {
    box->x += xVel;
    if ((box->x < (0 - box->w / 2)) || (box->x + box->w > H_MAP_WIDTH)) {
        box->x -= xVel;
    }

    box->y += yVel;
    if ((box->y < (0 - box->h / 2)) || (box->y + box->w > H_MAP_HEIGHT)) {
        box->y -= yVel;
    }
    calc_state();
}

void Dog::show(SDL_Surface* screen) {
    frame++;
    if (frame % 2 == 0)
        actionFrame++;

    SDL_Rect* clip = NULL;

    if (frame >= 1000)
        frame = 0;
    if (actionFrame >= 4 || actionFrame < 0)
        actionFrame = 0;

    switch (state) {
        case( FACE_DOWN): clip = &clipsStanding[ 0 ];
            break;
        case( FACE_UP): clip = &clipsStanding[ 1 ];
            break;
        case( FACE_LEFT): clip = &clipsStanding[ 2 ];
            break;
        case( FACE_RIGHT): clip = &clipsStanding[ 3 ];
            break;
        case( WALK_DOWN): clip = &clipsWalkingDown[ actionFrame ];
            break;
        case( WALK_UP): clip = &clipsWalkingUp[ actionFrame ];
            break;
        case( WALK_LEFT): clip = &clipsWalkingLeft[ actionFrame ];
            break;
        case( WALK_RIGHT): clip = &clipsWalkingRight[ actionFrame ];
            break;
        default: clip = &clipsStanding[ 0 ];
            break;
    }

    SDL_BlitSurface(clipSheet, clip, screen, box);

}

void Dog::reset() {
    xVel = 0;
    yVel = 0;
}

void Dog::toggle_stopped() {
    if (!stopped)
        set_stopped();
    else
        clear_following();
}

void Dog::set_stopped() {
    stopped = true;
    followCharacter = false;
    followRoute = false;
    xVel = 0;
    yVel = 0;
}

void Dog::set_following(int stance) {
    if ((stance < 1) || (stance > 3))
        return;

    switch (stance) {
        case( 1): stopped = false;
            followCharacter = true;
            followRoute = false;
            break;
        case( 2): stopped = false;
            followCharacter = false;
            followRoute = true;
            break;
        case( 3): set_stopped();
            break;
    }
}

void Dog::clear_following() {
    stopped = false;
    followCharacter = false;
    followRoute = false;
}

SDL_Rect& Dog::get_box() {
    return *box;
}

SDL_Rect* Dog::get_box(int flag) {
    return box;
}


// Sheep klasse
//

// Constructor en destructor

Sheep::Sheep() :
BaseClass(SHEEP) {
    box->x = 0;
    box->y = 0;

    box->w = SHEEP_HORIZONTAL_WIDTH;
    box->h = SHEEP_HORIZONTAL_HEIGHT;

    lastState = STATE(rand() % 2);
    state = lastState;

    itsNumber = 0;

    inner = (rand() % 10) + 1;
    frame = 0;
    actionFrame = 0;
    isFinished = false;
    isScarsed = false;

    xVel = 0;
    yVel = 0;

    loadedOK = true;
}

Sheep::Sheep(int xPos, int yPos) :
BaseClass(SHEEP) {
    box->x = xPos;
    box->y = yPos;

    box->w = SHEEP_HORIZONTAL_WIDTH;
    box->h = SHEEP_HORIZONTAL_HEIGHT;

    lastState = STATE(rand() % 2);
    state = lastState;

    itsNumber = 0;

    inner = (rand() % 10) + 1;
    frame = 0;
    actionFrame = 0;
    isFinished = false;
    isScarsed = false;


    xVel = 0;
    yVel = 0;

    loadedOK = true;
}

Sheep::~Sheep() {
}

void Sheep::set_start_square(int xPos, int yPos, int width) {
    box->x = (rand() % width) + xPos;
    box->y = (rand() % width) + yPos;
}

void Sheep::set_its_number(int number) {
    itsNumber = number;
    frame = rand() % 1000;
}

int Sheep::get_its_number() const {
    return itsNumber;
}

void Sheep::calc_state() {
    short int xSpeed = xVel;
    short int ySpeed = yVel;

    lastState = state;

    if (xSpeed < 0 && make_number_positive(ySpeed) < make_number_positive(xSpeed))
        state = WALK_LEFT;
    else if (xSpeed > 0 && make_number_positive(ySpeed) < make_number_positive(xSpeed))
        state = WALK_RIGHT;
    else if (ySpeed < 0 && make_number_positive(xSpeed) < make_number_positive(ySpeed))
        state = WALK_UP;
    else if (ySpeed > 0 && make_number_positive(xSpeed) < make_number_positive(ySpeed))
        state = WALK_DOWN;
    else if (xSpeed == 0 && ySpeed == 0) {
        switch (lastState) {
            case( WALK_UP): state = FACE_UP;
                break;
            case( WALK_DOWN): state = FACE_DOWN;
                break;
            case( WALK_LEFT): state = FACE_LEFT;
                break;
            case( WALK_RIGHT): state = FACE_RIGHT;
                break;
            default: state = lastState;
                break;
        }
    } else
        state = lastState;


    int width = 0;
    int height = 0;
    switch (state) {
        case( FACE_LEFT | WALK_LEFT | FACE_RIGHT | WALK_RIGHT):
        {
            width = SHEEP_HORIZONTAL_WIDTH;
            height = SHEEP_HORIZONTAL_HEIGHT;
            break;
        }
        case( FACE_UP | WALK_UP | FACE_DOWN | WALK_DOWN):
        {
            width = SHEEP_VERTICAL_WIDTH;
            height = SHEEP_VERTICAL_HEIGHT;
            break;
        }
    }

    box->w = width;
    box->h = height;

}

SDL_Rect& Sheep::get_box() {
    return *box;
}

SDL_Rect* Sheep::get_box(int flag) {
    return box;
}

int Sheep::get_xVel()const {
    return xVel;
}

int Sheep::get_yVel()const {
    return yVel;
}

void Sheep::handle_events(SDL_Event& event, Character& character) {

}

void Sheep::do_think(Character& character, Dog& hond) {

}

bool Sheep::calc_finish(SDL_Rect& finish) {
    // Als het schaap gedeeltelijk binnen de finish is
    if (calc_collision(box, &finish)) {
        isFinished = true;
        play_sound(3);
    }

    return isFinished;
}

void Sheep::move(Tile& finish) {

    box->x += xVel;
    if ((box->x < 0) || (box->x + box->w > H_MAP_WIDTH)) {
        box->x -= (xVel); //* 2 );

    }

    box->y += yVel;
    if ((box->y < 0) || (box->y + box->h > H_MAP_HEIGHT)) {
        box->y -= (yVel); //* 2 );
    }
    if (calc_finish(finish.get_inner())) {
        xVel = 0;
        yVel = 0;
    }

    calc_state();

    if (frame % 650 == 0) {
        if (inner % 2 == 0)
            play_sound(1);
        else
            play_sound(2);
    }

    handledMovement = false;
}

void Sheep::set_state(STATE newstate) {
    lastState = state;
    state = newstate;
}

void Sheep::move_random() {
    if (frame < framesMovingRandom)
        framesMovingRandom = frame - 1;
    if ((frame - framesMovingRandom) > 10) {
        isMovingRandom = true;
        framesMovingRandom = 0;

        int xV = rand() % ((X_SPEED / 3) * 2);
        int yV = rand() % ((Y_SPEED / 3) * 2);

        if (rand() % 2)
            xV -= (xV * 2);
        if (rand() % 2)
            yV -= (yV * 2);

        xVel = xV;
        yVel = yV;
    }
}

// Behandel de random movement van het schaap.
// Met de vlag wordt actie bepaald. false om het uit te zetten en true om een random movement in gang te zetten en op te volgen

void Sheep::random_movement(SheepManager* manager, bool flag) {
    if (!flag) {
        isMovingRandom = false;
        framesMovingRandom = frame;
    } else if (flag) {
        if (isMovingRandom == false) {
            SDL_Rect leader = manager->find_leader(this);
            if (leader.w == 0) {
                move_random();
            } else {
                goto_leader(leader);
            }
        } else if (isMovingRandom == true) {
            if (framesMovingRandom == 8) {
                isMovingRandom = false;
                xVel = 0;
                yVel = 0;
                framesMovingRandom = frame;
            } else {
                xVel = xVel;
                yVel = yVel;
                framesMovingRandom++;
            }
        }
    }
}

bool Sheep::check_values() {
    std::cout << "Beginnen met het checken van de geladen clips van het actieve schaap...\n";

    const short int NR_CLIPS = 4;

    std::cout << "Doorlopen van stilsta clips\n";
    for (int i = 0; i < NR_CLIPS; i++) {
        std::cout << "Adres van de clip " << &clipsStanding[ i ] << std::endl;
        std::cout << "Clip x: " << clipsStanding[ i ].x << std::endl;
    }
    std::cout << "Doorlopen animatie omhoog clips\n";
    for (int j = 0; j < NR_CLIPS; j++) {
        std::cout << "Adres van de clip " << &clipsWalkingUp[ j ] << std::endl;
        std::cout << "Clip x: " << clipsWalkingUp[ j ].x << std::endl;
    }
    std::cout << "Doorlopen animatie omlaag clips\n";
    for (int k = 0; k < NR_CLIPS; k++) {
        std::cout << "Adres van de clip " << &clipsWalkingDown[ k ] << std::endl;
        std::cout << "Clip x: " << clipsWalkingDown[ k ].x << std::endl;
    }
    std::cout << "Doorlopen animatie links clips\n";
    for (int t = 0; t < NR_CLIPS; t++) {
        std::cout << "Adres van de clip " << &clipsWalkingLeft[ t ] << std::endl;
        std::cout << "Clip x: " << clipsWalkingLeft[ t ].x << std::endl;
    }
    std::cout << "Doorlopen animatie rechts clips\n";
    for (int f = 0; f < NR_CLIPS; f++) {
        std::cout << "Adres van de clip " << &clipsWalkingRight[ f ] << std::endl;
        std::cout << "Clip x: " << clipsWalkingRight[ f ].x << std::endl;
    }

    std::cout << "Alle clips doorlopen...\n";
    return true;
}

void Sheep::show(SDL_Surface* screen) {
    frame++;
    if (frame % 4 == 0)
        actionFrame++;

    isScarsed = false;

    SDL_Rect* clip = NULL;

    if (frame >= 1000)
        frame = 0;
    if (actionFrame >= 4 || actionFrame < 0)
        actionFrame = 0;

    if (!isFinished) {
        switch (state) {
            case( FACE_DOWN): clip = &clipsStanding[ 0 ];
                break;
            case( FACE_UP): clip = &clipsStanding[ 1 ];
                break;
            case( FACE_LEFT): clip = &clipsStanding[ 2 ];
                break;
            case( FACE_RIGHT): clip = &clipsStanding[ 3 ];
                break;
            case( WALK_DOWN): clip = &clipsWalkingDown[ actionFrame ];
                break;
            case( WALK_UP): clip = &clipsWalkingUp[ actionFrame ];
                break;
            case( WALK_LEFT): clip = &clipsWalkingLeft[ actionFrame ];
                break;
            case( WALK_RIGHT): clip = &clipsWalkingRight[ actionFrame ];
                break;
            default: clip = &clipsStanding[ 0 ];
                break;
        }

        if (clip) {
            SDL_BlitSurface(clipSheet, clip, screen, box);
        }
    }

}

void Sheep::goto_leader(SDL_Rect& leader) {
    int xA = leader.x + (leader.w / 2);
    int yA = leader.y + (leader.h / 2);
    int xB = box->x + (box->w / 2);
    int yB = box->y + (box->h / 2);

    int SX_SPEED = X_SPEED * 1.5;
    int SY_SPEED = Y_SPEED * 1.5;

    // Mits de leider binnen een redelijke afstand is zet dan random movement aan en exit de functie
    if (distance(xA, yA, xB, yB) < ((box->w + box->h) * 3)) {
        move_random();
        return;
    }

    // Kijk eerst of de nieuwe coördinaten loodrecht op lijn liggen met de huidige coördinaten
    if (xA == xB) // op de x as
    {
        if (yA > yB)
            xVel = 0 + (SX_SPEED);
        else if (yA < yB)
            xVel = 0 - (SX_SPEED);
        // De lijn is loodrecht op de x-as, de snelheid is dus ingesteld en de waypoint is nog niet bereikt. return false
        return;
    }
    if (yA == yB) // op de y as
    {
        if (xA > xB)
            yVel = 0 + (SY_SPEED);
        else if (xA < xB)
            yVel = 0 - (SY_SPEED);
        // De lijn is loodrecht op de y-as, de snelheid is dus ingesteld en de waypoint is nog niet bereikt. return false
        return;
    }

    /* 
            Nu we hier aangekomen zijn liggen de punten niet loodrecht langs elkaar en moet de hoek dus berekend worden om de 
            x en y snelheden in te stellen. 
     */
    if ((xA > xB) && (yA > yB)) // Als het doel zich rechtsonder bevind
    {
        float xDiff = xA - xB;
        float yDiff = yA - yB;
        float totalDiff = xDiff + yDiff;
        float xPerc = (xDiff * 100) / totalDiff;
        float yPerc = (yDiff * 100) / totalDiff;
        xVel = 0 + float( (float( SX_SPEED) / 100) * xPerc);
        yVel = 0 + float( (float( SY_SPEED) / 100) * yPerc);
        return;
    } else if ((xA < xB) && (yA > yB)) // Als het doel zich linksonder bevind
    {
        float xDiff = xB - xA;
        float yDiff = yA - yB;
        float totalDiff = xDiff + yDiff;
        float xPerc = (xDiff * 100) / totalDiff;
        float yPerc = (yDiff * 100) / totalDiff;
        xVel = 0 - float( (float( SX_SPEED) / 100) * xPerc);
        yVel = 0 + float( (float( SY_SPEED) / 100) * yPerc);
        return;
    } else if ((xA > xB) && (yA < yB)) // Als het doel zich rechtsboven bevind
    {
        float xDiff = xA - xB;
        float yDiff = yB - yA;
        float totalDiff = xDiff + yDiff;
        float xPerc = (xDiff * 100) / totalDiff;
        float yPerc = (yDiff * 100) / totalDiff;
        xVel = 0 + float( (float( SX_SPEED) / 100) * xPerc);
        yVel = 0 + float( (float( SY_SPEED) / 100) * yPerc);
        return;
    } else if ((xA < xB) && (yA < yB)) // Als het doel zich linksboven bevind
    {
        float xDiff = xB - xA;
        float yDiff = yB - yA;
        float totalDiff = xDiff + yDiff;
        float xPerc = (xDiff * 100) / totalDiff;
        float yPerc = (yDiff * 100) / totalDiff;
        xVel = 0 - float( (float( SX_SPEED) / 100) * xPerc);
        yVel = 0 - float( (float( SY_SPEED) / 100) * yPerc);
        return;
    }
}

bool Sheep::is_finished()const {
    return isFinished;
}

void Sheep::set_xVel(int vel) {
    xVel = vel;
}

void Sheep::set_yVel(int vel) {
    yVel = vel;
}

int Sheep::get_inner()const {
    return inner;
}



// SheepManager klasse
//

SheepManager::SheepManager(Character* charac, Dog* dg, Tile* fnsh, SDL_Surface* itsgraphs, Mix_Chunk* snd1, Mix_Chunk* snd2, Mix_Chunk* snd3, int qty) {
    if (qty <= 1)
        qty = 2;

    itsSheep = new Sheep[ qty ];
    if (itsSheep == NULL) {
        printf("Throwing error from SheepManager constructor");
        throw ( ERR::Out_Of_Memory());
    }

    int startW = 220;
    int randXstart = rand() % (H_MAP_WIDTH - startW);
    int randYstart = rand() % (H_MAP_HEIGHT - startW);

    for (int i = 0; i < qty; i++) {
        itsSheep[ i ].set_its_number(i + 1);
        itsSheep[ i ].init_graphics(itsgraphs, 60, 75, 4);
        itsSheep[ i ].init_sounds(snd1, snd2, snd3);
        itsSheep[ i ].set_start_square(randXstart, randYstart, startW);
    }

    character = charac;
    dog = dg;
    finish = fnsh;

    scoreCounter = 0;
    score = 0;

    qtySheep = qty;

    itsTimer.start();
    scoreTimer.start();

    std::cout << "SheepManager initiated.\n";
}

SheepManager::~SheepManager() {
    delete [] itsSheep;
    std::cout << "SheepManager destroyed.\n";
}

void SheepManager::do_think() {

    for (int i = 0; i < qtySheep; i++) {
        itsSheep[ i ].handledMovement = false;

        if (set_velocity(&itsSheep[ i ], character->get_box(1), true)) {
            itsSheep[ i ].set_scarsed();
            itsSheep[ i ].play_sound(1);
        }
        if (set_velocity(&itsSheep[ i ], dog->get_box(1), true)) {
            itsSheep[ i ].set_scarsed();
            itsSheep[ i ].play_sound(2);
        }

        if (!(itsSheep[ i ].handledMovement))
            itsSheep[ i ].random_movement(this, true);

    }
}


// invoer parameters waarden:
//							0 - 
//							1 - 
//							2 - 

/**
 * Verkrijg het aantal shoarmarollen in de game
 * 
 * @param flag integer
 * @return 
 */
int SheepManager::get_sheep_count(int flag)const {
    
    switch (flag) {

        // Totaal aantal schapen beheerd door de SheepManager
        case 0:
            return qtySheep;
            break;

        // Totaal aantal schapen welke nog in het spel zijn
        case 1:
            int count = 0;
            for (int i = 0; i < qtySheep; i++) {
                if (!(itsSheep[ i ].is_finished())) {
                    count++;
                }
            }
            return count;
            break;
     
        // Totaal aantal gefinishte schapen
        case 2:
            int count = 0;
            for (int j = 0; j < qtySheep; j++) {
                if (itsSheep[ j ].is_finished()) {
                    count++;
                }
            }
            return count;
            break;
    }
    
    return 0;
}

// Deze functie scant de omgeving naar een belangrijker schaap
SDL_Rect SheepManager::find_leader(Sheep* sheep) {
    unsigned int max_distance = LEADER_DISTANCE;

    if (sheep->get_inner() < 7) {
        for (int i = 0; i < qtySheep; i++) {
            if (itsSheep[ i ].get_its_number() != sheep->get_its_number()) {
                if ((distance(sheep->get_box().x, sheep->get_box().y, itsSheep[ i ].get_box().x, itsSheep[ i ].get_box().y) < max_distance)
                        ) {
                    if (itsSheep[ i ].get_inner() > sheep->get_inner()) {
                        SDL_Rect temp = *itsSheep[ i ].get_box(1);
                        return temp;
                    }
                }
            }
        }
    }

    SDL_Rect temp2 = {0, 0, 0, 0};
    return temp2;
}

bool SheepManager::calc_collision(SDL_Rect* sheep, SDL_Rect* target) {
    if ((target->y + target->h) < sheep->y ||
            target->y > (sheep->y + sheep->h) ||
            (target->x + target->w) < sheep->x ||
            target->x > (sheep->x + sheep->w))
        return false;

    return true;
}

unsigned int SheepManager::check_collision(Sheep* sheep, BaseClass* targetCHAR) {
    // Deel de collision boxen van de invoerparameters op in 4 en ga per kant checken of deze geraakt word
    SDL_Rect* sheepBox = new SDL_Rect[ 4 ];
    if (!sheepBox) {
        printf("Throwing error from SheepManager::check_collision\n");
        throw ( ERR::Out_Of_Memory());
    }


    int vlag = 1;
    SDL_Rect* target = targetCHAR->get_box(vlag); // Vlag 1 om aan te duiden dat ik de pointer wil. Niets is de referentie

    /*
    Object x,y = 20,21
    0 x,y = x,y
    1 x,y = x + b/2, y
    2 x,y = x,y + h/2
    3 x,y = x + b/2,y + h/2
     */

    sheepBox[ 0 ].x = sheep->get_box().x;
    sheepBox[ 0 ].y = sheep->get_box().y;
    sheepBox[ 0 ].w = sheep->get_box().w / 2;
    sheepBox[ 0 ].h = sheep->get_box().h / 2;

    sheepBox[ 1 ].x = sheep->get_box().x + (sheep->get_box().w / 2);
    sheepBox[ 1 ].y = sheep->get_box().y;
    sheepBox[ 1 ].w = sheep->get_box().w / 2;
    sheepBox[ 1 ].h = sheep->get_box().h / 2;

    sheepBox[ 2 ].x = sheep->get_box().x;
    sheepBox[ 2 ].y = sheep->get_box().y + (sheep->get_box().h / 2);
    sheepBox[ 2 ].w = sheep->get_box().w / 2;
    sheepBox[ 2 ].h = sheep->get_box().h / 2;

    sheepBox[ 3 ].x = sheep->get_box().x + (sheep->get_box().w / 2);
    sheepBox[ 3 ].y = sheep->get_box().y + (sheep->get_box().h / 2);
    sheepBox[ 3 ].w = sheep->get_box().w / 2;
    sheepBox[ 3 ].h = sheep->get_box().h / 2;

    // check of het volledige schaap botst
    if (calc_collision(&sheepBox[ 0 ], target) && calc_collision(&sheepBox[ 1 ], target) &&
            calc_collision(&sheepBox[ 2 ], target) && calc_collision(&sheepBox[ 3 ], target)) {
        delete [] sheepBox;
        return COLLISION_FULL;
    }
        // check of er een botsing van boven is
    else if (calc_collision(&sheepBox[ 0 ], target) && calc_collision(&sheepBox[ 1 ], target)) {
        delete [] sheepBox;
        return COLLISION_TOP;
    }
        // check of er een botsing van beneden is
    else if (calc_collision(&sheepBox[ 2 ], target) && calc_collision(&sheepBox[ 3 ], target)) {
        delete [] sheepBox;
        return COLLISION_BOTTOM;
    }
        // check of er een botsing van links is
    else if (calc_collision(&sheepBox[ 0 ], target) && calc_collision(&sheepBox[ 2 ], target)) {
        delete [] sheepBox;
        return COLLISION_LEFT;
    }
        // check of er een botsing van rechts is
    else if (calc_collision(&sheepBox[ 1 ], target) && calc_collision(&sheepBox[ 3 ], target)) {
        delete [] sheepBox;
        return COLLISION_RIGHT;
    }
        // check of er een botsing van linksboven is
    else if (calc_collision(&sheepBox[ 0 ], target)) {
        delete [] sheepBox;
        return COLLISION_TOPLEFT;
    }
        // check of er een botsing van rechtsboven is
    else if (calc_collision(&sheepBox[ 1 ], target)) {
        delete [] sheepBox;
        return COLLISION_TOPRIGHT;
    }
        // check of er een botsing van linksonder is
    else if (calc_collision(&sheepBox[ 2 ], target)) {
        delete [] sheepBox;
        return COLLISION_BOTTOMLEFT;
    }
        // check of er een botsing van rechtsonder is
    else if (calc_collision(&sheepBox[ 3 ], target)) {
        delete [] sheepBox;
        return COLLISION_BOTTOMRIGHT;
    }

    // Als er uiteindelijk geen botsing is geweest geef dan NO_COLLISION terug
    delete [] sheepBox;
    return NO_COLLISION;

}

void SheepManager::handle_collision(Sheep* sheep, COLLIDEDIRECTION collision) {
    int x_vel = sheep->get_xVel();
    int y_vel = sheep->get_yVel();
    int w = 10;
    int h = 10;

    if ((sheep->frame % 10) == 0) {
        x_vel = 0;
        y_vel = 0;
    }

    //std::cout << collision.collision << std::endl;

    if (collision.collision == COLLISION_FULL && (sheep->frame % 10) == 0) {
        // Als er een volledige botsing is geef collision dan een willekeurige waarde behalve volledig en niets ga dan door met de switch
        collision.collision = (rand() % 7) + 1;
    }

    //std::cout << collision.collision << std::endl;

    switch (collision.collision) {
        case( COLLISION_TOP): y_vel += h;
            sheep->set_state(WALK_DOWN);
            break;
        case( COLLISION_BOTTOM): y_vel -= h;
            sheep->set_state(WALK_UP);
            break;
        case( COLLISION_LEFT): x_vel += w;
            sheep->set_state(WALK_RIGHT);
            break;
        case( COLLISION_RIGHT): x_vel -= w;
            sheep->set_state(WALK_LEFT);
            break;
        case( COLLISION_TOPRIGHT): y_vel += h;
            x_vel -= w;
            sheep->set_state(WALK_DOWN);
            break;
        case( COLLISION_TOPLEFT): y_vel += h;
            x_vel += w;
            sheep->set_state(WALK_DOWN);
            break;
        case( COLLISION_BOTTOMRIGHT): y_vel -= h;
            x_vel -= w;
            sheep->set_state(WALK_UP);
            break;
        case( COLLISION_BOTTOMLEFT): y_vel -= h;
            x_vel += w;
            sheep->set_state(WALK_UP);
            break;
        default: break;
    }

    sheep->handledMovement = true;
    sheep->random_movement(this, false);

    sheep->set_xVel(x_vel);
    sheep->set_yVel(y_vel);

}

bool SheepManager::set_velocity(Sheep* sheep, SDL_Rect* target, bool thread) {

    // Zet de waarden uit de SDL_Rects van het schaap en het doel om naar integers. Pak hiervoor het middenpunt
    int xA = sheep->get_box().x + (sheep->get_box().w / 2);
    int yA = sheep->get_box().y + (sheep->get_box().h / 2);
    int xB = target->x + (target->w / 2);
    int yB = target->y + (target->h / 2);

    // Bepaal de tolerantie afstand aan de hand van de bedreigingvlag
    int t_distance = (sheep->get_box().w / 2) + (target->w / 2);
    if (thread)
        t_distance = t_distance * 3;

    int SX_SPEED = X_SPEED * 1.5;
    int SY_SPEED = Y_SPEED * 1.5;

    // Bepaal of het doel dicht genoeg in de buurt ligt om van weg te lopen, keer anders als false terug. Alle code vanaf hier gaat ervan uit dat het 
    // schaap moet bewegen. Mocht dit niet gebeuren is de beweging van het schaap geregeld
    if (distance(xA, yA, xB, yB) > t_distance)
        return false;

    sheep->handledMovement = true;

    // Kijk eerst of de nieuwe coördinaten loodrecht op lijn liggen met de huidige coördinaten en de afstand is kleiner dan nodig
    if (xA == xB) // op de x as
    {
        if (yA > yB)
            sheep->set_xVel(0 - SX_SPEED);
        else if (yA < yB)
            sheep->set_xVel(0 + SX_SPEED);
        // De lijn is loodrecht op de x-as, de snelheid is dus ingesteld en het schaap heeft moeten bewegen, geef true terug
        return true;
    }
    if (yA == yB) // op de y as
    {
        if (xA > xB)
            sheep->set_yVel(0 - SY_SPEED);
        else if (xA < xB)
            sheep->set_yVel(0 + SY_SPEED);
        // De lijn is loodrecht op de y-as, de snelheid is dus ingesteld en het schaap heeft moeten bewegen, geef true terug
        return true;
    }

    /* 
            Nu we hier aangekomen zijn liggen de punten niet loodrecht langs elkaar en moet de hoek dus berekend worden om de 
            x en y snelheden in te stellen. Dit gaat om de beurt beginnend in de linkerbovenhoek, dan rechtsboven, linksonder 
            en uiteindelijk rechtsonder.
     */
    if ((xA > xB) && (yA > yB)) // Als het doel zich linksboven bevind
    {
        float xDiff = xA - xB;
        float yDiff = yA - yB;
        float totalDiff = xDiff + yDiff;
        float xPerc = (xDiff * 100) / totalDiff;
        float yPerc = (yDiff * 100) / totalDiff;
        sheep->set_xVel(0 + float( (float( SX_SPEED) / 100) * xPerc));
        sheep->set_yVel(0 + float( (float( SY_SPEED) / 100) * yPerc));
        return false; // Het doel is nog niet bereikt en de snelheid is ingesteld dus return false
    } else if ((xA < xB) && (yA > yB)) // Als het doel zich rechtsboven bevind
    {
        float xDiff = xB - xA;
        float yDiff = yA - yB;
        float totalDiff = xDiff + yDiff;
        float xPerc = (xDiff * 100) / totalDiff;
        float yPerc = (yDiff * 100) / totalDiff;
        sheep->set_xVel(0 - float( (float( SX_SPEED) / 100) * xPerc));
        sheep->set_yVel(0 + float( (float( SY_SPEED) / 100) * yPerc));
        return false; // Het doel is nog niet bereikt en de snelheid is ingesteld dus return false
    } else if ((xA > xB) && (yA < yB)) // Als het doel zich linksonder bevind
    {
        float xDiff = xA - xB;
        float yDiff = yB - yA;
        float totalDiff = xDiff + yDiff;
        float xPerc = (xDiff * 100) / totalDiff;
        float yPerc = (yDiff * 100) / totalDiff;
        sheep->set_xVel(0 + float( (float( SX_SPEED) / 100) * xPerc));
        sheep->set_yVel(0 - float( (float( SY_SPEED) / 100) * yPerc));
        return false; // Het doel is nog niet bereikt en de snelheid is ingesteld dus return false
    } else if ((xA < xB) && (yA < yB)) // Als het doel zich rechtsonder bevind
    {
        float xDiff = xB - xA;
        float yDiff = yB - yA;
        float totalDiff = xDiff + yDiff;
        float xPerc = (xDiff * 100) / totalDiff;
        float yPerc = (yDiff * 100) / totalDiff;
        sheep->set_xVel(0 - float( (float( SX_SPEED) / 100) * xPerc));
        sheep->set_yVel(0 - float( (float( SY_SPEED) / 100) * yPerc));
        return false; // Het doel is nog niet bereikt en de snelheid is ingesteld dus return false
    }
    // Mocht de functie het einde bereiken zonder getriggerd te zijn return dan false	
    return false;
}

void SheepManager::move(BaseClass* character, BaseClass* dog) {
    for (int i = 0; i < qtySheep; i++) {
        if (!itsSheep[ i ].is_finished()) {
            itsSheep[ i ].move(*finish);
        }
    }
}

// Functie geeft een scherm met de afmetingen van het spel terug met transparante achtergrond

SDL_Surface* SheepManager::draw_sheep() {
    if (itsSheep) {
        SDL_Surface* screen = SDL_CreateRGBSurface(SDL_HWSURFACE, H_MAP_WIDTH, H_MAP_HEIGHT, 32, 0, 0, 0, 0);
        if (screen == NULL) {
            printf("Throwing error from SheepManager::draw_sheep\n");
            throw ( ERR::Out_Of_Memory());
        }

        SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0x00, 0xFF, 0xFF));

        for (int i = 0; i < qtySheep; i++) {
            itsSheep[ i ].show(screen);
        }

        SDL_SetColorKey(screen, SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 0, 0xFF, 0xFF));

        return screen;
    }

    return NULL;
}

// Functie geeft de score weer op een Surface met voor de rest transparantie

SDL_Surface* SheepManager::draw_score(TTF_Font* font, Camera* cam) {
    //std::cout << "Reached SheepManager::draw_score\n";
    if (itsSheep) {
        SDL_Surface* screen = SDL_CreateRGBSurface(SDL_HWSURFACE, cam->get_view().w, cam->get_view().h, 32, 0, 0, 0, 0);
        if (screen == NULL) {
            printf("Throwing error from SheepManager::draw_score\n");
            throw ( ERR::Out_Of_Memory());
        }
        SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0x00, 0xFF, 0xFF));

        // Bereken het aantal punten wat toegekend word. Wanneer er binnen een seconde gefinished word tellen de punten dubbel.
        if (get_sheep_count(2) > scoreCounter) {
            for (int i = 0; i < (get_sheep_count(2) - scoreCounter); i++) {
                int addScore = 0;
                if (scoreTimer.get_ticks() < 1000)
                    addScore = (FINISH_SCORE * 2) * (i + 1);
                else
                    addScore = FINISH_SCORE;

                scoreCounter++;
                scoreTimer.start();

                score += addScore;
            }
        }

        // Maak een stream om de score om te vormen naar een char*.
        std::stringstream text;
        text << "Score: " << score << " punten";

        // Maak het scherm met de score en blit deze op het screen.
        SDL_Surface* scoreText = TTF_RenderText_Solid(font, text.str().c_str(), textColor);
        SDL_Rect offset;
        offset.x = (cam->get_view().w - scoreText->w) / 2;
        offset.y = 15;
        SDL_BlitSurface(scoreText, NULL, screen, &offset);
        SDL_FreeSurface(scoreText);

        // Geef het geblitte scherm terug met transparantie.
        SDL_SetColorKey(screen, SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 0, 0xFF, 0xFF));
        return screen;
    }

    return NULL;
}

// Ga voor het schaap kijken of dit in de buurt van TYPECHAR ( character of hond ) komt. De CLOSURE vlag is om te bepalen van welke kant dit komt ( ALL, UP, DOWN, LEFT, 
// RIGHT ) 

bool SheepManager::check_closure(Sheep* sheep, TYPECHAR type, CLOSURE closure) {
    // Als het schaap al gefinished is geef dan false terug
    if (sheep->is_finished())
        return false;

    // Referentie naar een collision box om de closure te checken en een bool om bij te houden of er ergens een collision is	
    SDL_Rect* checkBox = 0;

    // Zet de collision box vast naargelang de TYPECHAR vlag 
    if (type)
        checkBox = &(dog->get_box());
    else {
        checkBox = &(character->get_box());
        //std::cout << "checkBox = &( character->get_box() )\n";
    }

    // Bepaal welke closure gevraagd wordt en voer naargelang actie uit
    switch (closure) {
        case( ALL):
        {
            // Check of het karakter boven te dicht in de buurt komt
            if ((checkBox->y + (CHARACTER_HEIGHT + SCARSE_REGION) >= sheep->get_box().y) && (checkBox->y <= sheep->get_box().y) &&
                    ((checkBox->x + (CHARACTER_WIDTH + (SCARSE_REGION / 2)) >= sheep->get_box().x) &&
                    (checkBox->x <= sheep->get_box().x + (sheep->get_box().w + (SCARSE_REGION / 2))))) {
                sheep->set_yVel((0 + sheep->get_inner()) + Y_SPEED);
                sheep->set_state(WALK_DOWN);
                sheep->handledMovement = true;
            }
            // Check of het karakter onder te dicht in de buurt komt
            if ((checkBox->y <= sheep->get_box().y + (sheep->get_box().h + SCARSE_REGION)) && (checkBox->y >= (sheep->get_box().y + sheep->get_box().h)) &&
                    ((checkBox->x + (CHARACTER_WIDTH + (SCARSE_REGION / 2)) >= sheep->get_box().x) &&
                    (checkBox->x <= sheep->get_box().x + (sheep->get_box().w + (SCARSE_REGION / 2))))) {
                sheep->set_yVel((0 - sheep->get_inner()) - Y_SPEED);
                sheep->set_state(WALK_UP);
                sheep->handledMovement = true;
            }
            // Check of het karakter links te dicht in de buurt komt
            if ((checkBox->x + (CHARACTER_WIDTH + SCARSE_REGION) >= sheep->get_box().x) && (checkBox->x <= sheep->get_box().x) &&
                    ((checkBox->y + (CHARACTER_HEIGHT + (SCARSE_REGION / 2)) >= sheep->get_box().y) &&
                    (checkBox->y <= sheep->get_box().y + (sheep->get_box().h + (SCARSE_REGION / 2))))) {
                sheep->set_xVel((0 + sheep->get_inner()) + X_SPEED);
                sheep->set_state(WALK_RIGHT);
                sheep->handledMovement = true;
            }
            if ((checkBox->x <= sheep->get_box().x + (sheep->get_box().w + SCARSE_REGION)) && (checkBox->x >= (sheep->get_box().x + sheep->get_box().w)) &&
                    ((checkBox->y + (CHARACTER_HEIGHT + (SCARSE_REGION / 2)) >= sheep->get_box().y) &&
                    (checkBox->y <= sheep->get_box().y + (sheep->get_box().h + (SCARSE_REGION / 2))))) {
                sheep->set_xVel((0 - sheep->get_inner()) - X_SPEED);
                sheep->set_state(WALK_LEFT);
                sheep->handledMovement = true;
            }
            break;
        }
        case( UP):
        {
            // Check of het karakter boven te dicht in de buurt komt
            if ((checkBox->y + (CHARACTER_HEIGHT + SCARSE_REGION) >= sheep->get_box().y) && (checkBox->y <= sheep->get_box().y) &&
                    ((checkBox->x + (CHARACTER_WIDTH + (SCARSE_REGION / 2)) >= sheep->get_box().x) &&
                    (checkBox->x <= sheep->get_box().x + (sheep->get_box().w + (SCARSE_REGION / 2))))) {
                sheep->set_yVel((0 + sheep->get_inner()) + Y_SPEED);
                sheep->set_state(WALK_DOWN);
                sheep->handledMovement = true;
            }
            break;
        }
        case( DOWN):
        {
            // Check of het karakter onder te dicht in de buurt komt
            if ((checkBox->y <= sheep->get_box().y + (sheep->get_box().h + SCARSE_REGION)) && (checkBox->y >= (sheep->get_box().y + sheep->get_box().h)) &&
                    ((checkBox->x + (CHARACTER_WIDTH + (SCARSE_REGION / 2)) >= sheep->get_box().x) &&
                    (checkBox->x <= sheep->get_box().x + (sheep->get_box().w + (SCARSE_REGION / 2))))) {
                sheep->set_yVel((0 - sheep->get_inner()) - Y_SPEED);
                sheep->set_state(WALK_UP);
                sheep->handledMovement = true;
            }
            break;
        }
        case( LEFT):
        {
            // Check of het karakter links te dicht in de buurt komt
            if ((checkBox->x + (CHARACTER_WIDTH + SCARSE_REGION) >= sheep->get_box().x) && (checkBox->x <= sheep->get_box().x) &&
                    ((checkBox->y + (CHARACTER_HEIGHT + (SCARSE_REGION / 2)) >= sheep->get_box().y) &&
                    (checkBox->y <= sheep->get_box().y + (sheep->get_box().h + (SCARSE_REGION / 2))))) {
                sheep->set_xVel((0 + sheep->get_inner()) + X_SPEED);
                sheep->set_state(WALK_RIGHT);
                sheep->handledMovement = true;
            }
            break;
        }
        case( RIGHT):
        {
            if ((checkBox->x <= sheep->get_box().x + (sheep->get_box().w + SCARSE_REGION)) && (checkBox->x >= (sheep->get_box().x + sheep->get_box().w)) &&
                    ((checkBox->y + (CHARACTER_HEIGHT + (SCARSE_REGION / 2)) >= sheep->get_box().y) &&
                    (checkBox->y <= sheep->get_box().y + (sheep->get_box().h + (SCARSE_REGION / 2))))) {
                sheep->set_xVel((0 - sheep->get_inner()) - X_SPEED);
                sheep->set_state(WALK_LEFT);
                sheep->handledMovement = true;
            }
            break;
        }
        default: break;
    }

    return sheep->handledMovement;
}




