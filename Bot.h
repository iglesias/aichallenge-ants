#ifndef BOT_H_
#define BOT_H_

#include "State.h"
#include "Location.h"
#include <map>

/*
    This struct represents your bot in the game of Ants
*/
struct Bot
{
    State state;

    std::map< Location, Location > orders;

    Bot();

    void playGame();    //plays a single game of Ants

    void makeMoves();   //makes moves for a single turn

    bool doMoveDirection(const Location & antLoc, int direction);

    void doTurn();

    void endTurn();     //indicates to the engine that it has made its moves
};

#endif //BOT_H_
