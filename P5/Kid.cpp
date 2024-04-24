#include "Kid.hpp"

using namespace std;

Kid::Kid(string inputName, JobTable* inputTable) {
  //sets inputs
  name = inputName;
  jobTable = inputTable;
  
  //create signal set
  sigemptyset(&set);
  sigaddset(&set, SIGQUIT);
  sigaddset(&set, SIGUSR1);
  rc = pthread_sigmask(SIG_BLOCK, &set, NULL);
  if (rc) { perror("sigmask"); exit(1); }
}

void Kid::run() {
  //create signal actions
  sa.sa_mask = set;
  chooseMood();
  if (sigaction(SIGUSR1, &sa, NULL) < 0) { perror("sigaction"); exit(1); }
  rc = sigwait(&set, &sig);
  if (rc) { perror("sigwait"); exit(1); }
  
  //enter work loop
  for (;;) {
    if (sig == SIGUSR1) {
      //select job
      jobTable.setMutexLock(true);
      
      int jobToSelect = selectJob()
      jobTable.table[jobToSelect].chooseJob(name, ++jobTable.jobsBegan);
      currentJob = jobTable.table[jobToSelect];
      
      jobTable.setMutexLock(false);
      
      //do job, then announce done
      currentJob.doJob();
      currentJob.announceDone();
      myCompetedJobs.push_back(currentJob);
    }
    else if (sig == SIGQUIT) { //MIGHT NEED REWORK, DOESNT USE JOBTABLE BOOL
      //quit work look
      jobTable.quitFlag = true;
      cout << "quit signal received" << endl;
      break;
    }
  }
}

void Kid::chooseMood() {
  //randomly select mood
  srand(time(NULL));
  myMood = static_cast<Mood>(rand() % 5);
}

int Kid::selectJob() {
  int jobToSelect;
  int comparator = 100;
  int valueCompare = 0;
  //loop through all jobs on table
  for (int i = 0; i < 10; ++i) {
    switch (mood) {
      case LAZY:
        //find easiest job
        if (jobTable.table[i].e < comparator) {
          jobToSelect = i;
          comparator = jobTable.table[i].e;
        }
        break;
      case PRISSY:
        //find most pleasant job
        if (jobTable.table[i].p < comparator) {
          jobToSelect = i;
          comparator = jobTable.table[i].p;
        }
        break;
      case OVER_TIRED:
        //find shortest job
        if (jobTable.table[i].q < comparator) {
          jobToSelect = i;
          comparator = jobTable.table[i].q;
        }
        break;
      case GREEDY:
        //find job with highest value
        if (jobTable.table[i].value > valueCompare) {
           jobToSelect = i;
          valueCompare = jobTable.table[i].e;
        }
        break;
      case COOPERATIVE:
        //always take last job
        jobToSelect = 9;
        break;
    }
  }
  return jobToSelect;
}

void Kid::pay(int value) {
  //adds payment to total earnings
  earnings += value;
}

void Kid::print(ostream& out) {
  out << "My name is " << name << ", my mood is ";
  switch (myMood) {
    case LAZY:        out << "lazy, ";          break;
    case PRISSY:      out << "prissy, ";        break;
    case OVER_TIRED:  out << "over tired, ";    break;
    case GREEDY:      out << "greedy, ";        break;
    case COOPERATIVE: out << "cooperative, ";   break;
    default: out << "THIS DIDN'T WORK";
  }
  out << ", and I have completed " << completedJobsCount << " jobs, earning $" << earnings << endl;
  for (int i = 0; i < myCompetedJobs.Count; ++i) {
    myCompetedJobs[i].print(out);
  }
}
