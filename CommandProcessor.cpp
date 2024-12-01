#include "CommandProcessor.h"
#include <iostream>
#include <set>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <functional>
#include <map>
#include "Map.h"
#include "GameEngine.h"

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
string Command::getCommandString() const
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
    return "Command: " + command + "\nEffect: " + effect + "\n";
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

// void GameEngine::setCommandProcessor(CommandProcessor *processor) {
//     this->commandProcessor = processor;
// }

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
    getline(cin, cmd);
    return cmd;
}

// Stores the command in the command list and triggers logging
void CommandProcessor::saveCommand(Command *cmd)
{
    commands.push_back(cmd);
    Notify(this); // Part 5: Trigger logging
}

// Retrieves a command from input, validates it, and saves it if valid
void CommandProcessor::getCommand()
{
    string cmdStr = readCommand();
    Command *cmd = new Command(cmdStr);
    if (validateCommand(cmd))
    {
        // Parse the command and argument

        string fullCommand = cmd->getCommandString();
        // fullCommand = fullCommand.substr(fullCommand.find_first_not_of(' '), fullCommand.find_last_not_of(' ') - fullCommand.find_first_not_of(' ') + 1);
        istringstream iss(fullCommand);
        string command;
        string argument;

        iss >> command;
        getline(iss, argument);

        if (!argument.empty())
        {
            // remove spaces at the beginning of the argument
            argument = argument.substr(argument.find_first_not_of(' '));
        }

        // cout << "Debug: in function getCommand, comment receive: command:" << command <<";  argument:"<<argument<< endl;

        // As3-part2: add tournament mode during the game start
        if (command == "tournament")
        {
            handleTournamentCommand(cmd);
        }
        // Check the game state compatibility for each command
        else if (command == "loadmap")
        {
            // cout << "Debug: in function getCommand calling  handleloadmapCommand"<< endl;
            handleloadmapCommand(cmd);
        }
        else if (command == "validatemap")
        {
            // cout << "Debug: in function getCommand calling  handlevalidatemap"<< endl;
            handleValidateMapCommand(cmd);
        }
        else if (command == "addplayer")
        {
            // cout << "Debug: in function getCommand calling  handleaddplayer"<< endl;
            handleAddPlayerCommand(cmd);
        }
        else if (command == "gamestart")
        {
            // cout << "Debug: in function getCommand calling  handleGameStartCommand"<< endl;
            handleGameStartCommand(cmd);
        }

        else if (command == "replay")
        {
            // cout << "Debug: in function getCommand calling  handleReplayCommand"<< endl;
            handleReplayCommand(cmd);
        }
        else if (command == "quit")
        {
            cout << "Quit the game" << endl;
            handleQuitCommand(cmd);
        }
    }
    else
    {
        cmd->setEffect("Error: cannot read command");
        cout << "Debug Error in (getCommand): Invalid command entered: " << cmdStr << endl;
    }

    saveCommand(cmd); // Save the command to the list
}

// Validates if the command matches a list of known valid commands
bool CommandProcessor::validateCommand(const Command *cmd) const
{
    string fullCommand = cmd->getCommandString();
    istringstream iss(fullCommand);
    string command;
    string argument;

    iss >> command;
    getline(iss, argument);

    if (!argument.empty())
    {
        // remove spaces at the beginning of the argument
        argument = argument.substr(argument.find_first_not_of(' '));
    }

    // Define a list of valid commands
    const string validCommands[] = {"loadmap", "validatemap", "addplayer", "gamestart", "tournament", "replay", "quit"};

    // Check if the command is in the list of valid commands
    bool isCommand = false;
    for (const auto &validCmd : validCommands)
    {
        if (command == validCmd)
        {
            isCommand = true;
        }
    }

    // cout << "Debug in function(validateCommand): command receive:" << command <<";argument:"<<argument<< endl;
    // Validate the command based on the current game state
    if (command == "tournament" && gameEngine->getCurrentState() == START)
        return true;
    if (command == "loadmap" && (gameEngine->getCurrentState() == START || gameEngine->getCurrentState() == MAPLODADED))
    {
        return true;
    }
    if (command == "validatemap" && gameEngine->getCurrentState() == MAPLODADED)
        return true;
    if (command == "addplayer" && (gameEngine->getCurrentState() == MAPVALIDATED || gameEngine->getCurrentState() == PLAYERSADDED))
        return true;
    if (command == "gamestart" && gameEngine->getCurrentState() == PLAYERSADDED)
        return true;
    if (command == "replay" && gameEngine->getCurrentState() == WIN)
        return true;
    if (command == "quit" && gameEngine->getCurrentState() == WIN)
        return true;
    cout << "Debug in function(validateCommand): Invalid command full command:" << fullCommand << endl;
    cout << "Debug in function(validateCommand): Invalid command:" << command << ";argument " << argument << endl;
    return false;
}

