#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H

#include "GameEngine.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// Command class representing a command and its effect
class Command {
private:
    string command;
    string effect;

public:
    Command() = default;
    explicit Command(const string& cmd) : command(cmd) {}
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
        os << "Command: " << cmd.command << " | Effect: " << cmd.effect;
        return os;
    }
};

// CommandProcessor class for reading and processing commands
class CommandProcessor {
private:
    vector<Command*> commands;
    GameEngine* gameEngine;

protected:
    // Reads a command from the user input
    virtual string readCommand() {
        cout << "Enter command: ";
        string cmd;
        getline(cin, cmd);
        return cmd;
    }

    // Stores a command object in the commands list
    void storeCommand(Command* cmd) {
        commands.push_back(cmd);
    }

public:
    explicit CommandProcessor(GameEngine* engine = nullptr) : gameEngine(engine) {}
    virtual ~CommandProcessor() {
        for (Command* cmd : commands) {
            delete cmd;
        }
    }

    // Retrieves a command and validates it
    Command* getCommand() {
        string cmdStr = readCommand();
        Command* cmd = new Command(cmdStr);
        if (validateCommand(cmd)) {
            storeCommand(cmd);
        } else {
            cmd->setEffect("Invalid command");
            cout << "Error: Invalid command entered: " << cmdStr << endl;
        }
        return cmd;
    }

    // Validates the command
    bool validateCommand(const Command* cmd) const {
        return !cmd->getCommand().empty();  // Simple validation; can be extended
    }

    friend ostream& operator<<(ostream& os, const CommandProcessor& processor) {
        os << "CommandProcessor containing commands:\n";
        for (const Command* cmd : processor.commands) {
            os << *cmd << "\n";
        }
        return os;
    }
};

// FileLineReader class for reading commands from a file line-by-line
class FileLineReader {
private:
    ifstream file;

public:
    explicit FileLineReader(const string& fileName) : file(fileName) {
        if (!file.is_open()) {
            cerr << "Error: Unable to open file: " << fileName << endl;
        }
    }

    ~FileLineReader() {
        if (file.is_open()) {
            file.close();
        }
    }

    // Reads a single line from the file
    string readLine() {
        string line;
        if (getline(file, line)) {
            return line;
        }
        return "";  // Returns an empty string if EOF or error
    }
};

// FileCommandProcessorAdapter class adapts FileLineReader to CommandProcessor
class FileCommandProcessorAdapter : public CommandProcessor {
private:
    FileLineReader* fileReader;

protected:
    // Overrides readCommand to read from a file instead of console
    string readCommand() override {
        string cmd = fileReader->readLine();
        if (cmd.empty()) {
            cout << "Note: End of file reached or empty line encountered." << endl;
        } else {
            cout << "Command read from file: " << cmd << endl;
        }
        return cmd;
    }

public:
    explicit FileCommandProcessorAdapter(const string& fileName)
        : CommandProcessor(), fileReader(new FileLineReader(fileName)) {}

    ~FileCommandProcessorAdapter() override {
        delete fileReader;
    }

    // Disables copy constructor and assignment operator to prevent copying
    FileCommandProcessorAdapter(const FileCommandProcessorAdapter& other) = delete;
    FileCommandProcessorAdapter& operator=(const FileCommandProcessorAdapter& other) = delete;

    friend ostream& operator<<(ostream& os, const FileCommandProcessorAdapter& adapter) {
        os << "FileCommandProcessorAdapter with attached file reader";
        return os;
    }
};

#endif // COMMANDPROCESSOR_H
