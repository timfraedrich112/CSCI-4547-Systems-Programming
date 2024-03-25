#include "tools.hpp"
#include "Sniff.hpp"
#include "FileID.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

Sniff::Sniff(Params inputParams) {
  //initialization
  param = inputParams;
  pathName = param.pathName;
  firstDir = param.pathName;
  for (int i = 0; i < (int)param.words.size(); i++) {
    sniffWords.push_back(param.words[i]);
  }
}

void Sniff::oneDir(string inputDir) {
  //open directory
  if ((dir = opendir(inputDir.c_str())) == NULL) {
    //exit if not found
    cout << "Cannot open directory" << endl;
    exit(1);
  } else { cout << "\ncwd: " << inputDir << endl; }
  
  //read through directory
  while ((dp = readdir(dir)) != NULL) {
    if (param.d) cout << "\nStarting " << dp->d_name << endl;
    //discard first two entries: . and ..
    if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) { if (param.d) cout << "Discarded" << endl; continue; }
    
    //test type of directory entry: discard unless regular file or directory
    struct stat fileStat;
    if (lstat(dp->d_name, &fileStat) == -1) continue;
    
    //if directory or file
    if (S_ISCHR(fileStat.st_mode) || S_ISBLK(fileStat.st_mode) || S_ISFIFO(fileStat.st_mode) || S_ISLNK(fileStat.st_mode) || S_ISSOCK(fileStat.st_mode)) { 
      if (param.d) cout << "Discarded" << endl; 
      continue; 
    } else {
      //if verbatim switch is on, output name
      if (param.v) cout << "Directory read: " << dp->d_name << endl;
      
      //if entry is directory, do nothing (added in program 4)
      if (S_ISDIR(fileStat.st_mode)) { }
      
      //if entry is regular file, call oneFile() to process it
      if (S_ISREG(fileStat.st_mode)) {
        FileID newFileID = oneFile(dp->d_name, fileStat, inputDir);
        if (param.d) newFileID.print(cout);
        
        //if at least one sniff word was found, store in vector of files
        if (newFileID.foundWords.size() != 0) files.push_back(newFileID);
      }
      if (param.d) cout << "Done processing " << dp->d_name << endl;
    }
  }
  //close directory
  closedir(dir);
}

FileID Sniff::oneFile(string inputFile, struct stat fileStat, string inputDir) {
  FileID fileid(inputFile, fileStat.st_mode, fileStat.st_ino, inputDir);
  ifstream file;
  string read, temp;
  bool found;
  
  //open and read file
  file.open(inputFile);
  while (!file.eof()) {
    file >> read;
    
    //remove non-alphabetic characaters
    temp = "";
    for (int i = 0; i < (int)read.size(); i++) {
      if ((read[i] >= 'A' && read[i] <= 'Z') || (read[i] >= 'a' && read[i] <= 'z')) {
        temp = temp + read[i];
      }
    }
    read = temp;
    
    //for every word to be sniffed
    for (int i = 0; i < (int)sniffWords.size(); i++) {
      //is the word we read a sniff word
      if (read == sniffWords[i]) {
        found = true;
        //check if word sniffed has already been found
        for (int j = 0; j < (int)fileid.foundWords.size(); j++) {
          //don't add it if already found
          if (read == fileid.foundWords[j]) found = false;
        }
        //add the word if not already found
        if (found) {fileid.insertWord(read);}
      }
    }
  }
  
  //close and return file
  file.close();
  return fileid;
}

void Sniff::printFiles(ostream& out) {
  if (param.d) cout << "\nSniffed Files:" << endl;
  for (int i = 0; i < (int)files.size(); i++) { files[i].print(out); cout << endl; }
}
