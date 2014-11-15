/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_SINGLETON_H
#define EN_SINGLETON_H

template<class D>
struct Singleton
{
    static D& Get()
    {
        if(!instance)
            instance = new D;
        return *instance;
    }

private:
    static D *instance;
};

template<class D>
D *Singleton<D>::instance = 0;

#endif
