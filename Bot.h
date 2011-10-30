#ifndef BOT_H_
#define BOT_H_

#include <boost/bimap.hpp>
#include <map>
#include <set>
#include <tuple>

#include "State.h"
#include "Location.h"

using namespace boost::bimaps;

typedef std::tuple< Location, Location, double > Route;


// tags

struct AntLocation {};
struct FoodLocation {};

typedef boost::bimap
<

  tagged< Location, FoodLocation >, tagged< Location, AntLocation >

> TargetsBimap;

/*
    This struct represents your bot in the game of Ants
*/
struct Bot
{
    State state;

    std::map< Location, Location > orders;

    std::set< Location > unseen;

    // This map tracks the foods targets and associated ants
    TargetsBimap targets;

    Bot();

    void playGame();    //plays a single game of Ants

    void makeMoves();   //makes moves for a single turn

    bool doMoveDirection(const Location & antLoc, int direction);

    bool doMoveLocation(const Location & antLoc, const Location & destLoc);

    void doTurn();

    void endTurn();     //indicates to the engine that it has made its moves

    static bool cmpRoutes(const Route & lhs, const Route & rhs);

};

std::ostream & operator<<(std::ostream & os, const Route & route);

#endif //BOT_H_
