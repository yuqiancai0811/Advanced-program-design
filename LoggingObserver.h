#ifndef LOGGING_OBSERVER_H
#define LOGGING_OBSERVER_H

#include <string>
#include <fstream>
#include <iostream>
#include <list>
using namespace std;

// Iloggable class
class ILoggable
{
public:
    virtual string stringToLog() const = 0;
};
// Observer Class
class Observer
{
public:
    ~Observer();
    virtual void Update(ILoggable *loggable) = 0;

protected:
    Observer();
};

// LogObserver class for writing to a log file
class LogObserver : public Observer
{
public:
    LogObserver(const string &filename);
    ~LogObserver();
    void Update(ILoggable *loggable) override;

private:
    ofstream logFile;
};

// Subject class
class Subject 
{
private:
    list<Observer *> *_observers;
public:
    Subject();
    ~Subject();
    virtual void Attach(Observer *o);
    virtual void Detach(Observer *o);
    void Notify(ILoggable *loggable);
};
#endif // LOGGING_OBSERVER_H
