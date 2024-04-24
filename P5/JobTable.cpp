#include "JobTable.hpp"

using namespace std;

JobTable::JobTable() {
  pthread_mutex_init(&mutex, NULL);
}

static void* JobTable::kidMain(void* inputKid) {
  //run input kid's main
  Kid* kid = (Kid*)inputKid;
  kid*.run();
}

void JobTable::setMutexLock(bool input) {
  //set mutex lock to input
  if (input) pthread_mutex_lock(&mutex);
  else pthread_mutex_unlock(&mutex);
}
