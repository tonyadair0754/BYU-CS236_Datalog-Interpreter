#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "token.h"
#include "scanner.h"
#include "parser.h"
#include "interpreter.h"
#include "graph.h"

using namespace std;

int main(int argc, char* argv[]) {

	// Read in file/text
	string filename = argv[1];
	string input;

	if (argc == 1) { // No file specified, read from stdin
		stringstream ss;
		ss << cin.rdbuf();
		input = ss.str();
	}
	else { // File path provided as argument
		ifstream in(argv[1]);

		if (in.is_open()) {
			stringstream ss;
			ss << in.rdbuf();
			input = ss.str();
			in.close();
		} else {
			// If argument is not a valid file path, treat it as direct input string
			input = argv[1];
		}

	}

	Scanner s(input);
	vector<Token> tokens = s.scanTokens();

	Parser p(tokens);

	DatalogProgram program = p.parse();

	Interpreter interpreter(program);
	interpreter.interpret();

}
