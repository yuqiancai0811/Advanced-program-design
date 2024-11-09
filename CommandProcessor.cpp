#include "CommandProcessor.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// Implementation of Command class constructors and methods

Command::Command() : command(""), effect("") {}

Command::Command(const string &cmd) : command(cmd), effect("") {}

Command::Command(const Command &other) : command(other.command), effect(other.effect) {}

Command &Command::operator=(const Command &other)
{
    if (this != &other)
    {
        command = other.command;
        effect = other.effect;
    }
    return *this;
}

string Command::getCommand() const
{
    return command;
}

string Command::getEffect() const
{
    return effect;
}

void Command::setEffect(const string &eff)
{
    effect = eff;
}

void Command::saveEffect(const string &eff)
{
    effect = eff;
    Notify(this); // Part 5: trigger the writing of the entry in the log file
}

ostream &operator<<(ostream &os, const Command &cmd)
{
    os << "Command: " << cmd.command << " | Effect: " << cmd.effect;
    return os;
}

// Part5: Implementing the stringToLog() function from ILoggable
string Command::stringToLog() const
{
    return "Command: " + command + "\n";
}

// Implementation of CommandProcessor class

CommandProcessor::CommandProcessor(GameEngine *engine) : gameEngine(engine) {}

CommandProcessor::~CommandProcessor()
{
    for (Command *cmd : commands)
    {
        delete cmd;
    }
    commands.clear();
}

void CommandProcessor::setGameEngine(GameEngine *engine)
{
    gameEngine = engine;
}

string CommandProcessor::readCommand()
{
    string cmd;
    cout << "Enter command: ";
    getline(cin, cmd);
    return cmd;
}
void CommandProcessor::saveCommand(Command *cmd)
{
    commands.push_back(cmd);
    Notify(this); // Part 5: trigger the writing of the entry in the log file
}

Command *CommandProcessor::getCommand()
{
    string cmdStr = readCommand();
    Command *cmd = new Command(cmdStr);
    if (validateCommand(cmd))
    {
        saveCommand(cmd);
    }
    else
    {
        cmd->setEffect("Invalid command");
        cout << "Error: Invalid command entered: " << cmdStr << endl;
    }
    return cmd;
}

bool CommandProcessor::validateCommand(const Command *cmd) const
{
    const string validCommands[] = {"loadmap", "validatemap", "addplayer", "gamestart", "replay", "quit"};
    for (const auto &validCmd : validCommands)
    {
        if (cmd->getCommand() == validCmd)
        {
            return true;
        }
    }
    cout << "Invalid command: " << cmd->getCommand() << endl;
    return false;
}

ostream &operator<<(ostream &os, const CommandProcessor &processor)
{
    os << "CommandProcessor containing commands:\n";
    for (const Command *cmd : processor.commands)
    {
        os << *cmd << "\n";
    }
    return os;
}
// Part5: Implementing the stringToLog() function from ILoggable
string CommandProcessor::stringToLog() const
{
    if (commands.empty())
    {
        return "CommandProcessor: No commands processed yet.";
    }

    string log = "CommandProcessor: Commands processed:\n";
    for (const Command *cmd : commands)
    {
        log += " - " + cmd->stringToLog() + "\n";
    }
    return log;
}

// Implementation of FileLineReader class

FileLineReader::FileLineReader(const string &fileName) : file(fileName)
{
    if (!file.is_open())
    {
        cerr << "Error: Unable to open file: " << fileName << endl;
    }
}

FileLineReader::~FileLineReader()
{
    if (file.is_open())
    {
        file.close();
    }
}

string FileLineReader::readLine()
{
    string line;
    if (getline(file, line))
    {
        return line;
    }
    return ""; // Return an empty string if EOF or error
}

// Implementation of FileCommandProcessorAdapter class

FileCommandProcessorAdapter::FileCommandProcessorAdapter(GameEngine *engine, const std::string &fileName)
    : CommandProcessor(engine), fileReader(new FileLineReader(fileName)) {}

FileCommandProcessorAdapter::~FileCommandProcessorAdapter()
{
    delete fileReader;
}

string FileCommandProcessorAdapter::readCommand()
{
    string cmd = fileReader->readLine();
    if (cmd.empty())
    {
        cout << "Note: End of file reached or empty line encountered." << endl;
    }
    else
    {
        cout << "Command read from file: " << cmd << endl;
    }
    return cmd;
}

ostream &operator<<(ostream &os, const FileCommandProcessorAdapter &adapter)
{
    os << "FileCommandProcessorAdapter with attached file reader";
    return os;
}
