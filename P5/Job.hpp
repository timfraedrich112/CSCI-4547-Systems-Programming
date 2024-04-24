#ifndef JOB_HPP
#define JOB_HPP

#include "tools.hpp"
#include "JobTable.hpp"
#include "Mom.hpp"
#include "Kid.hpp"

using namespace std;

class Job {
  private:
    int jobNumber;
    string kidName;
  public:
    short int q, p, e;
    int value;
    enum JobStatus { NOT_STARTED, WORKING, COMPLETE };
    JobStatus status;
    Job() = default;
    ~Job() = default;
    void chooseJob(string inputKidName, int inputJobNumber);
    void doJob();
    void announceDone();
    void print(ostream& out);
};

#endif