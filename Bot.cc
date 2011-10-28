#include "Bot.h"

using namespace std;

//constructor
Bot::Bot()
{
  map< Location, Location > orders;
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
        makeMoves();
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
    printf(">>>>>>> (%d, %d) added to orders\n", newLoc.row, newLoc.col);
    return true;
  }
  else
    return false;
}

void Bot::doTurn()
{
  orders.clear();

  for ( uint antIdx = 0 ; antIdx < state.myAnts.size() ; ++antIdx )
    for ( int d = 0 ; d < TDIRECTIONS ; ++d )
      if ( doMoveDirection( state.myAnts[ antIdx ], d ) ) break;

}

//finishes the turn
void Bot::endTurn()
{
    if(state.turn > 0)
        state.reset();
    state.turn++;

    cout << "go" << endl;
};
