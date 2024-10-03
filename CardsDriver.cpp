#include "Cards.h"
#include <iostream>
using namespace std;

void testCards() {
    // Create a deck
    Deck deck;

    // Create a hand
    Hand hand;

    // Draw cards from the deck and add them to the hand
    cout << "Drawing cards and adding them to the hand..." << endl;
    for (int i = 0; i < 3; ++i) {
        // Draw a card from the deck
        Card *drawnCard = deck.draw();  // Now drawing a pointer to Card
        hand.addCard(drawnCard);        // Add the pointer to the hand
        cout << "Added card of type: " << drawnCard->getType() << " to hand." << endl;
    }

    cout << "\nPlaying cards in the hand..." << endl;

    // Play all cards in the hand one by one using the play method
    while (!hand.isEmpty()) {
        // Get the first card in the hand
        Card *firstCard = hand.getHand().front();
        // Play the card
        firstCard->play(hand, deck);
        cout << deck << endl;
        cout << hand << endl;
    }

    cout << "\nAll cards have been played and returned to the deck." << endl;

    // Display final state of deck and hand
    cout << deck << endl;
    cout << hand << endl;
}

int main() {
    // Run the test for cards
    testCards();

    return 0;
}
