#include "JobTable.hpp"

JobTable::JobTable() {
  pthread_mutex_init(&mutex, NULL);
}

void JobTable::setMutexLock(bool input) {
  //set mutex lock to input
  if (input) pthread_mutex_lock(&mutex);
  else pthread_mutex_unlock(&mutex);
}
