#include <array>

#include "Bot.h"

using namespace std;

//constructor
Bot::Bot()
{
  map< Location, Location > orders;
  map< Location, Location > targets;
};

//plays a single game of Ants.
void Bot::playGame()
{
    //reads the game parameters and sets up
    cin >> state;
    state.setup();
    endTurn();

    //continues making moves while the game is not over
    while(cin >> state)
    {
        state.updateVisionInformation();
        doTurn();
//        makeMoves();
        endTurn();
    }
};

//makes the bots moves for the turn
void Bot::makeMoves()
{
    state.bug << "turn " << state.turn << ":" << endl;
    state.bug << state << endl;

    //picks out moves for each ant
    for(int ant=0; ant<(int)state.myAnts.size(); ant++)
    {
        for(int d=0; d<TDIRECTIONS; d++)
        {
            Location loc = state.getLocation(state.myAnts[ant], d);

            if(!state.grid[loc.row][loc.col].isWater)
            {
                state.makeMove(state.myAnts[ant], d);
                break;
            }
        }
    }

    state.bug << "time taken: " << state.timer.getTime() << "ms" << endl << endl;
};

bool Bot::doMoveDirection(const Location & antLoc, int dir)
{
  Location newLoc = state.getLocation( antLoc, dir );
  if ( state.isFree( newLoc ) && orders.find( newLoc ) == orders.end() )
  {
    state.makeMove( antLoc, dir );
    orders.insert( pair< Location, Location >( newLoc, antLoc ) );
    return true;
  }
  else
    return false;
}

bool Bot::doMoveLocation(const Location & antLoc, const Location & destLoc)
{
  array< int, 2 > directions; // Initialization

  int ndirs = state.getDirection(antLoc, destLoc, directions);
  for ( int i = 0 ; i < ndirs ; ++i )
    if ( doMoveDirection( antLoc, directions[i] ) )
    {
      targets.insert( pair< Location, Location>( destLoc, antLoc ) );
      return true;
    }

  return false;
}

void Bot::doTurn()
{
  int distance = 0;
  uint nFood = state.food.size(), nMyAnts = state.myAnts.size();
  Route foodRoute;
  array< Route, nFood*nMyAnts > foodRoutes; 

  orders.clear();
  targets.clear(); 

  for ( uint foodIdx = 0 ; foodIdx < nFood ; ++foodIdx )
    for ( uint antIdx = 0 ; antIdx < nMyAnts ; ++antIdx )
    {
      distance = state.distance( state.myAnts[ antIdx ], state.food[ foodIdx ] );
      foodRoute = Route( state.myAnts[ antIdx ], state.food[ foodIdx ], distance );
      foodRoutes[ foodIdx*nMyAnts + antIdx ] = foodRoute;
    }

  sort( foodRoutes.begin(), foodRoutes.end() );

  for ( uint routeIdx = 0 ; routeIdx < nFood*nMyAnts ; ++routeIdx )
  {
    if ( !targets.find( route.getDest() ) )
  }

}

//finishes the turn
void Bot::endTurn()
{
    if(state.turn > 0)
        state.reset();
    state.turn++;

    cout << "go" << endl;
};
