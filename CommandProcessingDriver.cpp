#include "CommandProcessingDriver.h"
#include "GameEngine.h"
#include "CommandProcessor.h"

#include <iostream>
using std::cout;
using std::endl;

//{"start", "loadmap", "validatemap", "addplayer", "assignReinforcement", "issueOrders", "executeOrders", "win"};
// Test function to simulate command processing from user input
void testCommandProcessor() {
    // Initialize the GameEngine and CommandProcessor with the initial game state
    GameEngine* gameEngine = new GameEngine();
    CommandProcessor* processor = new CommandProcessor(gameEngine);

    // Link the GameEngine and CommandProcessor
    gameEngine->setCommandProcessor(processor);
    processor->setGameEngine(gameEngine);

    bool continueGame = true;

    while (continueGame) {
        // Transition to the START state and prompt user for commands
        gameEngine->transition(START);
        cout << "Current State: START" << endl;
        Command* command = processor->getCommand();

        // Validate the command; prompt user until a valid command is entered
        while (!processor->validateCommand(command)) {
            cout << "Invalid command. Please try again." << endl;
            command = processor->getCommand();
        }
        command->saveEffect("Map loaded.");  // Save the command effect
        cout << "Effect: " << command->getEffect() << endl;

        // Transition to MAP_LOADED state and process commands
        gameEngine->transition(MAPLODADED);
        cout << "Current State: MAP_LOADED" << endl;
        command = processor->getCommand();

        // Validate command and check for "validatemap"; prompt user if invalid
        while (!processor->validateCommand(command) || command->getCommand() != "validatemap") {
            if (processor->validateCommand(command) && command->getCommand() != "validatemap") {
                command->saveEffect("Map loaded.");
            }
            cout << "Invalid command. Please try again." << endl;
            command = processor->getCommand();
        }
        command->saveEffect("Map validated.");  // Set command effect for valid input
        cout << "Effect: " << command->getEffect() << endl;

        // Transition to MAP_VALIDATED state and process commands
        gameEngine->transition(MAPVALIDATED);
        cout << "Current State: MAP_VALIDATED" << endl;
        command = processor->getCommand();

        // Validate command in MAP_VALIDATED state
        while (!processor->validateCommand(command)) {
            cout << "Invalid command. Please try again." << endl;
            command = processor->getCommand();
        }
        command->saveEffect("Player added.");  // Record effect of command
        cout << "Effect: " << command->getEffect() << endl;

        // Transition to PLAYERS_ADDED state and process "gamestart" command
        gameEngine->transition(PLAYERSADDED);
        cout << "Current State: PLAYERS_ADDED" << endl;
        command = processor->getCommand();

        // Validate command; ensure "gamestart" command is entered
        while (!processor->validateCommand(command) || command->getCommand() != "gamestart") {
            if (processor->validateCommand(command) && command->getCommand() != "gamestart") {
                command->saveEffect("Player added.");
            }
            cout << "Invalid command. Please try again." << endl;
            command = processor->getCommand();
        }
        command->saveEffect("Game started.");  // Set effect for game start
        cout << "Effect: " << command->getEffect() << endl;

        // Transition to WIN state and process commands to end the game
        gameEngine->transition(WIN);
        cout << "Current State: WIN" << endl;
        command = processor->getCommand();

        // Validate command in WIN state
        while (!processor->validateCommand(command)) {
            cout << "Invalid command. Please try again." << endl;
            command = processor->getCommand();
        }
        // Check if user wants to quit the game
        if (command->getCommand() == "quit") {
            continueGame = false;
            command->saveEffect("Game ended.");
        } else {
            command->saveEffect("Restarting game.");
            gameEngine->transition(START);
        }
        cout << "Effect: " << command->getEffect() << endl;
    }

    // Display all commands entered during the game
    cout << "\nDisplaying All Commands:" << endl;
    cout << *processor;

    delete gameEngine;
    delete processor;
}

// Test function to process commands via console input
void testCommandProcessorConsole() {
    cout << "Console Input Test" << endl;
    GameEngine* gameEngine = new GameEngine();
    CommandProcessor* processor = new CommandProcessor(gameEngine);

    bool restartGame = true;

    while (restartGame) {
        // Set the game state to START and get command input
        gameEngine->transition(START);
        cout << "State: START" << endl;
        Command* command = processor->getCommand();

        // Validate input command
        while (!processor->validateCommand(command)) {
            cout << "Please enter a valid command." << endl;
            command = processor->getCommand();
        }
        command->saveEffect("Map loaded.");
        cout << "Effect: " << command->getEffect() << endl;

        // Transition to WIN state for console command validation
        gameEngine->transition(WIN);
        command = processor->getCommand();

        // Validate command and handle quit or restart
        while (!processor->validateCommand(command)) {
            cout << "Please enter a valid command." << endl;
            command = processor->getCommand();
        }
        if (command->getCommand() == "quit") {
            restartGame = false;
            command->saveEffect("Game ended.");
        } else {
            command->saveEffect("Restarting game.");
            gameEngine->transition(START);
        }
    }

    delete gameEngine;
    delete processor;
}

// Test function for command processing from a file input
void testCommandProcessorFile(const string& fileName) {
    cout << "---------------File Input Test----------------" << endl;
    GameEngine* gameEngine = new GameEngine();
    FileCommandProcessorAdapter* processor = new FileCommandProcessorAdapter(gameEngine, fileName);

    bool replayGame = true;

    while (replayGame) {
        // Transition to START state and get command from file
        gameEngine->transition(START);
        cout << "State: START" << endl;
        Command* command = processor->getCommand();

        // Validate command from file input
        while (!processor->validateCommand(command)) {
            cout << "Invalid command from file. Please check file content." << endl;
            command = processor->getCommand();
        }
        command->saveEffect("Map loaded.");
        cout << "Effect: " << command->getEffect() << endl;

        // Repeat similar structure for other states; transition to WIN
        gameEngine->transition(WIN);
        command = processor->getCommand();

        // Validate WIN state commands, handle quit or restart
        while (!processor->validateCommand(command)) {
            cout << "Please enter a valid command." << endl;
            command = processor->getCommand();
        }
        if (command->getCommand() == "quit") {
            replayGame = false;
            command->saveEffect("Game ended.");
        } else {
            command->saveEffect("Restarting game.");
            gameEngine->transition(START);
        }
    }

    delete gameEngine;
    delete processor;
}

int main() {
    int choice;
    std::cout << "Select test to run:\n";
    std::cout << "1. Test Command Processor with User Input\n";
    std::cout << "2. Test Command Processor with Console Input\n";
    std::cout << "3. Test Command Processor with File Input\n";
    std::cout << "Enter choice (1/2/3): ";
    std::cin >> choice;

    if (choice == 1) {
        std::cout << "\n--- Running Test Command Processor with User Input ---\n";
        testCommandProcessor();
    }
    else if (choice == 2) {
        std::cout << "\n--- Running Test Command Processor with Console Input ---\n";
        testCommandProcessorConsole();
    }
    else if (choice == 3) {
        std::string fileName;
        std::cout << "Enter the file name for commands: ";
        std::cin >> fileName;
        std::cout << "\n--- Running Test Command Processor with File Input ---\n";
        //testCommandProcessorFile("Command1.txt");
    }
    else {
        std::cout << "Invalid choice. Exiting program.\n";
    }

    return 0;
}