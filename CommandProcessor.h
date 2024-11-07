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
    string command;  // The command string
    string effect;   // The effect/result of the command

public:
    Command();                        // Default constructor
    Command(const string& cmd);       // Constructor with command input
    Command(const Command& other);    // Copy constructor

    string getCommand() const;        // Getter for the command string
    void saveEffect(const string& eff);  // Sets the effect of the command

    Command& operator=(const Command& other);  // Assignment operator overload
    string getEffect() const;                  // Getter for the effect
    void setEffect(const string& eff);         // Sets the effect

    friend ostream& operator<<(ostream& os, const Command& cmd); // Overloads << operator
};

// CommandProcessor class to process and validate commands
class CommandProcessor {
private:
    vector<Command*> commands;  // List of command pointers
    GameEngine* gameEngine;     // Pointer to the GameEngine for context

protected:
    virtual string readCommand();      // Reads a command from console input
    void storeCommand(Command* cmd);   // Stores a command in the command list

public:
    CommandProcessor(GameEngine* engine = nullptr);  // Constructor with GameEngine pointer
    virtual ~CommandProcessor();                     // Destructor to clean up the command list

    void setGameEngine(GameEngine* engine);          // Sets the GameEngine context for command validation
    Command* getCommand();                           // Retrieves a command and validates it
    bool validateCommand(const Command* cmd) const;  // Validates the command

    friend ostream& operator<<(ostream& os, const CommandProcessor& processor);  // Overloads << operator
};

// FileLineReader class to read commands from a file
class FileLineReader {
private:
    ifstream file;  // Input file stream

public:
    explicit FileLineReader(const string& fileName);  // Constructor with filename
    ~FileLineReader();                                // Destructor to close the file
    string readLine();                                // Reads a line from the file
};

// FileCommandProcessorAdapter class to adapt file reading to command processing
class FileCommandProcessorAdapter : public CommandProcessor {
private:
    FileLineReader* fileReader;  // FileLineReader pointer for reading commands from a file

protected:
    string readCommand() override;  // Reads a command from the file instead of console

public:
    explicit FileCommandProcessorAdapter(GameEngine* engine, const string& fileName);  // Constructor
    ~FileCommandProcessorAdapter() override;                                          // Destructor

    // Delete copy constructor and assignment operator to prevent copying
    FileCommandProcessorAdapter(const FileCommandProcessorAdapter& other) = delete;
    FileCommandProcessorAdapter& operator=(const FileCommandProcessorAdapter& other) = delete;

    friend ostream& operator<<(ostream& os, const FileCommandProcessorAdapter& adapter);  // Overloads << operator
};

#endif // COMMANDPROCESSOR_H
