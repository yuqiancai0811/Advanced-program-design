#include "LoggingObserver.h"
#include <iostream>
using namespace std; 

//Observer methods
Observer::Observer(){
}; 
Observer::~Observer(){
};

// LogObserver methods
LogObserver::LogObserver(const std::string& filename) {
    logFile.open(filename, std::ios::out | std::ios::app);
    if (!logFile) {
        std::cerr << "Error opening log file!" << std::endl;
    }
}

LogObserver::~LogObserver() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void LogObserver::Update(ILoggable* loggable) {
    if (logFile.is_open()) {
        logFile << loggable->stringToLog() << std::endl;
    }
}

// Subject class methods
Subject::Subject(){
	_observers = new list<Observer*>;
}
Subject::~Subject(){ 
	delete _observers;
}
void Subject::Attach(Observer* o){
  _observers->push_back(o);
};
void Subject::Detach(Observer* o){
  _observers->remove(o);
};
void Subject::Notify(ILoggable* loggable){
  list<Observer *>::iterator i = _observers->begin();
  for (list<Observer*>::iterator i = _observers->begin(); i != _observers->end(); ++i)
	(*i)->Update(loggable); 
};

//ILoggable Method
//The stringToLog() function should be implemented in each class 
//that inherits from ILoggable, tailored to provide meaningful information for logging