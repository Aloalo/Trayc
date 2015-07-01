/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include "TwoVariableFunction.h"
#include <iostream>
#include "TmpHandlers.h"

using namespace std;
using namespace glm;
using namespace engine;

TwoVariableFunction::TwoVariableFunction(void)
{
    mSymbolTable.add_variable("x", mVarA);
    mSymbolTable.add_variable("y", mVarB);
    mSymbolTable.add_constants();
    mExpression.register_symbol_table(mSymbolTable);
}

void TwoVariableFunction::SetFunction(const string &expressionString)
{
    if(!mParser.compile(expressionString, mExpression))
    {
        cerr << "Invalid function: " << expressionString << endl;
        inputHandler.SetQuit();
    }
}

float TwoVariableFunction::operator()(const vec2 &x)
{
    mVarA = x.x;
    mVarB = x.y;
    return mExpression.value();
}
