#ifndef FILEID_HPP
#define FILEID_HPP

#include "tools.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

class FileID {
  private:
    string fileName;
    int iNode;
    string pathName;
    mode_t fileType;
  public:
    vector<string> foundWords;
    FileID() = default;
    FileID(string inputName, mode_t inputType, int inputINode, string inputPath);
    ~FileID() = default;
    void print(ostream& out);
    void insertWord(string inputWord);
};

#endif
