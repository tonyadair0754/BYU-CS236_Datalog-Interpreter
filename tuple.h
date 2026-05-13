#pragma once
#include <vector>
#include <sstream>
#include <string>
#include <set>

using namespace std;

class Tuple : public vector<string> {

public:

	Tuple(vector<string> values) : vector<string>(values) { }

	string toString(const Scheme& scheme) const
	{
		stringstream out;

		for (size_t i = 0; i < size(); i++) {
			if (i > 0) out << ", ";
			out << scheme.at(i) << "=" << at(i);
		}

		return out.str();
	}
};
