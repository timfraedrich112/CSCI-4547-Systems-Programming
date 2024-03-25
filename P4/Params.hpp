#ifndef PARAMS_HPP
#define PARAMS_HPP

#include "tools.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

using namespace std;

class Params {
  private:
    int ch, code; //for getopt_long
    //bool switchs
    struct option longOpts[6] = {
                { "debug",      no_argument,       NULL,  'd' },
		{ "verbose",	no_argument,       NULL,  'v' },
		{ "recursive",  no_argument,       NULL,  'R' },
		{ "input",	required_argument, NULL,  'i' },
		{ "output",	required_argument, NULL,  'o' },
		{ NULL,         0,                 NULL,   0  }
	};
  public:
    string pathName;
    string outFileName;
    bool d = 0, v = 0, R = 0, i = 0, o = 0;
    vector<string> words;
    Params() = default;
    Params(int argc, char* const* argv);
    ~Params() = default;
    void print(ostream& out, int argc, char* const* argv);
    void usage(ostream& out);
};

#endif
