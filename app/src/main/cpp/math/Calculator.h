#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>

class Calculator {
public:
    static float evaluateExpression(std::string expression);

private:
    static float evaluate(const std::string& expression);
    static bool isOperatorOrLeftParen(char c);
    static bool hasHigherPrecedence(char op1, char op2);
    static int getPrecedence(char op);
    static float applyOperation(float a, float b, char op);
};

#endif // CALCULATOR_H