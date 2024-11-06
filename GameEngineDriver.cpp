#include "GameEngine.h"
#include <iostream>

void testGameStates() {
}

void testStartupPhase() {
    GameEngine game;



    std::string command;
    while (game.getCurrentState() != "END") {
        std::cout << "Current game state: " << game.getCurrentState() << std::endl;
        std::cout << "Enter command (start/play/end): ";
        std::cin >> command;
        game.handleUserCommand(command,game);
    }
    std::cout << "Exiting the game ";
    
}

// Main function that now calls the testStartupPhase function
int main() {
     testStartupPhase();
     return 0;
 }
