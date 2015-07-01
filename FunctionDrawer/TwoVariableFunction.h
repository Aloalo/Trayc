/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef FD_TWOVARIABLEFUNCTION_H
#define FD_TWOVARIABLEFUNCTION_H

#include <exprtk.hpp>
#include <string>
#include <glm/glm.hpp>

class TwoVariableFunction
{
public:
    TwoVariableFunction(void);

    void SetFunction(const std::string &expressionString);
    float operator()(const glm::vec2 &x);

private:
    //Math stuff
    exprtk::expression<float> mExpression;
    exprtk::parser<float> mParser;
    exprtk::symbol_table<float> mSymbolTable;
    float mVarA, mVarB;
};

#endif
