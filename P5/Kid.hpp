#ifndef KID_HPP
#define KID_HPP

#include "tools.hpp"
#include "Job.hpp"
#include "JobTable.hpp"
#include "Mom.hpp"

using namespace std;

class Kid {
  private:
    Mood myMood;
    Job currentJob;
    JobTable* jobTable;
    int completedJobsCount = 0;
    int rc, sig;
    sigset_t set;
    struct sigaction sa;
  public:
    string name;
    int earnings = 0;
    vector<Job> myCompetedJobs;
    enum Mood { LAZY, PRISSY, OVER_TIRED, GREEDY, COOPERATIVE }; 
    Kid() = default;
    Kid(string inputName, JobTable* inputTable);
    ~Kid() = default;
    void run();
    void chooseMood();
    int selectJob();
    void pay(int value);
    void print(ostream& out);
};

#endif
