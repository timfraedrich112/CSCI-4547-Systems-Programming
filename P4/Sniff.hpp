#ifndef SNIFF_HPP
#define SNIFF_HPP

#include "tools.hpp"
#include "Params.hpp"
#include "FileID.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>

using namespace std;

class Sniff {
  private:
    Params param;
    string current;
    vector<string> sniffWords;
    vector<FileID> files;
  public:
    string first;
    Sniff() = default;
    Sniff(Params inputParams);
    ~Sniff() = default;
    void run(string first);
    void travel(string inputDir);
    FileID processFile(string inputFile, struct stat fileStat, string inputDir);
    void printFiles(ostream& out);
};

#endif
