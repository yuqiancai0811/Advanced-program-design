#ifndef CARDS_H
#define CARDS_H

#include <vector>
#include <string>
#include <iostream>

using namespace std;

// Define types of cards
struct CardType
{
    static const string BOMB;
    static const string REINFORCEMENT;
    static const string BLOCKADE;
    static const string AIRLIFT;
    static const string DIPLOMACY;
};
// Forward declaration
class Hand;
class Deck;

// Card class
class Card
{
public:
    // Constructors
    Card(const string &type);

    Card(const Card &other);

    Card &operator=(const Card &other);
    ~Card(); // Destructor

    string getType() const;
    // Play the card
    void play(Hand &hand, Deck &deck);

    // friend operator to access private members
    friend ostream &operator<<(ostream &out, const Card &card);

private:
    string *type;
};

// Deck class
class Deck
{
public:
    // Constructors
    Deck();

    Deck(const Deck &other);

    Deck &operator=(const Deck &other);

    ~Deck();

    // Draws a card from the deck
    Card *draw();
    // play a card then return to the deck
    void addCard(Card *card);
    // friend operator to access private members
    friend ostream &operator<<(ostream &out, const Deck &deck);

private:
    vector<Card *> cards; // Collection of card pointers
};

// Hand class
class Hand
{
public:
    // constructors
    Hand();
    Hand(const Hand &other);
    Hand &operator=(const Hand &other);
    ~Hand(); // Destructor

    //getter method
    vector<Card *> getHand() const;

    // Adds a card to the hand
    void addCard(Card *card);
    // Removes a card from the hand
    void removeCard(const Card &card);

    // check if the hand is empty
    bool isEmpty() const;
    // friend operator to access private members
    friend ostream &operator<<(ostream &out, const Hand &hand);

private:
    vector<Card *> handCards; // Cards in the player's hand
};

#endif
