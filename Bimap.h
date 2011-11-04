#ifndef _BIMAP_H_
#define _BIMAP_

#include <map>

template < class TKey, class TValue >
class Bimap {

  public:

    bool containsKey(const TKey & key);

    typename std::map< TKey, TValue >::iterator endKey();

    bool containsValue(const TValue & value);

    typename std::map< TValue, TKey >::iterator endValue();

    bool insert(const TKey & key, const TValue & value);

    void clear();

  private:
  
    std::map< TKey, TValue > forwardMap;

    std::map< TValue, TKey > reverseMap;

};

#include "Bimap.cpp"

#endif // _BIMAP_H
