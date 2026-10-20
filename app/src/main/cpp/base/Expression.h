#ifndef CPP_BASE_EXPRESSION_H
#define CPP_BASE_EXPRESSION_H

#include <string>
#include <vector>

class Expression {
public:
    class VariableResolver {
    public:
        virtual ~VariableResolver() {}
        virtual float getValue(const std::string& varName) = 0;
    };

    Expression(const std::string& exprStr);

    float evaluate(VariableResolver* resolver) const;

    std::string toString() const;

private:
    enum TokenType { NUMBER, VARIABLE, OP };

    class Token {
    public:
        TokenType type;
        float numberValue;
        std::string varName;
        char opChar;

        Token(TokenType type, float val);
        Token(TokenType type, const std::string& var);
        Token(TokenType type, char op);

        std::string toString() const;
    };

    std::vector<Token> rpnTokens;

    std::vector<Token> tokenize(const std::string& expr) const;
    std::vector<Token> compile(const std::string& expr) const;
    int precedence(char op) const;
    float applyOp(char op, float a, float b) const;
};

#endif // CPP_BASE_EXPRESSION_H