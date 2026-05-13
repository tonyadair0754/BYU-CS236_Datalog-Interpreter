#include "parser.h"
#include <iostream>

using namespace std;

// Constructor
Parser::Parser(const vector<Token>& tokens)
    : tokens(tokens), tokenIndex(0) {}

// Helper functions
TokenType Parser::currentTokenType() const {
    if (tokenIndex < tokens.size()) {
        return tokens.at(tokenIndex).getType();
    }
    return END;
}

const Token& Parser::currentToken() const {
    return tokens.at(tokenIndex);
}

void Parser::advanceToken() {
    tokenIndex++;
}

void Parser::match(TokenType expectedType) {
    if (currentTokenType() == expectedType) {
        advanceToken();
    } else {
        throw currentToken();
    }
}

DatalogProgram Parser::parse() {
    match(SCHEMES);
    match(COLON);

    // MUST have at least one scheme
    if (currentTokenType() != ID) {
        throw currentToken();
    }
    while (currentTokenType() == ID) {
        datalogProgramObj.schemes.push_back(parseScheme());
    }

    match(FACTS);
    match(COLON);
    while (currentTokenType() == ID) {
        datalogProgramObj.facts.push_back(parseFact());
    }

    match(RULES);
    match(COLON);
    while (currentTokenType() == ID) {
        datalogProgramObj.rules.push_back(parseRule());
    }

    match(QUERIES);
    match(COLON);

    // MUST have at least one query
    if (currentTokenType() != ID) {
        throw currentToken();
    }
    while (currentTokenType() == ID) {
        datalogProgramObj.queries.push_back(parseQuery());
    }

    match(END);
    return datalogProgramObj;
}

// Parsing individual structures
Predicate Parser::parseScheme() {
    string name = currentToken().getValue();
    match(ID);
    Predicate p(name);
    match(LEFT_PAREN);
    p.addParameter(Parameter(currentToken().getValue(), false));
    match(ID);
    for (const auto& param : parseIdList()) {
        p.addParameter(param);
    }
    match(RIGHT_PAREN);
    return p;
}

Predicate Parser::parseFact() {
    string name = currentToken().getValue();
    match(ID);
    Predicate p(name);
    match(LEFT_PAREN);

    string stringVal = currentToken().getValue();
    datalogProgramObj.domain.insert(stringVal);
    p.addParameter(Parameter(stringVal, true));
    match(STRING);

    for (const auto& param : parseStringList()) {
        datalogProgramObj.domain.insert(param.value);
        p.addParameter(param);
    }

    match(RIGHT_PAREN);
    match(PERIOD);
    return p;
}

Rule Parser::parseRule() {
    Predicate head = parseHeadPredicate();
    Rule r(head);
    match(COLON_DASH);
    r.body.push_back(parsePredicate());
    for (const auto& pred : parsePredicateList()) {
        r.body.push_back(pred);
    }
    match(PERIOD);
    return r;
}

Predicate Parser::parseQuery() {
    Predicate q = parsePredicate();
    match(Q_MARK);
    return q;
}

Predicate Parser::parseHeadPredicate() {
    string name = currentToken().getValue();
    match(ID);
    Predicate p(name);
    match(LEFT_PAREN);
    p.addParameter(Parameter(currentToken().getValue(), false));
    match(ID);
    for (const auto& param : parseIdList()) {
        p.addParameter(param);
    }
    match(RIGHT_PAREN);
    return p;
}

Predicate Parser::parsePredicate() {
    string name = currentToken().getValue();
    match(ID);
    Predicate p(name);
    match(LEFT_PAREN);
    p.addParameter(parseParameter());
    for (const auto& param : parseParameterList()) {
        p.addParameter(param);
    }
    match(RIGHT_PAREN);
    return p;
}

// parameter -> STRING | ID
Parameter Parser::parseParameter() {
    if (currentTokenType() == STRING) {
        string val = currentToken().getValue();
        match(STRING);
        return Parameter(val, true);
    } else if (currentTokenType() == ID) {
        string val = currentToken().getValue();
        match(ID);
        return Parameter(val, false);
    } else {
        throw currentToken();
    }
}

// idList -> COMMA ID idList | lambda
vector<Parameter> Parser::parseIdList() {
    vector<Parameter> ids;
    if (currentTokenType() == COMMA) {
        match(COMMA);
        ids.push_back(Parameter(currentToken().getValue(), false));
        match(ID);
        auto rest = parseIdList();
        ids.insert(ids.end(), rest.begin(), rest.end());
    }
    return ids;
}

// stringList -> COMMA STRING stringList | lambda
vector<Parameter> Parser::parseStringList() {
    vector<Parameter> strings;
    if (currentTokenType() == COMMA) {
        match(COMMA);
        strings.push_back(Parameter(currentToken().getValue(), true));
        match(STRING);
        auto rest = parseStringList();
        strings.insert(strings.end(), rest.begin(), rest.end());
    }
    return strings;
}

// predicateList -> COMMA predicate predicateList | lambda
vector<Predicate> Parser::parsePredicateList() {
    vector<Predicate> predicates;
    if (currentTokenType() == COMMA) {
        match(COMMA);
        predicates.push_back(parsePredicate());
        auto rest = parsePredicateList();
        predicates.insert(predicates.end(), rest.begin(), rest.end());
    }
    return predicates;
}

// parameterList -> COMMA parameter parameterList | lambda
vector<Parameter> Parser::parseParameterList() {
    vector<Parameter> parameters;
    if (currentTokenType() == COMMA) {
        match(COMMA);
        parameters.push_back(parseParameter());
        auto rest = parseParameterList();
        parameters.insert(parameters.end(), rest.begin(), rest.end());
    }
    return parameters;
}