// Handler for loadmap command - prompts the user for map name and loads it
void CommandProcessor::handleloadmapCommand(Command *command)
{
    istringstream iss(command->getCommandString());
    string commandstring;
    string argument;

    iss >> commandstring;
    getline(iss, argument);

    if (!argument.empty())
    {
        // remove spaces at the beginning of the argument
        argument = argument.substr(argument.find_first_not_of(' '));
    }

    // Load map and set state based on result
    Map objectr;
    gameEngine->selectedMap = objectr.loadMapFromFile(argument);

    bool result = (gameEngine->selectedMap == nullptr);
    if (!result)
    {
        cout << "Map " << argument << " loaded successfully!\n";
        gameEngine->transition(MAPLODADED);

        string effect = "Map loading successfully! Map name:";
        effect.append(argument);
        command->setEffect(effect);
    }
    else
    {
        cout << "Failed to load the map. Please try again.\n";
        command->setEffect("Failed loading the map."); //---------------
    }
}

void CommandProcessor::handleValidateMapCommand(Command *command)
{
    // Parse the command to extract information for potential logging or debugging
    istringstream iss(command->getCommandString()); //---------------

    cout << "Validating the map...\n";

    // Call the validate function on the selected map to check if it meets the required criteria
    bool mapValidated = (gameEngine->selectedMap->validate());

    if (mapValidated)
    {
        // If the map is validated successfully, transition the game state to MAPVALIDATED
        cout << "Map validated successfully!\n";
        gameEngine->transition(MAPVALIDATED);

        string effect = "Map validation successful!"; //---------------
        command->setEffect(effect);                        //---------------
    }
    else
    {
        cout << "Map validation failed. Please load a valid map.\n";
        string effect = "Map validation failed. Please load a valid map."; //---------------
        command->setEffect(effect);                                             //---------------
    }
}

void CommandProcessor::handleAddPlayerCommand(Command *command)
{
    // Parse the command to extract the player name argument
    istringstream iss(command->getCommandString());
    string commandstring;
    string argument;

    // Extract command and argument from the input string
    iss >> commandstring;
    getline(iss, argument);
    string playerName = argument;

    if (!argument.empty())
    {
        // Remove leading spaces from the argument to get the exact player name
        argument = argument.substr(argument.find_first_not_of(' '));
    }

    // Add a new player with the given name to the game engine's player list
    gameEngine->playerList.push_back(new Player(playerName));

    gameEngine->transition(PLAYERSADDED);

    // Display the total number of players in the game
    cout << "Total number of players: " << gameEngine->playerList.size() << endl;

    // Iterate over the player list to display each player's name
    for (Player *player : gameEngine->playerList)
    {
        cout << "Player: " << player->getName() << " \n";
    }

    // -----------------------------------------------------
    string effect = "Player " + playerName + " added successfully. Total players: " + to_string(gameEngine->playerList.size());
    command->setEffect(effect);
    // -----------------------------------------------------
}

void CommandProcessor::handleQuitCommand(Command *command)
{
    // Check if the current state of the game engine is WIN
    if (gameEngine->getCurrentState() == WIN)
    {
        // Output message indicating game is exiting
        cout << "Exiting the game." << endl;

        // -----------------------------------------------------
        string effect = "Game exited successfully.";
        command->setEffect(effect); // Store effect in the command for logging or further use
        // -----------------------------------------------------
        // Exit the program as the game is in the WIN state
        exit(0);
    }
    else
    {
        // Output message indicating that quit command is not allowed in the current state
        cout << "Quit command is only valid in the WIN state." << endl;

        // -----------------------------------------------------
        string effect = "Quit command failed. Current state: " + gameEngine->getCurrentState();
        command->setEffect(effect); // Store the failure effect in the command
        // -----------------------------------------------------
    }
}

void CommandProcessor::handleReplayCommand(Command *command)
{
    cout << "Replay command passed." << endl;
    gameEngine->transition(START);
    gameEngine->resetGame();
    cout << "New game." << endl;
    cout << "Please setup the game again." << endl;
}

void CommandProcessor::handleGameStartCommand(Command *command)
{
    gameEngine->gamestart(*gameEngine);
};

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
FileCommandProcessorAdapter::FileCommandProcessorAdapter(GameEngine *engine, const string &fileName) : CommandProcessor(engine), fileReader(new FileLineReader(fileName)) {}

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

