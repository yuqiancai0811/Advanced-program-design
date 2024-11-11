#include "CommandProcessor.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <functional>
#include <map>
#include "Map.h"

using namespace std;

// Implementation of Command class constructors and methods

// Default constructor initializes command and effect as empty strings
Command::Command() : command(""), effect("") {}

// Constructor that sets the command string with a given input
Command::Command(const string &cmd) : command(cmd), effect("") {}

// Copy constructor to initialize a Command object from another Command object
Command::Command(const Command &other) : command(other.command), effect(other.effect) {}

// Assignment operator to copy data from another Command object
Command &Command::operator=(const Command &other)
{
    if (this != &other)
    {
        command = other.command;
        effect = other.effect;
    }
    return *this;
}

// Getter for the command string
string Command::getCommand() const
{
    return command;
}

// Getter for the effect string
string Command::getEffect() const
{
    return effect;
}

// Setter to set the effect string
void Command::setEffect(const string &eff)
{
    effect = eff;
}

// Method to save the effect and trigger logging
void Command::saveEffect(const string &eff)
{
    effect = eff;
    Notify(this); // Part 5: Trigger logging through the observer pattern
}

// Overloading the << operator to print Command details
ostream &operator<<(ostream &os, const Command &cmd)
{
    os << "Command: " << cmd.command << " | Effect: " << cmd.effect;
    return os;
}

// Part 5: Implement the stringToLog() function for logging command details
string Command::stringToLog() const
{
    return "Command: " + command + "\n";
}

// Implementation of CommandProcessor class

// Constructor that sets the associated GameEngine instance
CommandProcessor::CommandProcessor(GameEngine *engine) : gameEngine(engine) {}

// Destructor to clean up command pointers stored in the command list
CommandProcessor::~CommandProcessor()
{
    for (Command *cmd : commands)
    {
        delete cmd;
    }
    commands.clear();
}

// Setter to assign the GameEngine instance
void CommandProcessor::setGameEngine(GameEngine *engine)
{
    gameEngine = engine;
}

// Reads a command from user input
string CommandProcessor::readCommand()
{
    string cmd;
    cout << "Enter command: ";
    cin>>cmd;
    return cmd;
}

// Stores the command in the command list and triggers logging
void CommandProcessor::saveCommand(Command *cmd)
{
    commands.push_back(cmd);
    Notify(this); // Part 5: Trigger logging
}

// Retrieves a command from input, validates it, and saves it if valid
Command *CommandProcessor::getCommand()
{
    string cmdStr = readCommand();
    Command *cmd = new Command(cmdStr);
    if (validateCommand(cmd))
    {   

    std::istringstream iss(cmd->getCommand());
    std::string command;
    std::string argument;

    iss >> command;
    std::getline(iss, argument);

        if (!argument.empty()) {
                // remove space;
                argument = argument.substr(argument.find_first_not_of(' '));
        }

        saveCommand(cmd);
        if(command=="loadmap"){
            if(gameEngine->getCurrentState()=="start" || gameEngine->getCurrentState()=="maploaded");
        }
        else if(command=="validatemap"){
            if(gameEngine->getCurrentState()=="maploaded");
        }
        else if(command=="addplayer"){
            if(gameEngine->getCurrentState()=="mapvalidated" || gameEngine->getCurrentState()=="playersadded");
        }
        else if(command=="gamestart"){
            if(gameEngine->getCurrentState()=="playersadded" );
        }
        else if(command=="replay"){
            if(gameEngine->getCurrentState()=="win" );
        }
        else if(command=="quit"){
            if(gameEngine->getCurrentState()=="win" );  
        }

    }
    else
        {
        cmd->setEffect("Error: can not read command");
        cout << "Error: Invalid command entered: " << cmdStr << endl;
        }

    return cmd;
}

