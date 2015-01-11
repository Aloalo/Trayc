/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include "TwoVariableFunction.h"
#include <iostream>
#include <Engine/Core/EventHandler.h>

using namespace std;
using namespace glm;
using namespace engine;

TwoVariableFunction::TwoVariableFunction(void)
{
    symbol_table.add_variable("x", a);
    symbol_table.add_variable("y", b);
    symbol_table.add_constants();
    expression.register_symbol_table(symbol_table);
}

void TwoVariableFunction::SetFunction(const string &expressionString)
{
    if(!parser.compile(expressionString, expression))
    {
        cerr << "Invalid function: " << expressionString << endl;
        EventHandler::SetQuit();
    }
}

float TwoVariableFunction::operator()(const vec2 &x)
{
    a = x.x;
    b = x.y;
    return expression.value();
}
