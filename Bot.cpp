#include <algorithm>
#include <array>

#include "Bot.h"

using namespace std;

//constructor
Bot::Bot()
{
};

//plays a single game of Ants.
void Bot::playGame()
{
    //reads the game parameters and sets up
    cin >> state;
    state.setup();
    endTurn();

    // All the locations are unseen at the beginning
    for ( int row = 0 ; row < state.rows ; ++row )
      for ( int col = 0 ; col < state.cols ; ++col )
        unseen.insert( Location(row, col) );
    state.bug << "after the constructor " << unseen.size() << endl;

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
      targets.insert(destLoc, antLoc);
      return true;
    }

  return false;
}

void Bot::doTurn()
{
  int distance = 0;
  const uint nFood = state.food.size(), nMyAnts = state.myAnts.size();
  Route route;
  vector< Route > foodRoutes( nFood*nMyAnts, make_tuple(Location(), Location(), 0) ); 
  vector< Location >::iterator hillIt;
  map< Location, Location >::iterator ordersIt;
  bool movingOut = false;

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
      route = make_tuple( state.myAnts[ antIdx ], state.food[ foodIdx ], distance );
      foodRoutes[ foodIdx*nMyAnts + antIdx ] = route;
    }

  sort( foodRoutes.begin(), foodRoutes.end(), cmpRoutes );

#ifdef DEBUG
  state.bug << ">> " << nMyAnts*nFood << " routes computed!" << endl << endl;
  state.bug << ">> Sorted routes:" << endl;
  for ( uint routeIdx = 0 ; routeIdx < nFood*nMyAnts ; ++routeIdx )
    state.bug << ">>>> " << foodRoutes[ routeIdx ] << endl;
  state.bug << endl;
#endif

  vector< Route >::iterator routeIt;
  for ( routeIt = foodRoutes.begin() ; routeIt < foodRoutes.end() ; ++routeIt )
  {

    if ( !targets.containsKey( get<1>( *routeIt ) ) )  // food already queried?
    {
      if ( !targets.containsValue( get<0>( *routeIt ) ) ) // ant already doing something?
        if ( doMoveLocation( get<0>( *routeIt ), get<1>( *routeIt ) ) )
        {
#ifdef DEBUG
          state.bug << ">> Added move from " << get<0>( *routeIt ) << " to " 
                                             << get<1>( *routeIt ) << endl;
#endif
        }
    }

  }

  // Remove the locations that are visible
  state.bug << "there are " << unseen.size() << " unseen locations here" << endl;

  set< Location >::iterator unseenIt = unseen.begin();
  while ( unseenIt != unseen.end() )
  {
    if ( state.grid[ unseenIt->row ][ unseenIt->col ].isVisible )
      unseen.erase( unseenIt++ );
    else
      ++unseenIt;
  }

  state.bug << "there are " << unseen.size() << " unseen locations here" << endl;

  // Explore unseen areas
  vector< Route > unseenRoutes( unseen.size(), make_tuple(Location(), Location(), 0) );
  for ( uint antIdx = 0 ; antIdx < state.myAnts.size() ; ++antIdx )
  {
    // Check that we are not already moving the ant
    movingOut = false;
    for ( ordersIt = orders.begin() ; ordersIt != orders.end() ; ++ordersIt )
      if ( ordersIt->second == state.myAnts[ antIdx ] )
      {
        movingOut = true;
        break;
      }

    if ( !movingOut )
    {
      int i;
      for ( i = 0, unseenIt = unseen.begin() ; unseenIt != unseen.end() ; ++unseenIt )
      {
        distance = state.distance( state.myAnts[ antIdx ], *unseenIt );
        route = make_tuple( state.myAnts[ antIdx ], *unseenIt, distance );
        unseenRoutes[ i++ ] = route;
      }

      sort( unseenRoutes.begin(), unseenRoutes.end(), cmpRoutes );

      for ( routeIt = unseenRoutes.begin() ; routeIt != unseenRoutes.end() ; ++routeIt )
        if ( doMoveLocation( get<0>( *routeIt ), get<1>( *routeIt ) ) ) break;

    }
  }

  // Move out from our hills
  for ( hillIt = state.myHills.begin() ; hillIt != state.myHills.end() ; ++hillIt )
  {
    vector< Location >::iterator it = 
      find(state.myAnts.begin(), state.myAnts.end(), *hillIt);
    if ( it != state.myAnts.end() )
    {
      // Check that we are not already moving the ant
      movingOut = false;
      for ( ordersIt = orders.begin() ; ordersIt != orders.end() ; ++ordersIt )
        if ( ordersIt->second == *hillIt )
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
