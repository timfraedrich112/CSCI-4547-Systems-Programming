#include "tools.hpp"
#include "FileID.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

FileID::FileID(string inputName, mode_t inputType, int inputINode, string inputPath) {
  fileName = inputName;
  fileType = inputType;
  iNode = inputINode;
  pathName = inputPath;
}

void FileID::print(ostream& out) {
  out << "File name: \t" << fileName << endl;
  out << "Pathname: \t" << pathName << endl;
  out << "Type: \t\t";
    if (S_ISDIR(fileType)) out << "Directory" << endl;
    else if (S_ISREG(fileType)) out << "Regular File" << endl;
  out << "iNode number: \t" << iNode << endl;
  out << "Words found: \t";
  for (int i = 0; i < (int)foundWords.size(); i++) { 
    cout << foundWords[i] << " ";
  }
  cout << endl; 
}

void FileID::insertWord(string inputWord) {
  foundWords.push_back(inputWord);
}
