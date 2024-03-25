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
    string pathName;
    vector<string> sniffWords;
    vector<FileID> files;
    DIR *dir;
    struct dirent *dp;
  public:
    string firstDir;
    Sniff() = default;
    Sniff(Params inputParams);
    ~Sniff() = default;
    void oneDir(string inputDir);
    FileID oneFile(string inputFile, struct stat fileStat, string inputDir);
    void printFiles(ostream& out);
};

#endif
