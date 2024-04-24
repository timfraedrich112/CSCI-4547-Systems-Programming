#include "Mom.hpp"

Mom::Mom() {
  cout << "Mom says it's time to work!" << endl;
}

//CREATE
void Mom::run() {
  initializeJobTable();
  //create kids and their threads
  for (int i = 0; i < 4; ++i) {
    Kid newKid(kidNames[i], jobTable);
    kids[i] = newKid;
    pthread_create(&kidThreads[i], NULL, JobTable::kidMain, (void*) &kids[i]);
  }
  
  //send start signals
  startTime = time(NULL);
  for (int i = 0; i < 4; ++i) { pthread_kill(kidThreads[i], SIGUSR1); }
  
  //look for finished jobs until work time is over
  double elapsedTime = 0;
  for (;;) {
    sleep(1);
    currentTime = time(NULL);
    elapsedTime = difftime(currentTime, startTime);
    if (elapsedTime < 21) findCompletedJobs();
    else break;
  }
  
  //send quit signals, then join threads
  for (int i = 0; i < 4; ++i) { pthread_kill(kidThreads[i], SIGQUIT); }
  for (int i = 0; i < 4; ++i) { pthread_join(kidThreads[i], NULL); }
  
  //when all kids joined, use completed jobs vector to find how much each kid earned, and pay highest extra $5
  int highestEarnings = 0;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; i < kids[i].myCompletedJobs.size(); ++j) {
      kids[i].pay(kids[i].myCompletedJobs[j].value);
    }
    if (kids[i].earnings > highestEarnings) {
      highestEarner = i;
    }
  }
  kids[highestEarner].pay(5);
  
  //print results
  for (int i = 0; i < 4; ++i) { print(cout); }
  print(cout);
}

void Mom::initializeJobTable() {
  jobTable = new JobTable();
  jobTable->setMutexLock(true);
  
  //create 10 jobs to fill the table
  for (int i = 0; i < 10; ++i) {
    Job newJob;
    jobTable->table.push_back(newJob);
    jobTable->jobCount++;
  }
  
  jobTable->setMutexLock(false);
}

void Mom::findCompletedJobs() {
  jobTable->setMutexLock(true);
  
  //through whole table, if completed job is found
  for (int i = 0; i < 10; ++i) {
    if (jobTable->table[i].status == Job::JobStatus::COMPLETE) {
      //add to completed list and remove from table
      completedJobs.push_back(jobTable->table[i]);
      jobTable->table.erase(jobTable->table.begin() + i);

      //add new job
      Job newJob;
      jobTable->table.push_back(newJob);
      jobTable->jobCount++;
    }
  }
  
  jobTable->setMutexLock(false);
}

void Mom::print(ostream& out) {
  out << "=============================" << endl;
  out << "Time to work is over!" << endl;
  out << "The highest earner is " << kids[highestEarner].name << endl;
  out << "-----------------------------" << endl;
  out << "All jobs completed: " << endl;
  for (int i = 0; i < completedJobs.size(); ++i) {
    completedJobs[i].print(out);
  }
}
