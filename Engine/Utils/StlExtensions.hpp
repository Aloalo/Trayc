/*
* Author: Jure Ratkovic
*/

#ifndef STLEXTENSIONS_H
#define STLEXTENSIONS_H

#include <algorithm>
#include <assert.h>

namespace stdext
{
    template<typename T>
    using Iterator = typename T::iterator;

    template<typename T>
    using ValueType = typename T::value_type;

    template<typename Container, typename Value>
    inline Iterator<Container> find(Container &c, const Value &v)
    {
        return std::find(begin(c), end(c), v);
    }

    template<typename Container, typename Predicate>
    inline Iterator<Container> remove_if(Container &c, Predicate p)
    {
        return std::remove_if(begin(c), end(c), p);
    }

    template<typename Container, typename Value>
    inline Container& erase(Container &c, const Value &v)
    {
        const Iterator<Container> eraseLocation = find(c, v);
        assert(eraseLocation != end(c));
        c.erase(eraseLocation);
        return c;
    }

    template<typename Container, typename Predicate>
    inline Container& erase_if(Container &c, Predicate p)
    {
        c.erase(remove_if(c, p), end(c));
        return c;
    }

    template<typename Container, typename Predicate>
    inline Container& sort(Container &c, Predicate p)
    {
        std::sort(begin(c), end(c), p);
        return c;
    }

    template<typename Container>
    inline Container& sort(Container &c)
    {
        std::sort(begin(c), end(c));
        return c;
    }
    
    template<typename Container>
    inline void fill(Container &c, const ValueType<Container> &v)
    {
        std::fill(begin(c), end(c), v);
    }

    template<typename Container>
    inline ValueType<Container> take_top(Container &c)
    {
        const ValueType<Container> t = c.top();
        c.pop();
        return t;
    }
}

#endif
