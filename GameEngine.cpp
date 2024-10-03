#include "GameEngine.h"
#include "MapDriver.h"
#include <iostream>

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
        mapValidated= true;
        
        
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

// Prompts the user for the next action after startup(play)
void GameEngine::promptNextAction() const {
    std::cout << "Next action: Enter 'play' to start gameplay or 'end' to end the game.\n";
}
void GameEngine::reinforcementPhase(){
    std::cout<<"Entering Reinforcement state...";
}

void executeOrdersPhase(){
    std::cout<<"Entering excuteOrder state...";
};

void issueOrdersPhase(){
    std::cout<<"Entering issueOrders state...";
};