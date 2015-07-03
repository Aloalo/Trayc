/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include "TwoVariableFunction.h"
#include <iostream>

using namespace std;
using namespace glm;

TwoVariableFunction::TwoVariableFunction(const string &expressionString)
{
    mSymbolTable.add_variable("x", mVarA);
    mSymbolTable.add_variable("y", mVarB);
    mSymbolTable.add_constants();
    mExpression.register_symbol_table(mSymbolTable);

    if(!mParser.compile(expressionString, mExpression))
    {
        cerr << "Invalid function: " << expressionString << endl;
    }
}

float TwoVariableFunction::operator()(const vec2 &x)
{
    mVarA = x.x;
    mVarB = x.y;
    return mExpression.value();
}

float TwoVariableFunction::operator()(float x, float z)
{
    mVarA = x;
    mVarB = z;
    return mExpression.value();
}
