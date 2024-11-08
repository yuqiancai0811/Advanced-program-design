#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <string>
#include <iostream>
#include <vector>
#include "Map.h"
#include "Player.h"
#include "Orders.h"
#include "Cards.h"

#include "CommandProcessor.h"
#include "LoggingObserver.h"


class GameEngine : public Subject, public ILoggable{
private:
   
    CommandProcessor* commandProcessor;///

    Map* selectedMap;                // The game map
    std::vector<Player*> playerList; // List of players
    Deck deck;                       // The deck of cards for the game
    vector <Player*> eliminatedPlayers;
    Player *currentPlayer;
    vector <Player*> playerOder;
    string currentState;  // The current state of the game added by Yuqian Cai 

public:
    GameEngine();  // Constructor to initialize the game
    ~GameEngine(); // Destructor to clean up resources

    // Handles the startup phase of the game
    void startupPhase();

    void assignTerritoryToPlayer(Territory* territory, Player* player);

    void gamestart(GameEngine &game);

    // Processes user commands for starting the game
    void handleUserCommand(const std::string& command, GameEngine &game); 

    // Handles the gameplay phase (assigning reinforcement, issuing orders, executing orders)
    void playGame();

    // Function to draw and play cards
    void playCards(Player* player);

    // Helper function to transition between states
    void transition(const std::string& newState);

    // /////2
    // void transitionTo(GameEngine newState);


    // Prints the initial welcome message
    void printWelcomeMessage();

    // Prompts the user for the next action after startup
    void promptNextActionPlay();
    void reinforcementPhase();
    void executeOrdersPhase();
    void issueOrdersPhase();

    // Declaration for the checkWinCondition function
    void checkWinCondition();
    // Get the current game state as a string
    std::string getCurrentState();

    void setcurrentState(std::string newGameState);

    void AssignTerritories();

    void randomizeOrderOfPlay();

    /* -------------- Part 3 ------------------*/
    Player* winner;                  // Pointer to the player who won the game
    // Game phases
    void reinforcementPhase();        // Reinforcement phase of the game
    void issueOrdersPhase();          // Issuing orders phase of the game
    void executeOrdersPhase();        // Execution of orders phase
    void resetGame(); //Not sure if it's part of MainLoop...

    // Main game loop
    void mainGameLoop();              // Main game loop that handles phase transitions and checks win condition

    // Set and Get current game state
    void setCurrentState(const std::string& newGameState); // Sets the current game state
    // std::string getCurrentState() const;                   // Gets the current game state

    // Game initialization and setup
    void startupPhase();              // Handles the startup phase commands and game setup
    void gamestart(GameEngine &game); // Handles the game start operations after setup is complete

    // Command processing
    void handleUserCommand(const std::string& command, GameEngine &game);  // Processes user commands

    // Additional game setup and management
    void AssignTerritories();         // Assigns territories to players
    void randomizeOrderOfPlay();      // Randomly determines the order of play for players

    // Print and display
    void printWelcomeMessage();       // Prints the welcome message when the game starts

    //Part5: Override the stringToLog function from ILoggable
    string stringToLog() const override;
};
#endif // GAMEENGINE_H
