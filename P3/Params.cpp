#include "tools.hpp"
#include "Params.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

using namespace std;

Params::Params(int argc, char* const* argv) {
  //read words
  for (int i = 0; i < argc; i++) {
      if (i == 0) continue;
      else if (argv[i][0] == '-') break;
      else words.push_back(argv[i]);
  }

  //check switches
  ofstream outFile;
  for (;;) {
    ch = getopt_long(argc, argv, "dvRi:o:",longOpts, &code);
    if (ch == -1) break;
    switch(ch) {
      case 'd': d = 1;
        break;
      case 'v': v = 1;
        break;
      case 'R': R = 1;
        break;
      case 'i': i = 1;
        pathName = optarg;
        break;
      case 'o': o = 1;
        outFileName = optarg;
        outFile.open(outFileName);
        break;
    }
  }
  
  //print and close
  usage(cout);
  if (o == 1) print(outFile, argc, argv); else print(cout, argc, argv);
  outFile.close();
}

void Params::print(ostream& out, int argc, char* const* argv) {
  out << "\nCommand: \t";
  for (int i = 0; i < argc; i++) { out << argv[i] << " "; }
  out << "\nDebug: \t\t";
  if (d==1) out << "True" << endl; else out << "False" << endl;
  out << "Verbose: \t";
  if (v==1) out << "True" << endl; else out << "False" << endl;
  out << "Recursive: \t";
  if (R==1) out << "True" << endl; else out << "False" << endl;
  out << "Input: \t\t";
  if (i==1) out << "True, arg: " << pathName << endl; else out << "False" << endl;
  out << "Output: \t";
  if (o==1) out << "True, arg: " << outFileName << endl; else out << "False" << endl;
  out << "Words: \t\t";
  for (int i = 0; i < (int)words.size(); i++) { out << words[i] << " "; }
  out << endl;
}

void Params::usage(ostream& out) {
  out << "\nusage: arguments [strings: words to sniff], options [-dvR] [-i filename] [-o filename]" << endl;
}
