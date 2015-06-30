/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef FD_TMPHANDLERS_H
#define FD_TMPHANDLERS_H

#include <Engine/Engine.h>

static engine::SDLHandler sdlHandler;
static engine::InputHandler inputHandler;
static engine::UpdateableHandler updateableHandler(1.0f / 60.0f);

#endif
