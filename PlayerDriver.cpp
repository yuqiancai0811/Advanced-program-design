#include "Player.h"
#include "Map.h"
#include "Orders.h"
#include <iostream>

using namespace std;  // This allows using standard library names without std:: prefix

void testPlayers() {
    // Create some territories
    vector<string> adjTerritoryNames1 = {"Territory 2", "Territory 3"};
    Territory* territory1 = new Territory("Territory 1", 0, 0, "Continent 1", adjTerritoryNames1);

    vector<string> adjTerritoryNames2 = {"Territory 1", "Territory 3"};
    Territory* territory2 = new Territory("Territory 2", 1, 1, "Continent 1", adjTerritoryNames2);

    vector<string> adjTerritoryNames3 = {"Territory 1", "Territory 2"};
    Territory* territory3 = new Territory("Territory 3", 2, 2, "Continent 2", adjTerritoryNames3);

    // Create a player and assign territories to the player
    Player player1("Player 1");
    player1.addTerritory(territory1);
    player1.addTerritory(territory2);

    territory1->addAdjacentTerritory(territory2);
    territory2->addAdjacentTerritory(territory3);

    // Test toDefend and toAttack methods
    cout << "Territories to defend:\n";
    for (Territory* t : player1.toDefend()) {
        cout << t->getName() << " ";
    }
    cout << endl;

    cout << "Territories to attack:\n";
    for (Territory* t : player1.toAttack()) {
        cout << t->getName() << " ";
    }
    cout << endl;

    // Test issuing orders
    player1.issueOrder();  // Creates and issues a deployOrder

    // Print the player's orders
    cout << "Player 1 after issuing an order:" << endl;
    cout << player1 << endl;

    // Clean up dynamically allocated memory
    delete territory1;
    delete territory2;
    delete territory3;
}

int main() {
    testPlayers();  // Call the function that tests the Player functionality
    return 0;
}
