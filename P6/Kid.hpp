#pragma once

#ifndef KID_HPP
#define KID_HPP

#include "tools.hpp"
#include "Job.hpp"
#include "JobTable.hpp"

using namespace std;

class Kid {
  private:
    Job currentJob;
    JobTable* jobTable;
    int completedJobsCount = 0;
    int rc, sig;
    sigset_t set;
    static bool workFlag;
    static bool quitFlag;
    static void usr1Handler(int sig) { if (sig == SIGUSR1) Kid::workFlag = true; }
    static void quitHandler(int sig) { if (sig == SIGQUIT) Kid::quitFlag = true; }
  public:
    string name;
    int earnings = 0;
    vector<Job> myCompletedJobs;
    enum Mood { LAZY, PRISSY, OVER_TIRED, GREEDY, COOPERATIVE }; 
    Mood myMood;
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
