#pragma once

#ifndef JOB_HPP
#define JOB_HPP

#include "tools.hpp"

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
    Job(int inputNumber);
    ~Job() = default;
    void chooseJob(string inputKidName);
    void doJob();
    void announceDone();
    void print(ostream& out);
};

#endif
