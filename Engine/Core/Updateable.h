/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_UPDATEABLE_H
#define EN_UPDATEABLE_H

namespace engine
{
    class Updateable
    {
    public:
        Updateable(void);
        virtual ~Updateable(void);

        virtual void Update(float dt) = 0;

        bool active;
    };
}

#endif
