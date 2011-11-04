#ifndef _BIMAP_H_
#define _BIMAP_

#include <map>

template < class TKey, class TValue >
class Bimap {

  public:

    bool containsKey(const TKey & key)
    {
      return ( forwardMap.find( key ) != forwardMap.end() );
    }

    typename std::map< TKey, TValue >::iterator endKey()
    {
      return forwardMap.end();
    }

    bool containsValue(const TValue & value)
    {
      return ( reverseMap.find( value ) != reverseMap.end() );
    }

    typename std::map< TValue, TKey >::iterator endValue()
    {
    return reverseMap.end();
    }

    bool insert(const TKey & key, const TValue & value)
    {
      if ( !containsKey(key) )
      {
        forwardMap.insert( std::pair< TKey, TValue >(key, value) );
        reverseMap.insert( std::pair< TValue, TKey >(value, key) );
        return true;
      }
      else
        return false;
    }

    void clear()
    {
      forwardMap.clear();
      reverseMap.clear();
    }

  private:
  
    std::map< TKey, TValue > forwardMap;

    std::map< TValue, TKey > reverseMap;

};

#endif // _BIMAP_H
