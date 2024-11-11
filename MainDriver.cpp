#include <iostream>
#include <string>

// Include headers for each part's test functions
#include "CommandProcessor.h"
#include "LoggingObserver.h"
#include "Map.h"
#include "Player.h"
#include "Orders.h"
#include "Cards.h"
#include "GameEngine.h"

// Declarations of test functions
void testCommandProcessor();
void testStartupPhase();
void testMainGameLoop();
void testOrderExecution();
void testLoggingObserver();

// int main(int argc, char* argv[]) {
//     bool runTests = false;
//
//     // Check for "-test" argument
//     if (argc > 1 && std::string(argv[1]) == "-test") {
//         runTests = true;
//     }
//
//     if (runTests) {
//         std::cout << "Running tests for all parts:\n";
//
//         std::cout << "\n--- Test CommandProcessor ---\n";
//         testCommandProcessor();
//
//         std::cout << "\n--- Test Startup Phase ---\n";
//         testStartupPhase();
//
//         std::cout << "\n--- Test Main Game Loop ---\n";
//         testMainGameLoop();
//
//         std::cout << "\n--- Test Order Execution ---\n";
//         testOrderExecution();
//
//         std::cout << "\n--- Test Logging Observer ---\n";
//         testLoggingObserver();
//
//         std::cout << "\nAll tests completed.\n";
//     } else {
//         std::cout << "Starting the game in normal mode...\n";
//
//         // Initialize and run the game normally
//         GameEngine gameEngine;
//         gameEngine.startupPhase();  // Assuming startupPhase() initializes the game setup
//         gameEngine.mainGameLoop();  // Run the main game loop
//
//         std::cout << "Game has ended. Thank you for playing!\n";
//     }
//
//     return 0;
// }
