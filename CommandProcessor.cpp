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
    Command() = default;
    Command(const string& cmd) : command(cmd) {}
    Command(const Command& other) : command(other.command), effect(other.effect) {}
    Command& operator=(const Command& other) {
        if (this != &other) {
            command = other.command;
            effect = other.effect;
        }
        return *this;
    }

    string getCommand() const { return command; }
    string getEffect() const { return effect; }
    void setEffect(const string& eff) { effect = eff; }

    friend ostream& operator<<(ostream& os, const Command& cmd) {
        os << "Command: " << cmd.command << ", Effect: " << cmd.effect;
        return os;
    }
};


class CommandProcessor {
private:
    vector<Command*> commands;
    GameEngine* gameEngine;

protected:
    virtual string readCommand() {
        string cmd;
        cout << "Enter command: ";
        getline(cin, cmd);
        return cmd;
    }

    void storeCommand(Command* cmd) {
        commands.push_back(cmd);
    }

public:
    CommandProcessor(GameEngine* engine = nullptr) : gameEngine(engine) {}
    virtual ~CommandProcessor() {
        for (Command* cmd : commands) {
            delete cmd;
        }
    }

    Command* getCommand() {
        string cmdStr = readCommand();
        Command* cmd = new Command(cmdStr);
        if (validateCommand(cmd)) {
            storeCommand(cmd);
            return cmd;
        } else {
            cmd->setEffect("Invalid command.");
            cout << "Invalid command: " << cmdStr << endl;
            return cmd;
        }
    }

    bool validateCommand(const Command* cmd) {

        return !cmd->getCommand().empty();
    }

    friend ostream& operator<<(ostream& os, const CommandProcessor& processor) {
        os << "CommandProcessor with commands: ";
        for (const Command* cmd : processor.commands) {
            os << *cmd << " ";
        }
        return os;
    }
};


class FileLineReader {
private:
    ifstream file;

public:
    explicit FileLineReader(const string& fileName) : file(fileName) {
        if (!file.is_open()) {
            cerr << "Failed to open file: " << fileName << endl;
        }
    }

    ~FileLineReader() {
        if (file.is_open()) {
            file.close();
        }
    }

    string readLine() {
        string line;
        if (getline(file, line)) {
            return line;
        }
        return ""; 
    }
};


class FileCommandProcessorAdapter : public CommandProcessor {
private:
    FileLineReader* fileReader;

protected:
    string readCommand() override {
        string cmd = fileReader->readLine();
        if (cmd.empty()) {
            cout << "End of file or empty line." << endl;
        } else {
            cout << "Read command from file: " << cmd << endl;
        }
        return cmd;
    }

public:
    explicit FileCommandProcessorAdapter(const string& fileName)
        : CommandProcessor(), fileReader(new FileLineReader(fileName)) {}

    ~FileCommandProcessorAdapter() override {
        delete fileReader;
    }

    FileCommandProcessorAdapter(const FileCommandProcessorAdapter& other) = delete;
    FileCommandProcessorAdapter& operator=(const FileCommandProcessorAdapter& other) = delete;

    friend ostream& operator<<(ostream& os, const FileCommandProcessorAdapter& adapter) {
        os << "FileCommandProcessorAdapter with file reader";
        return os;
    }
};

#endif // COMMANDPROCESSOR_H
