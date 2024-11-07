#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H

#include "GameEngine.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// Command class to encapsulate command data and effects
class Command {
private:
    string command;
    string effect;

public:
    Command() : command(""), effect("") {}
    Command(const string& cmd) : command(cmd), effect("") {}
    Command(const Command& other) : command(other.command), effect(other.effect) {}

    std::string getCommand() const { return command; }  
    void saveEffect(const std::string& eff) { effect = eff; } 

    Command& operator=(const Command& other) {
        if (this != &other) {
            command = other.command;
            effect = other.effect;
        }
        return *this;
    }

    string getEffect() const { return effect; }  
    void setEffect(const string& eff) { effect = eff; }

    friend ostream& operator<<(ostream& os, const Command& cmd) {
        os << "Command: " << cmd.command << ", Effect: " << cmd.effect;
        return os;
    }
};

// CommandProcessor class to process and validate commands
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


    void storeCommand(Command* cmd) { commands.push_back(cmd); }

public:
    CommandProcessor();
    void setGameEngine(GameEngine* engine);//

    CommandProcessor(GameEngine* engine = nullptr) : gameEngine(engine) {}
    virtual ~CommandProcessor() {
        for (auto cmd : commands) {
            delete cmd;
        }
        commands.clear();
    }

 
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

 
    bool validateCommand(const Command* cmd) const {
        const string validCommands[] = {"loadmap", "validatemap", "addplayer", "gamestart", "replay", "quit"};
        for (const auto& validCmd : validCommands) {
            if (cmd->getCommand() == validCmd) {
                return true;
            }
        }
        cout << "Invalid command: " << cmd->getCommand() << endl;
        return false;
    }

    friend ostream& operator<<(ostream& os, const CommandProcessor& processor) {
        os << "CommandProcessor with " << processor.commands.size() << " commands.";
        return os;
    }
};

// FileLineReader class to read commands from a file
class FileLineReader {
private:
    ifstream file;

public:
    explicit FileLineReader(const string& fileName) : file(fileName) {
        if (!file.is_open()) {
            cerr << "Error: Cannot open file " << fileName << endl;
        }
    }

    ~FileLineReader() {
        if (file.is_open()) {
            file.close();
        }
    }

    // Reads a line from the file
    string readLine() {
        string line;
        if (getline(file, line)) {
            return line;
        }
        return "";  // Return an empty string if end of file or error
    }
};

// FileCommandProcessorAdapter class to adapt file reading to command processing
class FileCommandProcessorAdapter : public CommandProcessor {
private:
    FileLineReader* fileReader;

protected:
    // Reads a command from the file
    string readCommand() override {
        if (fileReader) {
            return fileReader->readLine();
        }
        return "";
    }

public:
     explicit FileCommandProcessorAdapter(GameEngine* engine, const string& fileName)
    : CommandProcessor(engine), fileReader(new FileLineReader(fileName)) {}


    ~FileCommandProcessorAdapter() override {
        delete fileReader;
        fileReader = nullptr;
    }

    // Delete copy constructor and assignment operator to prevent copying
    FileCommandProcessorAdapter(const FileCommandProcessorAdapter& other) = delete;
    FileCommandProcessorAdapter& operator=(const FileCommandProcessorAdapter& other) = delete;

    friend ostream& operator<<(ostream& os, const FileCommandProcessorAdapter& adapter) {
        os << "FileCommandProcessorAdapter reading from file.";
        return os;
    }
};

#endif // COMMANDPROCESSOR_H
