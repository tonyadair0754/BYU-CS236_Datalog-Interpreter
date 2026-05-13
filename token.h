#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <cctype>

using namespace std;

enum TokenType {
        COMMA,
        PERIOD,
        Q_MARK,
        LEFT_PAREN,
        RIGHT_PAREN,
        COLON,
        COLON_DASH,
        MULTIPLY,
        ADD,
        SCHEMES,
        FACTS,
        RULES,
        QUERIES,
        ID,
        STRING,
        COMMENT,
        UNDEFINED,
        END
};

class Token {

        private:
                TokenType type;
                string value;
                int line;

        public:
		Token(TokenType type, string value, int line)
			 : type(type), value(value), line(line) { }

                string typeName(TokenType type) const {
                        switch (type) {
                                case COMMA: return "COMMA";
                                case PERIOD: return "PERIOD";
                                case Q_MARK: return "Q_MARK";
                                case LEFT_PAREN: return "LEFT_PAREN";
                                case RIGHT_PAREN: return "RIGHT_PAREN";
                                case COLON: return "COLON";
                                case COLON_DASH: return "COLON_DASH";
                                case MULTIPLY: return "MULTIPLY";
                                case ADD: return "ADD";
                                case SCHEMES: return "SCHEMES";
                                case FACTS: return "FACTS";
                                case RULES: return "RULES";
								case QUERIES: return "QUERIES";
                                case ID: return "ID";
                                case STRING: return "STRING";
                                case COMMENT: return "COMMENT";
                                case END: return "END";
                                default: return "UNDEFINED";
                	}
		}

		TokenType getType() const {
                	return type;
                }

		string getValue() const {
			return value;
		}

		string toString() const {
			stringstream out;

			out << "("
				<< typeName(type)
				<< ",\""
				<< value
				<< "\","
				<< line
				<< ")";

			return out.str();
		}
};
