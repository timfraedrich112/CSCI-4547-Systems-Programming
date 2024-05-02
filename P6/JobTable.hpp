#pragma once

#ifndef JOBTABLE_HPP
#define JOBTABLE_HPP

#include "tools.hpp"
#include "Job.hpp"

using namespace std;

class JobTable {
  public:
    int jobCount = 0;
    vector<Job> table;
    bool quitFlag = false;
    bool workFlag = false;
    pthread_mutex_t mutex;
    JobTable();
    ~JobTable() = default;
    void setMutexLock(bool input);
};

#endif
