#include "CommandProcessingDriver.h"
#include "GameEngine.h"
#include "CommandProcessor.h"

#include <iostream>
using std::cout;
using std::endl;

//{"start", "loadmap", "validatemap", "addplayer", "assignReinforcement", "issueOrders", "executeOrders", "win"};
// Test function to simulate command processing from user input
void testCommandProcessor() {
    GameEngine* gameEngine=new GameEngine;

    // Setup players and territories
    gameEngine = gameEngine->startupPhase();  // This should add players and distribute territories
    
    while(true){
        gameEngine->commandProcessor->getCommand();

    }
   

}

// Test function to process commands via console input
void testCommandProcessorConsole() {
    cout << "Console Input Test" << endl;
   GameEngine* gameEngine=new GameEngine;

    // Setup players and territories
    gameEngine = gameEngine->startupPhase();  // This should add players and distribute territories
    
    while(true){
       
        gameEngine->commandProcessor->getCommand();
    }
    
}

// Test function for command processing from a file input
void testCommandProcessorFile(string fileName) {
    cout << "---------------File Input Test----------------" << endl;
    
    GameEngine* gameEngine = new GameEngine();
    gameEngine = gameEngine->startupPhase();  // This should add players and distribute territories
    FileCommandProcessorAdapter* fileAdapter = new FileCommandProcessorAdapter(gameEngine, fileName);
    gameEngine->setProcessor(fileAdapter);
    while(true){
        std::string input;
        cin>>input;
        gameEngine->commandProcessor->getCommand();

    }
    
}

int main() {
    int choice;
    std::cout << "Select test to run:\n";
    std::cout << "1. Test Command Processor with User Input\n";
    std::cout << "2. Test Command Processor with Console Input\n";
    std::cout << "3. Test Command Processor with File Input\n";
    std::cout << "4. To exist\n";
    std::cout << "Enter choice (1/2/3): ";
    std::cin >> choice;

    while(choice!=4){
    if (choice == 1) {
        std::cout << "\n--- Running Test Command Processor with User Input ---\n";
        testCommandProcessor();
    }
    else if (choice == 2) {
        std::cout << "\n--- Running Test Command Processor with Console Input ---\n";
        testCommandProcessorConsole();
    }
    else if (choice == 3) {
        // std::string fileName;
        // std::cout << "Enter the file name for commands: ";
        // std::cin >> fileName;
        std::cout << "\n--- Running Test Command Processor with File Input ---\n";
        testCommandProcessorFile(std::string("Command1.txt"));
    }
    else {
        std::cout << "Invalid choice. \n";
    }
    }

    return 0;
}