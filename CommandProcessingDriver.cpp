#include "CommandProcessingDriver.h"
#include "GameEngine.h"
#include "CommandProcessor.h"
#include <iostream>
using std::cout;
using std::endl;

void testCommandProcessor() {
    GameEngine* gameEngine = new GameEngine(GameState::start);
    CommandProcessor* processor = new CommandProcessor(gameEngine);

    gameEngine->setCommandProcessor(processor);
    processor->setGameEngine(gameEngine);

    bool continueGame = true;

    while (continueGame) {
        // START state
        gameEngine->transitionTo(GameState::start);
        cout << "Current State: START" << endl;
        Command* command = processor->getCommand();
        
        while (!processor->validateCommand(command)) {     ////
            cout << "Invalid command. Please try again." << endl;
            command = processor->getCommand();
        }
        command->saveEffect("Map loaded.");
        cout << "Effect: " << command->getEffect() << endl;

        // MAP_LOADED state
        gameEngine->transitionTo(GameState::mapLoaded);
        cout << "Current State: MAP_LOADED" << endl;
        command = processor->getCommand();

        while (!processor-> validateCommand(command) || command->getCommand() != "validatemap") {////
            if (processor->validateCommand(command) && command->getCommand() != "validatemap") { ////
                command->saveEffect("Map loaded.");
            }
            cout << "Invalid command. Please try again." << endl;
            command = processor->getCommand();
        }
        command->saveEffect("Map validated.");
        cout << "Effect: " << command->getEffect() << endl;

        // MAP_VALIDATED state
        gameEngine->transitionTo(GameState::mapValidated);
        cout << "Current State: MAP_VALIDATED" << endl;
        command = processor->getCommand();

        while (!processor->validateCommand(command)) {///
            cout << "Invalid command. Please try again." << endl;
            command = processor->getCommand();
        }
        command->saveEffect("Player added.");
        cout << "Effect: " << command->getEffect() << endl;

        // PLAYERS_ADDED state
        gameEngine->transitionTo(GameState::playersAdded);
        cout << "Current State: PLAYERS_ADDED" << endl;
        command = processor->getCommand();

        while (!processor->validateCommand(command) || command->getCommand() != "gamestart") {///
            if (processor->validateCommand(command) && command->getCommand() != "gamestart") {///
                command->saveEffect("Player added.");
            }
            cout << "Invalid command. Please try again." << endl;
            command = processor->getCommand();
        }
        command->saveEffect("Game started.");
        cout << "Effect: " << command->getEffect() << endl;

        // WIN state
        gameEngine->transitionTo(GameState::win);
        cout << "Current State: WIN" << endl;
        command = processor->getCommand();

        while (!processor->validateCommand(command)) {///
            cout << "Invalid command. Please try again." << endl;
            command = processor->getCommand();
        }
        if (command->getCommand() == "quit") {
            continueGame = false;
            command->saveEffect("Game ended.");
        } else {
            command->saveEffect("Restarting game.");
            gameEngine->transitionTo(GameState::start);
        }
        cout << "Effect: " << command->getEffect() << endl;
    }

    cout << "\nDisplaying All Commands:" << endl;
    cout << *processor;

    delete gameEngine;
    delete processor;
}

void testCommandProcessorConsole() {
    cout << "Console Input Test" << endl;
    GameEngine* gameEngine = new GameEngine(GameState::start);
    CommandProcessor* processor = new CommandProcessor(gameEngine);

    bool restartGame = true;

    while (restartGame) {
        gameEngine->transitionTo(GameState::start);
        cout << "State: START" << endl;
        Command* command = processor->getCommand();
        
        while (!processor->validateCommand(command)) {////
            cout << "Please enter a valid command." << endl;
            command = processor->getCommand();
        }
        command->saveEffect("Map loaded.");
        cout << "Effect: " << command->getEffect() << endl;

        // Continue with similar structure for other states
        gameEngine->transitionTo(GameState::win);
        command = processor->getCommand();

        while (!processor->validateCommand(command)) {///
            cout << "Please enter a valid command." << endl;
            command = processor->getCommand();
        }
        if (command->getCommand() == "quit") {
            restartGame = false;
            command->saveEffect("Game ended.");
        } else {
            command->saveEffect("Restarting game.");
            gameEngine->transitionTo(GameState::start);
        }
    }

    delete gameEngine;
    delete processor;
}

void testCommandProcessorFile(const string& fileName) {
    cout << "File Input Test" << endl;
    GameEngine* gameEngine = new GameEngine(GameState::start);
    FileCommandProcessorAdapter* processor = new FileCommandProcessorAdapter(gameEngine, fileName);


    bool replayGame = true;

    while (replayGame) {
        gameEngine->transitionTo(GameState::start);
        cout << "State: START" << endl;
        Command* command = processor->getCommand();
        
        while (!processor->validateCommand(command)) {////
            cout << "Invalid command from file. Please check file content." << endl;
            command = processor->getCommand();
        }
        command->saveEffect("Map loaded.");
        cout << "Effect: " << command->getEffect() << endl;

        // Repeat similar structure for other states
        gameEngine->transitionTo(GameState::win);
        command = processor->getCommand();

        while (!processor->validateCommand(command)) {///
            cout << "Please enter a valid command." << endl;
            command = processor->getCommand();
        }
        if (command->getCommand() == "quit") {
            replayGame = false;
            command->saveEffect("Game ended.");
        } else {
            command->saveEffect("Restarting game.");
            gameEngine->transitionTo(GameState::start);
        }
    }

    delete gameEngine;
    delete processor;
}