// ---------------------------------Asg3 part2--------------------------------- :
// handleTournamentCommand function
void CommandProcessor::handleTournamentCommand(Command *command)
{
    cout << "Debug: Entering handleTournamentCommand." << endl;
    gameEngine->setTournamentMode(true);
    cout << "Debug: Tournament mode set to true." << endl;

    // Parse the command to extract tournament parameters
    istringstream iss(command->getCommandString());
    string commandstring;
    string argument;

    iss >> commandstring;   // This should capture the "tournament" command
    getline(iss, argument); // This captures the rest of the line as arguments

    if (!argument.empty())
    {
        argument = argument.substr(argument.find_first_not_of(' ')); // Trim leading spaces
    }

    TournamentParameters params = parseTournamentCommand(argument);
    // Output the detailed parameters
    cout << "Debug: Parsed Tournament Parameters:" << endl;
    cout << "  Maps: ";
    for (const auto &map : params.mapFiles)
    {
        cout << map << " ";
    }
    cout << endl;

    cout << "  Player Strategies: ";
    for (const auto &strategy : params.playerStrategies)
    {
        cout << strategy << " ";
    }
    cout << endl;

    cout << "  Number of Games: " << params.numberOfGames << endl;
    cout << "  Max Number of Turns: " << params.maxTurns << endl;

    if (validateTournamentParameters(params))
    {
        gameEngine->startTournament(params);
        string effect = "Tournament started successfully with the following settings: Maps=" +
                        to_string(params.mapFiles.size()) + ", Strategies=" +
                        to_string(params.playerStrategies.size()) + ", Games=" +
                        to_string(params.numberOfGames) + ", Max Turns=" +
                        to_string(params.maxTurns);
        command->setEffect(effect);
        cout << effect << endl;
    }
    else
    {
        cout << "Invalid tournament parameters. Please check the format and try again.\n";
        string effect = "Invalid tournament parameters.";
        command->setEffect(effect);
    }
}
// Parses the tournament command arguments into a TournamentParameters struct
TournamentParameters CommandProcessor::parseTournamentCommand(const string &arguments)
{
    istringstream iss(arguments);
    string token;
    TournamentParameters params;
    params.numberOfGames = 0; // Default initialization
    params.maxTurns = 0;      // Default initialization

    map<string, vector<string> *> flagMap = {
        {"-M", &params.mapFiles},
        {"-P", &params.playerStrategies}};

    map<string, int *> intFlagMap = {
        {"-G", &params.numberOfGames},
        {"-D", &params.maxTurns}};

    string currentFlag;

    while (iss >> token)
    {
        cout << "Processing token: " << token << endl; // Debug output
        if (token[0] == '-')
        { // Check if it's a flag
            currentFlag = token;
            cout << "Current flag: " << currentFlag << endl; // Debug output
        }
        else
        {
            if (!currentFlag.empty() && flagMap.find(currentFlag) != flagMap.end())
            {
                flagMap[currentFlag]->push_back(token);
                cout << "Added to vector under flag " << currentFlag << ": " << token << endl; // Debug output
            }
            else if (!currentFlag.empty() && intFlagMap.find(currentFlag) != intFlagMap.end())
            {
                *(intFlagMap[currentFlag]) = stoi(token);
                cout << "Set integer under flag " << currentFlag << ": " << token << endl; // Debug output
            }
        }
    }

    return params;
}

// validates the parsed tournament parameters to ensure they meet expected constraints
bool CommandProcessor::validateTournamentParameters(const TournamentParameters &params)
{
     // Define valid strategies
    const set<string> validStrategies = {"Aggressive", "Benevolent", "Neutral", "Cheater", "Human", "aggressive", "benevolent", "neutral", "cheater", "human"};

    // Validate the number of maps
    if (params.mapFiles.empty() || params.mapFiles.size() > 5)
    {
        cout << "Error: Invalid number of maps." << endl;
        return false;
    }

    // Validate the player strategies : 1)numbers
    if (params.playerStrategies.size() < 2 || params.playerStrategies.size() > 4)
    {
        cout << "Error: Invalid number of player strategies." << endl;
        return false;
    }
    //2):valid strategies
    for (const auto& strategy : params.playerStrategies) {
        if (validStrategies.find(strategy) == validStrategies.end()) {
            cout << "Invalid player strategy: " << strategy << endl;
            return false;
        }
    }

    // Validate the number of games
    if (params.numberOfGames < 1 || params.numberOfGames > 5)
    {
        cout << "Error: Invalid number of games." << endl;
        return false;
    }

    // Validate the max number of turns
    if (params.maxTurns < 10 || params.maxTurns > 50)
    {
        cout << "Error: Invalid max number of turns." << endl;
        return false;
    }

    return true;
}

// tournament -M USA.map Europe.map -P Aggressive Benevolent Neutral Cheater -G 3 -D 11