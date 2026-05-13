#pragma once
#include <vector>
#include <set>
#include <string>

#include "token.h"
#include "datalog.h"
#include "predicate.h"
#include "rules.h"

using namespace std;

class Parser {
private:
    vector<Token> tokens;
    size_t tokenIndex;         // Current position in the token vector

    DatalogProgram datalogProgramObj;

    TokenType currentTokenType() const;

    const Token& currentToken() const;

    void advanceToken();

    void match(TokenType expectedType);

    Predicate parseScheme();

    Predicate parseFact();

    Rule parseRule();

    Predicate parseQuery();

    Predicate parseHeadPredicate();

    Predicate parsePredicate();

    Parameter parseParameter();

    vector<Parameter> parseIdList();

    vector<Parameter> parseStringList();

    vector<Predicate> parsePredicateList();

    vector<Parameter> parseParameterList();

public:
    Parser(const vector<Token>& tokens);
    DatalogProgram parse();
};

