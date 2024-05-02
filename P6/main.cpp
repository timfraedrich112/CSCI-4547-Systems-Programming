#include "tools.hpp"
#include "Mom.hpp"

using namespace std;

int main(int argc, char* argv[]) {
  banner();
  
  srand(time(NULL));
  Mom m;
  m.run();

  bye();
  return 0;
}
