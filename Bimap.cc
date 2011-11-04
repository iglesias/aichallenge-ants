using namespace std;

template < class TKey, class TValue >
bool Bimap< TKey, TValue >::containsKey(const TKey & key)
{
  return ( forwardMap.find( key ) != forwardMap.end() );
}

template < class TKey, class TValue >
typename map< TKey, TValue >::iterator Bimap< TKey, TValue >::endKey()
{
  return forwardMap.end();
}

template < class TKey, class TValue >
bool Bimap< TKey, TValue >::containsValue(const TValue & value)
{
  return ( reverseMap.find( value ) != reverseMap.end() );
}

template < class TKey, class TValue >
typename map< TValue, TKey >::iterator Bimap< TKey, TValue >::endValue()
{
  return reverseMap.end();
}

template < class TKey, class TValue >
bool Bimap< TKey, TValue>::insert(const TKey & key, const TValue & value)
{
  if ( !containsKey(key) )
  {
    forwardMap.insert( pair< TKey, TValue >(key, value) );
    reverseMap.insert( pair< TValue, TKey >(value, key) );
    return true;
  }
  else
    return false;
}

template < class TKey, class TValue >
void Bimap< TKey, TValue >::clear()
{
  forwardMap.clear();
  reverseMap.clear();
}