// Validates if the command matches a list of known valid commands
bool CommandProcessor::validateCommand(const Command *cmd) const
{   
    std::istringstream iss(cmd->getCommand());
    std::string command;
    std::string argument;

    iss >> command;
    std::getline(iss, argument);

        if (!argument.empty()) {
            // remove space;
            argument = argument.substr(argument.find_first_not_of(' '));
        }

    const string validCommands[] = {"loadmap", "validatemap", "addplayer", "gamestart", "replay", "quit"};
    
    //check if the command is A command or not
    bool isCommand=false;
    for (const auto &validCmd : validCommands)
    {
        if (cmd->getCommand() == validCmd)
        {
            isCommand=true;
        }
    }

    //check the  Validation with game state
             if(command=="loadmap"){
             if(gameEngine->getCurrentState()=="start" || gameEngine->getCurrentState()=="maploaded") return true;
        }
        else if(command=="validatemap"){
            if(gameEngine->getCurrentState()=="maploaded") return true;
        }
        else if(command=="addplayer"){
            if(gameEngine->getCurrentState()=="mapvalidated" || gameEngine->getCurrentState()=="playersadded") return true;
        }
        else if(command=="gamestart"){
            if(gameEngine->getCurrentState()=="playersadded" ) return true;
        }
        else if(command=="replay"){
            if(gameEngine->getCurrentState()=="win" ) return true;
        }
        else if(command=="quit"){
            if(gameEngine->getCurrentState()=="win" ) return true;  
        }

    cout << "Invalid command: " << cmd->getCommand() << endl;
    return false;

}

void CommandProcessor::handleloadmapCommand(Command* Command){
    std::istringstream iss(Command->getCommand());
    std::string command;
    std::string argument;

    iss >> command;
    std::getline(iss, argument);

        if (!argument.empty()) {
            // remove space;
            argument = argument.substr(argument.find_first_not_of(' '));
        }
    
    cout << "Please select the name of the map you want to load: ";
        string mapName;
        cin >> mapName;

        *gameEngine->selectedMap = *gameEngine->selectedMap->loadMapFromFile(mapName);
        
        bool result =  (gameEngine->selectedMap == nullptr);
        if (!result) {
            cout << "Map " << mapName << " loaded successfully!\n";
            gameEngine->setcurrentState(MAPLODADED);
        } else {
            cout << "Failed to load the map. Please try again.\n";
        }

}

// Overloading the << operator to print details of the CommandProcessor
ostream &operator<<(ostream &os, const CommandProcessor &processor)
{
    os << "CommandProcessor containing commands:\n";
    for (const Command *cmd : processor.commands)
    {
        os << *cmd << "\n";
    }
    return os;
}

// Part 5: Implement the stringToLog() function for logging processed commands
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

// Constructor that opens a file for reading
FileLineReader::FileLineReader(const string &fileName) : file(fileName)
{
    if (!file.is_open())
    {
        cerr << "Error: Unable to open file: " << fileName << endl;
    }
}

// Destructor that closes the file if open
FileLineReader::~FileLineReader()
{
    if (file.is_open())
    {
        file.close();
    }
}

// Reads a line from the file and returns it
string FileLineReader::readLine()
{
    string line;
    if (getline(file, line))
    {
        return line;
    }
    return ""; // Return an empty string if end of file or error
}

// Implementation of FileCommandProcessorAdapter class

// Constructor that initializes the CommandProcessor with a file reader for reading commands from a file
FileCommandProcessorAdapter::FileCommandProcessorAdapter(GameEngine *engine, const std::string &fileName)
    : CommandProcessor(engine), fileReader(new FileLineReader(fileName)) {}

// Destructor to clean up the file reader
FileCommandProcessorAdapter::~FileCommandProcessorAdapter()
{
    delete fileReader;
}

// Overrides readCommand to read commands from the file instead of console
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

// Overloading the << operator to print FileCommandProcessorAdapter details
ostream &operator<<(ostream &os, const FileCommandProcessorAdapter &adapter)
{
    os << "FileCommandProcessorAdapter with attached file reader";
    return os;
}
