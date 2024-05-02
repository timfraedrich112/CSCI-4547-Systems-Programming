#pragma once

#ifndef MOM_HPP
#define MOM_HPP

#include "tools.hpp"
#include "Job.hpp"
#include "JobTable.hpp"
#include "Kid.hpp"

using namespace std;

class Mom {
  private:
    JobTable* jobTable;
    vector<Job> completedJobs;
    time_t startTime, currentTime;
    int error;
    
    //kids
    Kid kids[4];
    string kidNames[4] = {"Angela", "Bobby", "Carla", "Dave"};
    pthread_t kidThreads[4];
    int highestEarner;
  public:
    Mom();
    ~Mom() = default;
    void run();
    static void* kidMain(void* inputKid);
    void initializeJobTable();
    void findCompletedJobs();
    void print(ostream& out);
};

#endif
