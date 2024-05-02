#include "Mom.hpp"

Mom::Mom() {
  cout << "It's time for chores!" << endl;
}

void Mom::run() {
  initializeJobTable();
  //create kids and their threads
  for (int i = 0; i < 4; ++i) {
    Kid newKid(kidNames[i], jobTable);
    kids[i] = newKid;
    error = pthread_create(&kidThreads[i], NULL, kidMain, (void*) &kids[i]);  if (error) fatal("pthread create");
  }
  
  //send start signals
  startTime = time(NULL);
  for (int i = 0; i < 4; ++i) { error = pthread_kill(kidThreads[i], SIGUSR1); if (error) fatal("pthread kill sigusr1"); }
  
  //look for finished jobs until work time is over
  double elapsedTime = 0;
  for (;;) {
    sleep(1);
    currentTime = time(NULL);
    elapsedTime = difftime(currentTime, startTime);
    cout << "working for " << elapsedTime << endl;
    if (elapsedTime < 21) findCompletedJobs();
    else break;
  }
  
  //send quit signals, then join threads
  for (int i = 0; i < 4; ++i) { error = pthread_kill(kidThreads[i], SIGQUIT); if (error) fatal("pthread kill sigquit"); }
  for (int i = 0; i < 4; ++i) { error = pthread_join(kidThreads[i], NULL);    if (error) fatal("pthread join"); }
  cout << "JOINED" << endl;
  
  //when all kids joined, use completed jobs vector to find how much each kid earned, and pay highest extra $5
  int highestEarnings = 0;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; i < (int)kids[i].myCompletedJobs.size(); ++j) {
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

void* Mom::kidMain(void* inputKid) {
  //run input kid's main
  Kid* kid = (Kid*)inputKid;
  kid->run();
  pthread_exit(NULL);
}

void Mom::initializeJobTable() {
  jobTable = new JobTable();
  jobTable->setMutexLock(true);
  
  //create 10 jobs to fill the table
  for (int i = 0; i < 10; ++i) {
    Job newJob(++jobTable->jobCount);
    jobTable->table.push_back(newJob);
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
      Job newJob(++jobTable->jobCount);
      jobTable->table.push_back(newJob);
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
  for (int i = 0; i < (int)completedJobs.size(); ++i) {
    completedJobs[i].print(out);
  }
}
