#include "Location.h"

using namespace std;

ostream & operator <<(ostream & os, const Location & loc)
{
  os << "(" << loc.row << ", " << loc.col << ")";
  return os;
}
