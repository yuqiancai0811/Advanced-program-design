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

/*-------------------- Phases for Part 3 -------------------------*/
extern const std::string START;
extern const std::string ASSIGN_REINFORCEMENT;
extern const std::string ISSUE_ORDERS;
extern const std::string EXECUTE_ORDERS;
extern const std::string WIN;
/*---------------------------------------------------------------*/

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

    

    void gamestart(GameEngine &game);

    // Processes user commands for starting the game
    void handleUserCommand(const std::string& command, GameEngine &game); 

    // Handles the gameplay phase (assigning reinforcement, issuing orders, executing orders)
    void playGame();

    // Function to draw and play cards
    void playCards(Player* player);

    

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
    


    /*  --------------  Game state part ------------------*/
    // Get the current game state as a string
    std::string getCurrentState();
    void setcurrentState(std::string newGameState);
    // Helper function to transition between states
    void transition(const std::string& newState);
    



    /* Assignment 2 -------------- Part 2 ------------------*/
    // Additional game setup and management
    void AssignTerritories();         // Assigns territories to players
    void assignTerritoryToPlayer(Territory* territory, Player* player);

    void randomizeOrderOfPlay();      // Randomly determines the order of play for players
    
    void startupPhase();              // Handles the startup phase commands and game setup
    // 1) use the loadmap <filename> command to select a map from a list of map files as stored in a directory,
    // which results in the map being loaded in the game.
    // 2) use the validatemap command to validate the map (i.e. it is a connected graph, etc – see assignment 1).
    // 3) use the addplayer <playername> command to enter players in the game (2-6 players)

    void gamestart(GameEngine &game); // Handles the game start operations after setup is complete
    // a) fairly distribute all the territories to the players
    // b) determine randomly the order of play of the players in the game
    // c) give 50 initial army units to the players, which are placed in their respective reinforcement pool
    // d) let each player draw 2 initial cards from the deck using the deck’s draw() method





    /* -------------- Part 3 ------------------*/
    Player* winner;                  // Pointer to the player who won the game
    // Game phases
    void reinforcementPhase();        // Reinforcement phase of the game
    void issueOrdersPhase();          // Issuing orders phase of the game
    void executeOrdersPhase();        // Execution of orders phase
    void resetGame(); //Not sure if it's part of MainLoop...
    void removeEliminatedPlayers(); //Helper function for mainGameLoop

    // Main game loop
    void mainGameLoop();            // Main game loop that handles phase transitions and checks win condition

    // Set and Get current game state
    void setCurrentState(const std::string& newGameState); // Sets the current game state
    // std::string getCurrentState() const;                   // Gets the current game state

    // Game initialization and setup
    
    

    // Command processing
    void handleUserCommand(const std::string& command, GameEngine &game);  // Processes user commands

    

    // Print and display
    void printWelcomeMessage();       // Prints the welcome message when the game starts

    //Part5: Override the stringToLog function from ILoggable
    string stringToLog() const override;
};
#endif // GAMEENGINE_H
