#include "GameEngine.h"
#include "MapDriver.h"
#include <iostream>
#include <limits>
//g++ GameEngine.cpp GameEngineDriver.cpp Cards.cpp Map.cpp Orders.cpp Player.cpp

void GameEngine::printWelcomeMessage() {
    std::cout << "Welcome to the Warzone game!\n";
    std::cout << "Type 'start' to start the game.\n";
}

// Constructor initializes the game state to "START"
GameEngine::GameEngine() : currentState("START"), selectedMap(nullptr) {
    printWelcomeMessage();
}

void GameEngine::setcurrentState(std::string newGameState){
    currentState=newGameState;
}

// Destructor cleans up resources
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
    
    while (getCurrentState() == "START") {     //state1
        std::cout << "Please enter the name of the map you want to load: ";
        std::string mapName;
        std::cin >> mapName;

        selectedMap = selectedMap -> loadMapFromFile(mapName);
        bool result =  (selectedMap == nullptr);
        if (!result) {
            std::cout << "Map " << mapName << " loaded successfully!\n";
            setcurrentState("MAP_LOADED");
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
    
    transitionTo("reinforcement");// Start from the reinforcement phase
    //setcurrentState("reinforcement");// Start from the reinforcement phase
    
    while (getCurrentState() != "win") {
        // Handle the Reinforcement phase
        if (getCurrentState() == "reinforcement") {
            reinforcementPhase();  // Assign reinforcements
        }
        // Handle the Issue Orders phase
        else if (getCurrentState() == "issue orders") {
            issueOrdersPhase();  // Players issue orders
        }
        // Handle the Execute Orders phase
        else if (getCurrentState() == "execute orders") {
            executeOrdersPhase();  // Execute orders

            std::cout << "Press Enter to 'end' to the end the game or enter 'continue' to continue playing ...\n";
            std::string userInput;
            std::getline(std::cin, userInput);

            if(userInput=="end"){
                transitionTo("win");
            }
        }


        // If a player has won, transition to the win state
        //checkWinCondition();
        
        // // Ask for confirmation before moving to the next phase
        // if (currentState != "win") {
        //     std::cout << "Press Enter to proceed to the next phase...\n";
        //     std::cin.ignore();
        //     std::string userInput;
        //     std::getline(std::cin, userInput);  // Wait for user input (pressing Enter)
        // }
        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }
    std::cout << "Game Over! You have won!\n";
}

// Reinforcement phase with looping mechanism
void GameEngine::reinforcementPhase() {
    std::cout << "Entering Reinforcement Phase...\n";
    bool done1 = false;
    
    while (!done1) {
        std::cout << "Enter 'next' to finish the Reinforcement Phase or type any command for actions...\n";
        std::string userInput2;
        std::getline(std::cin, userInput2);
        
        if (userInput2=="next") {  // If user presses Enter without input, end the phase
            done1 = true;}
        else {
             // Here you can handle any additional reinforcement actions
             std::cout << "Reinforcement Action: " << userInput2 << std::endl;
             }
    }
    transitionTo("issue orders");
}

// Issuing Orders phase with looping mechanism
void GameEngine::issueOrdersPhase() {
    std::cout << "Entering Issue Orders Phase...\n";
    bool done2 = false;
    while (!done2) {
        std::cout << "Press 'next' to finish the Issue Orders Phase or type any command for actions...\n";

        std::string userInput3;
        std::getline(std::cin, userInput3);
        
        if (userInput3=="next") {  // If user presses Enter without input, end the phase
            done2 = true;
        } else {
            // Here you can handle any additional issue order actions
            std::cout << "Issue Order Action: " << userInput3 << std::endl;
        }
    }
    transitionTo("execute orders");
}

// Execute Orders phase - integrated logic for card drawing and playing
void GameEngine::executeOrdersPhase() {
    std::cout << "Entering Execute Orders Phase...\n";
    
    for (Player* player : playerList) {
        std::cout << "Player " << player->getName() << " is drawing cards...\n";
        
        Hand hand;  // Initialize the player's hand

        // Each player draws 3 cards
        for (int i = 0; i < 3; ++i) {
            Card* drawnCard = deck.draw();  // Drawing from the deck
            hand.addCard(drawnCard);
            std::cout << "Player " << player->getName() << " drew a card: " << drawnCard->getType() << std::endl;
        }

        // Play all cards in the hand
        // while (!hand.isEmpty()) {
        //     // hand.playFirstCard(deck);  // Assuming this method is implemented as we discussed earlier
        // }
    }

}

// New function to check win condition
void GameEngine::checkWinCondition() {
    for (Player* player : playerList) {
        // Check if the player owns all territories (winning condition)
        if (player->getOwnedTerritories().size() == selectedMap->getTerritories().size()) {
            std::cout << "Player " << player->getName() << " has won the game!\n";
            transitionTo("win");  // Transition to the win state
            break;
        }
    }
}

// Transition to the next game state
void GameEngine::transitionTo(const std::string& newState) {
    setcurrentState(newState);
    std::cout << "Game state changed to: " << getCurrentState() << std::endl;
}


