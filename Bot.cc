#include <algorithm>
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
    state.bug << "inseerted move towards " << newLoc << endl;
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
      targets.insert( TargetsBimap::value_type( destLoc, antLoc ) );
      return true;
    }

  return false;
}

void Bot::doTurn()
{
  int distance = 0;
  const uint nFood = state.food.size(), nMyAnts = state.myAnts.size();
  Route foodRoute;
  vector< Route > foodRoutes( nFood*nMyAnts, make_tuple(Location(), Location(), 0) ); 
  vector< Location >::iterator hillIt;

  orders.clear();
  targets.clear(); 

  // Prevent stepping on our own hills
  for ( hillIt = state.myHills.begin() ; hillIt < state.myHills.end() ; ++hillIt )
    orders.insert( pair< Location, Location >( *hillIt, Location() ) );

  // Food gathering
  for ( uint foodIdx = 0 ; foodIdx < nFood ; ++foodIdx )
    for ( uint antIdx = 0 ; antIdx < nMyAnts ; ++antIdx )
    {
      distance = state.distance( state.myAnts[ antIdx ], state.food[ foodIdx ] );
      foodRoute = make_tuple( state.myAnts[ antIdx ], state.food[ foodIdx ], distance );
      foodRoutes[ foodIdx*nMyAnts + antIdx ] = foodRoute;
    }

  sort( foodRoutes.begin(), foodRoutes.end(), cmpRoutes );

#ifdef DEBUG
  state.bug << ">> " << nMyAnts*nFood << " routes computed!" << endl << endl;
  state.bug << ">> Sorted routes:" << endl;
  for ( uint routeIdx = 0 ; routeIdx < nFood*nMyAnts ; ++routeIdx )
    state.bug << ">>>> " << foodRoutes[ routeIdx ] << endl;
  state.bug << endl;
#endif

  TargetsBimap::map_by< FoodLocation >::const_iterator foodIt;
  TargetsBimap::map_by< AntLocation  >::const_iterator antIt;
  vector< Route >::iterator routeIt;
  for ( routeIt = foodRoutes.begin() ; routeIt < foodRoutes.end() ; ++routeIt )
  {

    foodIt = targets.by< FoodLocation >().find( get<1>( *routeIt ) );
    if ( foodIt == targets.by< FoodLocation >().end() )  // food location already queried?
    {
      antIt = targets.by< AntLocation >().find( get<0>( *routeIt ) );
      if ( antIt == targets.by< AntLocation >().end() ) // ant already doing something?
        if ( doMoveLocation( get<0>( *routeIt ), get<1>( *routeIt ) ) )
        {
#ifdef DEBUG
          state.bug << ">> Added move from " << get<0>( *routeIt ) << " to " 
                                             << get<1>( *routeIt ) << endl;
#endif
        }
    }

  }

  // Move out from our hills
  map< Location, Location >::iterator ordersIt;
  bool movingOut = false;
  for ( hillIt = state.myHills.begin() ; hillIt < state.myHills.end() ; ++hillIt )
  {
    vector< Location >::iterator it = 
      find(state.myHills.begin(), state.myHills.end(), *hillIt);
    if ( it != state.myAnts.end() )
    {
      // Check that we are not already moving from the hill
      for ( ordersIt = orders.begin() ; ordersIt != orders.end() ; ++ordersIt )
        if ( ordersIt->second == *hillIt)
        {
          movingOut = true;
          break;
        }

      if ( !movingOut )
        for ( int d = 0 ; d < TDIRECTIONS ; ++d )
        {
          if ( doMoveDirection( *hillIt, d ) )
            break;
        }

    }
  }

}

//finishes the turn
void Bot::endTurn()
{
    if(state.turn > 0)
        state.reset();
    state.turn++;

    cout << "go" << endl;
}

bool Bot::cmpRoutes(const Route & lhs, const Route & rhs)
{
  return ( std::get<2>(lhs) < std::get<2>(rhs) );
}

ostream & operator<<(ostream & os, const Route & route)
{
  os << get<0>( route ) << " " << get<1>( route ) << " " << get<2>( route); 
  return os;
}
