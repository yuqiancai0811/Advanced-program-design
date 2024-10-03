#include "GameEngine.h"
#include "MapDriver.h"
#include <iostream>


void GameEngine::printWelcomeMessage() {
    std::cout << "Welcome to the Warzone game!\n";
    std::cout << "Type 'start' to start the game.\n";
}

// Constructor initializes the game state to "START"
GameEngine::GameEngine() : currentState("START"), selectedMap(nullptr) {
    printWelcomeMessage();
}

// Destructor cleans up resources
/// Destructor cleans up resources
GameEngine::~GameEngine() {
    // Clean up currentPlayer, if dynamically allocated
    if (currentPlayer != nullptr) {
        delete this->currentPlayer;
        currentPlayer = nullptr;
    }
    // Clean up selectedMap, if dynamically allocated
    if (selectedMap != nullptr) {
        delete this->selectedMap;
        selectedMap = nullptr;
    }
    // Clean up playerList
    for (Player* player : playerList) {
        delete player;
    }
    playerList.clear();  // Clear the vector to remove any remaining pointers
    // Clean up eliminatedPlayers
    for (Player* player : eliminatedPlayers) {
        delete player;
    }
    eliminatedPlayers.clear();  // Clear the vector
    // The `deck` is an object, so its destructor will be automatically called,
}

std::string GameEngine::getCurrentState(){
    return currentState;
}



// Handles the startup phase of the game
void GameEngine::handleStartup() {

    std::cout << "Game is starting, loading the map...\n";
    

    while (currentState == "START") {     //state1
        std::cout << "Please enter the name of the map you want to load: ";
        std::string mapName;
        std::cin >> mapName;

        selectedMap = selectedMap -> loadMapFromFile(mapName);
        bool result =  (selectedMap == nullptr);
        if (!result) {
            std::cout << "Map " << mapName << " loaded successfully!\n";
            currentState = "MAP_LOADED";
        } else {
            std::cout << "Failed to load the map. Please try again.\n";
        }
    }

    
    while (currentState == "MAP_LOADED") {     //state2
        std::cout << "Validating the map...\n";
        
        
        bool mapValidated = selectedMap->validate(); 
        
        
        if (mapValidated) {
            std::cout << "Map validated successfully!\n";
            currentState = "ADD_Player";            //state3
        } else {
            std::cout << "Map validation failed. Please load a valid map.\n";
            currentState = "START"; 
        }
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
        transitionTo("PLAYERS_ADDED");     //state4
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
        promptNextActionPlay();
    } else if (command == "end") {
        transitionTo("END");
    } else {
        std::cout << "Invalid command.\n";
    }
}
//adjust----

// Main gameplay loop
void GameEngine::promptNextActionPlay() {

    std::cout<<"play fun"<<std::endl;
    currentState = "reinforcement";  // Start from the reinforcement phase

    while (currentState != "win") {
        // Handle the Reinforcement phase
        if (currentState == "reinforcement") {
            reinforcementPhase();  // Assign reinforcements
            currentState = "issue orders";  // Move to the issue orders phase
        }
        // Handle the Issue Orders phase
        else if (currentState == "issue orders") {
            issueOrdersPhase();  // Players issue orders
            currentState = "execute orders";  // Move to the execute orders phase
        }
        // Handle the Execute Orders phase
        else if (currentState == "execute orders") {
            executeOrdersPhase();  // Execute orders
            currentState = "reinforcement";  // After execution, go back to reinforcement for the next turn
        }
    }
    std::cout << "Game Over! You have won!\n";
}

// Reinforcement phase - logic from playGame()
void GameEngine::reinforcementPhase() {
    std::cout << "Entering Reinforcement Phase...\n";
    
    if (currentState == "PLAYERS_ADDED") {
        std::cout << "Assigning reinforcements to players...\n";
        transitionTo("ASSIGN_REINFORCEMENT");
    }

    // Add reinforcement logic for players here
}

// Issuing Orders phase
void GameEngine::issueOrdersPhase() {
    std::cout << "Entering Issue Orders Phase...\n";
    // Implement the logic for issuing orders here
}

// Execute Orders phase - integrated logic for card drawing and playing
void GameEngine::executeOrdersPhase() {
    std::cout << "Entering Execute Orders Phase...\n";
    
    for (Player* player : playerList) {
        Hand hand = Hand();  // Assuming you initialize a new hand or retrieve it from the player

        // Each player draws 3 cards
        for(int i = 0; i < 3; ++i) {
            Card* drawnCard = deck.draw();  // Drawing from the deck
            hand.addCard(drawnCard);
            std::cout << "Player " << player->getName() << " drew a card: " << drawnCard->getType() << std::endl;
        }

        // Play all cards in the hand
        while (!hand.isEmpty()) {
            hand.playFirstCard(deck);
        }
    }
}

// Transition to the next game state
void GameEngine::transitionTo(const std::string& newState) {
    currentState = newState;
    std::cout << "Game state changed to: " << currentState << std::endl;
}