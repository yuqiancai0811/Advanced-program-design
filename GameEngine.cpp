#include "GameEngine.h"
#include "MapDriver.h"
#include <iostream>

// Constructor initializes the game state to "START"
GameEngine::GameEngine() : currentState("START"), selectedMap(nullptr) {
    printWelcomeMessage();
}

// Destructor cleans up resources
GameEngine::~GameEngine() {
    delete selectedMap;
    for (Player* player : playerList) {
        delete player;
    }
}

// Handles the startup phase of the game
void GameEngine::handleStartup() {
    std::cout << "Loading map...\n";

    std::string mapName;
    std::cout << "Enter the map file name: ";
    std::cin >> mapName;

    selectedMap = selectedMap -> loadMapFromFile(mapName);

    if (selectedMap && selectedMap->validate()) {
        std::cout << "Map loaded and validated successfully!\n";
        transitionTo("MAP_LOADED");
    } else {
        std::cout << "Failed to load or validate the map. Exiting...\n";
        return;
    }

    // Adding players
    std::string playerName;
    while (true) {
        std::cout << "Enter player name (or 'done' to finish): ";
        std::cin >> playerName;
        if (playerName == "done") break;
        playerList.push_back(new Player(playerName));
        std::cout << "Player " << playerName << " added.\n";
    }

    if (!playerList.empty()) {
        transitionTo("PLAYERS_ADDED");
    } else {
        std::cout << "No players added. Exiting...\n";
        return;
    }
}

// Processes user commands
void GameEngine::handleUserCommand(const std::string& command) {
    if (command == "start") {
        handleStartup();
    } else if (command == "play") {
        playGame();
    } else if (command == "end") {
        transitionTo("END");
    } else {
        std::cout << "Invalid command.\n";
    }
}

// Manages the gameplay phase (assigning reinforcement, issuing orders)
void GameEngine::playGame() {
    if (currentState == "PLAYERS_ADDED") {
        std::cout << "Assigning reinforcements to players...\n";
        transitionTo("ASSIGN_REINFORCEMENT");
    }
    // Additional game phases can be handled here
}

// Handles card drawing and playing for a player
// void GameEngine::playCards(Player* player) {
//     // Get Hand function not added.

//     //Hand& hand = player->getHand();
//     Hand hand= Hand();

//     for (int i = 0; i < 3; ++i) {
//         Card* drawnCard = deck.draw();
//         hand.addCard(drawnCard);
//         std::cout << "Player " << player->getName() << " drew a card: " << drawnCard->getType() << std::endl;
//     }

//     // Play all cards in the hand
//     while (!hand.isEmpty()) {
//         hand.playFirstCard(deck);
//     }
// }

// Helper function to transition between states
void GameEngine::transitionTo(const std::string& newState) {
    currentState = newState;
    std::cout << "Game state changed to: " << currentState << std::endl;
}

// Returns the current game state as a string
std::string GameEngine::getCurrentState() const {
    return currentState;
}

// Prints the welcome message at the start of the game
void GameEngine::printWelcomeMessage() const {
    std::cout << "Welcome to the Warzone game!\n";
    std::cout << "Type 'start' to start the game.\n";
}

// Prompts the user for the next action after startup
void GameEngine::promptNextAction() const {
    std::cout << "Next action: Enter 'play' to start gameplay or 'end' to end the game.\n";
}
