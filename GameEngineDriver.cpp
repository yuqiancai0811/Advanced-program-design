#include "GameEngine.h"
#include <iostream>

void testGameStates() {
    GameEngine gameEngine;

    std::string command;
    while (gameEngine.getCurrentState() != "END") {
        std::cout << "Current game state: " << gameEngine.getCurrentState() << std::endl;
        std::cout << "Enter command (start/play/end): ";
        std::cin >> command;
        gameEngine.handleUserCommand(command);
    }
}

// Main function that now calls the testGameStates function
int main() {
    testGameStates();
    return 0;
}
