#ifndef JOBTABLE_HPP
#define JOBTABLE_HPP

#include "tools.hpp"
#include "Job.hpp"
#include "Mom.hpp"
#include "Kid.hpp"

using namespace std;

class JobTable {
  public:
    int jobCount = 0;
    int jobsBegan = 0;
    vector<Job> table;
    pthread_mutex_t mutex;
    bool quitFlag = false;
    JobTable() = default;
    ~JobTable() = default;
    static void* kidMain(void* inputKid);
    void setMutexLock(bool input);
};

#endif
