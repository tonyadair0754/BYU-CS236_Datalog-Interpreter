#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include "token.h"

using namespace std;

class Scanner {
private:
	string input;
	int line;

	void consumeWhitespace() {
		while (!input.empty() && isspace(input.at(0))) {
			if (input.at(0) == '\n') line++;
			input = input.substr(1);
		}
	}

	// Returns true if a comment was consumed, false otherwise.
	bool consumeComment() {
		if (!input.empty() && input.at(0) == '#') {
			if (input.size() > 1 && input.at(1) == '|') { // Multi-line comment
				input = input.substr(2);
				while (!input.empty()) {
					if (input.size() > 1 && input.at(0) == '|' && input.at(1) == '#') {
						input = input.substr(2);
						break;
					}
					if (input.at(0) == '\n') {
						line++;
					}
					input = input.substr(1);
				}
				return true; // A comment was found and consumed
			} else { // Single-line comment
				input = input.substr(1);
				while (!input.empty() && input.at(0) != '\n') {
					input = input.substr(1);
				}
				return true;
			}
		}
		return false; // No comment at current position
	}

public:
	Scanner(const string& input)
	 : input(input), line(1) { }

	vector<Token> scanTokens() {
		vector<Token> tokens;
		while (true) {

			Token t = scanToken();
			tokens.push_back(t);
			if (t.getType() == END) break;
		}
		return tokens;
	}

	Token scanToken() {
		while (true) {
			consumeWhitespace();

			if (input.empty()) {
				return Token(END, "", line);
			}

			if (!input.empty() && input.at(0) == '#') {
				if (consumeComment()) {
					continue;
				}
			}
			break;
		}

		char c = input.at(0);

		// Single-character tokens
		switch (c) {
			case ',':
				input = input.substr(1);
				return Token(COMMA, ",", line);
			case '.':
				input = input.substr(1);
				return Token(PERIOD, ".", line);
			case '?':
				input = input.substr(1);
				return Token(Q_MARK, "?", line);
			case '(':
				input = input.substr(1);
				return Token(LEFT_PAREN, "(", line);
			case ')':
				input = input.substr(1);
				return Token(RIGHT_PAREN, ")", line);
			case '*':
				input = input.substr(1);
				return Token(MULTIPLY, "*", line);
			case '+':
				input = input.substr(1);
				return Token(ADD, "+", line);
		}

		// Colon or Colon-dash
		if (c == ':') {
			if (input.size() >= 2 && input.at(1) == '-') {
				input = input.substr(2);
				return Token(COLON_DASH, ":-", line);
			}
			else {
				input = input.substr(1);
				return Token(COLON, ":", line);
			}
		}

		// Strings
		if (c == '\'') {
			string str = "";
			int startLine = line;
			//bool terminated = false;

			str += input.at(0);
			input = input.substr(1); // consume opening quote

			while (!input.empty()) {
				char ch = input.at(0);
				str += ch;
				input = input.substr(1);

				if (ch == '\n') line++;

				if (ch == '\'') {
					//terminated = true;
					//break;
					return Token(STRING, str, startLine);
				}
			}

			return Token(UNDEFINED, str, startLine);
		}

		if (isdigit(c)) {
			string bad = "";
			int startLine = line;

			while (!input.empty() && isalnum(input.at(0))) {
				bad += input.at(0);
				input = input.substr(1);
			}

			return Token(UNDEFINED, bad, startLine);
		}

		// IDs and keywords
		if (isalpha(c)) {
        	string word = "";
			int startLine = line;

			word += c;
			input = input.substr(1);

        	while (!input.empty() && (isalnum(input.at(0)))) {
            	word += input.at(0);
            	input = input.substr(1);
        	}

        	if (word == "Schemes" && word.length() == 7) return Token(SCHEMES, word, startLine);
        	if (word == "Facts" && word.length() == 5) return Token(FACTS, word, startLine);
        	if (word == "Rules" && word.length() == 5) return Token(RULES, word, startLine);
        	if (word == "Queries" && word.length() == 7) return Token(QUERIES, word, startLine);

        	return Token(ID, word, startLine);
		}

		/*
		// Comments
		if (c == '#') {
			int startLine = line;
			string comment;

			// Multi-line comment
			if (input.size() > 1 && input.at(1) == '|') {
				comment = "#|";
				input = input.substr(2);
				bool terminated = false;

				while (!input.empty()) {
					if (input.size() > 1 && input.at(0) == '|' && input.at(1) == '#') {
						comment += "|#";
						input = input.substr(2);
						terminated = true;
						break;
					}

					char ch = input.at(0);
					comment += ch;
					input = input.substr(1);

					if (ch == '\n') line++;
				}

				if (terminated) {
					return Token(COMMENT, comment, startLine);
				}
				else {
					return Token(UNDEFINED, comment, startLine);
				}
			}

			// Single-line comment
			else {
				comment = "#";
				input = input.substr(1);

				while (!input.empty() && input.at(0) != '\n') {
					comment += input.at(0);
					input = input.substr(1);
				}

				return Token(COMMENT, comment, startLine);
			}
		}
		*/

		// Else, UNDEFINED
		input = input.substr(1);
			return Token(UNDEFINED, string(1, c), line);
	}
};
