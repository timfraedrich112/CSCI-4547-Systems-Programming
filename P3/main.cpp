#include "tools.hpp"
#include "Params.hpp"
#include "FileID.hpp"
#include "Sniff.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>

using namespace std;

int main(int argc, char* argv[]) {
  banner();
  
  Params par(argc, argv);
  Sniff sn(par);
  
  chdir(sn.firstDir.c_str());
  sn.oneDir(sn.firstDir);
  sn.printFiles(cout);
  
  bye();
  return 0;
}
