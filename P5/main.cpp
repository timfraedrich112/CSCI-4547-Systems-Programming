#include "tools.hpp"
#include "Mom.hpp"
#include "Kid.hpp"
#include "Job.hpp"
#include "JobTable.hpp"

using namespace std;

int main(int argc, char* argv[]) {
  banner();
  
  Mom m;
  m.run();

  bye();
  return 0;
}
