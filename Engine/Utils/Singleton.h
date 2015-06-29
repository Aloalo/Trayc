/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_SINGLETON_H
#define EN_SINGLETON_H

namespace engine
{
    template<class D>
    struct Singleton
    {
        static D& Get()
        {
            if(!mInstance)
                mInstance = new D;
            return *mInstance;
        }

    private:
        static D *mInstance;
    };

    template<class D>
    D *Singleton<D>::mInstance = nullptr;
}

#endif
