#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H

#include "GameEngine.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class Command {
private:
    string command;
    string effect;

public:
    Command();
    Command(const string& cmd);
    Command(const Command& other);
    Command& operator=(const Command& other);

    string getCommand() const;
    string getEffect() const;
    void setEffect(const string& eff);

    friend ostream& operator<<(ostream& os, const Command& cmd);
};

class CommandProcessor {
private:
    vector<Command*> commands;
    GameEngine* gameEngine;

protected:
    virtual string readCommand();

    void storeCommand(Command* cmd);

public:
    CommandProcessor(GameEngine* engine = nullptr);
    virtual ~CommandProcessor();

    Command* getCommand();
    bool validateCommand(const Command* cmd);

    friend ostream& operator<<(ostream& os, const CommandProcessor& processor);
};

class FileLineReader {
private:
    ifstream file;

public:
    explicit FileLineReader(const string& fileName);
    ~FileLineReader();

    string readLine();
};

class FileCommandProcessorAdapter : public CommandProcessor {
private:
    FileLineReader* fileReader;

protected:
    string readCommand() override;

public:
    explicit FileCommandProcessorAdapter(const string& fileName);
    ~FileCommandProcessorAdapter() override;

    FileCommandProcessorAdapter(const FileCommandProcessorAdapter& other) = delete;
    FileCommandProcessorAdapter& operator=(const FileCommandProcessorAdapter& other) = delete;

    friend ostream& operator<<(ostream& os, const FileCommandProcessorAdapter& adapter);
};

#endif // COMMANDPROCESSOR_H
