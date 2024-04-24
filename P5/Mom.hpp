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
    
    //kids
    Kid kids[4];
    const string kidNames[4] = {"Angela", "Bobby", "Carla", "Dave"};
    pthread_t kidThreads[4];
    int highestEarner;
  public:
    Mom() = default;
    ~Mom() = default;
    void run();
    void initializeJobTable();
    void findCompletedJobs();
    void print(ostream& out);
};

#endif
