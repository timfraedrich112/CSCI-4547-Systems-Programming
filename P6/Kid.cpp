#include "Kid.hpp"

Kid::Kid(string inputName, JobTable* inputTable) {
  //sets inputs
  name = inputName;
  jobTable = inputTable;
  Kid::workFlag = false;
  Kid::quitFlag = false;
  
  //create signal set
  sigemptyset(&set);
  sigaddset(&set, SIGUSR1);
  sigaddset(&set, SIGQUIT);
  rc = pthread_sigmask(SIG_BLOCK, &set, NULL);      if (rc) { fatal("sigmask"); }

  //create signal actions
  struct sigaction usr1SA;
  usr1SA.sa_mask = set;
  usr1SA.sa_flags = 0;
  usr1SA.sa_handler = Kid::usr1Handler;
  rc = sigaction(SIGUSR1, &usr1SA, NULL);           if (rc) { fatal("sigaction usr1"); }
  struct sigaction quitSA;
  quitSA.sa_mask = set;
  quitSA.sa_flags = 0;
  quitSA.sa_handler = Kid::quitHandler;
  rc = sigaction(SIGQUIT, &quitSA, NULL);           if (rc) { fatal("sigaction quit"); }
}

void Kid::run() {
  rc = sigwait(&set, &sig);                         if (rc) { perror("sigwait"); exit(1); }
  //enter work loop
  chooseMood();
  cout << name << " starting, " << endl;
  
  for (;;) {
    if (Kid::quitFlag) {
      cout << "quit signal received" << endl;
      break;
    }
    else if (Kid::workFlag) {
      cout << "WORKING" << endl;
      //select job
      jobTable->setMutexLock(true);
      
      int jobToSelect = selectJob();
      jobTable->table[jobToSelect].chooseJob(name);
      currentJob = jobTable->table[jobToSelect];
      currentJob.print(cout);
      
      jobTable->setMutexLock(false);
      
      //do job, then announce done
      currentJob.doJob();
      currentJob.announceDone();
      myCompletedJobs.push_back(currentJob);
    }
  }
}

void Kid::chooseMood() {
  //randomly select mood
  myMood = static_cast<Mood>(rand() % 5);
}

int Kid::selectJob() {
  int jobToSelect;
  int comparator = 100;
  int valueCompare = 0;
  //loop through all jobs on table
  for (int i = 0; i < 10; ++i) {
    switch (myMood) {
      case LAZY:
        //find easiest job
        if (jobTable->table[i].e < comparator) {
          jobToSelect = i;
          comparator = jobTable->table[i].e;
        }
        break;
      case PRISSY:
        //find most pleasant job
        if (jobTable->table[i].p < comparator) {
          jobToSelect = i;
          comparator = jobTable->table[i].p;
        }
        break;
      case OVER_TIRED:
        //find shortest job
        if (jobTable->table[i].q < comparator) {
          jobToSelect = i;
          comparator = jobTable->table[i].q;
        }
        break;
      case GREEDY:
        //find job with highest value
        if (jobTable->table[i].value > valueCompare) {
           jobToSelect = i;
          valueCompare = jobTable->table[i].e;
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
  for (int i = 0; i < (int)myCompletedJobs.size(); ++i) {
    myCompletedJobs[i].print(out);
  }
}
