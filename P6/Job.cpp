#include "Job.hpp"

Job::Job(int inputNumber) {
  jobNumber = inputNumber;
  q = (rand() % 5) + 1;
  p = (rand() % 5) + 1;
  e = (rand() % 5) + 1;
  value = q * (p + e);
}

void Job::chooseJob(string inputKidName) {
  kidName = inputKidName;
  status = WORKING;
}

void Job::doJob() {
  sleep(q);
}

void Job::announceDone() {
  status = COMPLETE;
  cout << kidName << " finished job " << jobNumber << endl;
}

void Job::print(ostream& out) {
  out << "Job " << jobNumber << endl;
  out << "\tKid: " << kidName << endl;
  out << "\tQ: " << q << endl;
  out << "\tE: " << e << endl;
  out << "\tP: " << p << endl;
  out << "\tValue = $" << value << endl;
  out << "\tStatus: ";
  switch (status) {
    case NOT_STARTED: out << "not started" << endl;
      break;
    case WORKING:     out << "working" << endl;
      break;
    case COMPLETE:    out << "completed" << endl;
  }
  out << "-------" << endl;
}
