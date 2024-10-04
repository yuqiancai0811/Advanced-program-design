# COMP345 Course Project - Warzone Game

## Team Project

- **Date**: Fall 2024
- **Instructor**: Dr. Hakim Mellah (hakim.mellah@concordia.ca)

---

## Assignment 1:

### **Due Date: OCT-04-2024**

### Contributors:

- Zexu Hao(40233332) - Map
- Mingming Zhang (40258080) - Player
- Mengge He (40229748) - Orders list
- Yuqian Cai (40187954) - Cards deck/hand
- Tuya Wei (40233182) - Game Engine

### **Specific Design Requirements:**

1. All data members of user-defined class type must be of pointer type.
2. All file names and the content of the files must be according to what is given in the description below.
3. All different parts must be implemented in their own separate .cpp/.h file duo. All functions’ implementation
   must be provided in the .cpp file (i.e. no inline functions are allowed).
4. All classes must implement a correct copy constructor, assignment operator, and stream insertion operator.
5. Memory leaks must be avoided.
6. Code must be documented using comments (user-defined classes, methods, free functions, operators).
7. If you use third-party libraries that are not available in the labs and require setup/installation, you may not
   assume to have help using them and you are entirely responsible for their proper installation for grading purposes.

---

### **5 Parts to be Implemented**

#### Part 1: Map

- **Implemented by:** Zexu Hao
- **Requirements:**
  1. The `Map` class is implemented as a connected graph. The graph’s nodes represent a territory (implemented as a `Territory` class). Edges between nodes represent adjacency between territories.
  2. Continents are connected subgraphs. Each territory belongs to one and only one continent. A territory is owned by a player and contains a number of armies.
  3. The `Map` class can be used to represent any map graph configuration.
  4. The `Map` class includes a `validate()` method that makes the following checks:
     1. The map is a connected graph.
     2. Continents are connected subgraphs.
     3. Each country belongs to one and only one continent.
  5. `MapLoader` can read any Conquest map file.
  6. `MapLoader` creates a map object as a graph data structure.
  7. `MapLoader` should be able to read any text file (even invalid ones).
  8. A driver named `testLoadMaps()` reads many different map files, creates a Map object for valid files, and rejects the invalid ones.

---

#### Part 2: Player

- **Implemented by:** Mingming Zhang
- **Requirements:**
  1. The player owns a collection of territories (see Part 1).
  2. The player owns a hand of Warzone cards (see Part 4).
  3. The player contains methods `toDefend()` and `toAttack()` that return a list of territories that are to be defended and attacked, respectively.
  4. The player must also have a method `issueOrder()` that creates an order object and adds it to the player’s list of orders.
  5. A driver named `testPlayers()` creates players and demonstrates that the player objects indeed have the above features.

---

#### Part 3: Orders List

- **Implemented by:** Mengge He
- **Requirements:**
  1. The `OrdersList` class contains a list of `Order` objects.
  2. Each kind of order is implemented as a subclass of the `Order` class.
  3. The `Order` class implements a stream insertion operator that outputs a string describing the order. If the order has been executed, it should also output the effect of the order, stored as a string.
  4. Every order subclass must implement the `validate()` method that is used to validate if the order is valid.
  5. Every order subclass must implement the `execute()` method that first validates the order and executes its action if it is valid, according to the order’s meaning and the player’s state.
  6. The `OrdersList` class implements a `remove()` method that deletes an order from the list.
  7. The `OrdersList` class implements a `move()` method to move an order in the list of orders.
  8. A driver named `testOrdersLists()` creates orders of every kind, places them in an `OrdersList` object, and demonstrates that the above features are available.

---

#### Part 4: Cards Deck/Hand

- **Implemented by:** Yuqian Cai
- **Requirements:**
  1. A `Deck` object contains a finite collection of Warzone cards.
  2. A `Hand` object is a finite collection of Warzone cards.
  3. Each card has a type from: bomb, reinforcement, blockade, airlift, diplomacy.
  4. The `Deck` has a `draw()` method that allows a player to draw a card at random from the cards remaining in the deck and place it in their hand.
  5. Each card has a `play()` method that enables a player to use it during gameplay by creating special orders. Once a card has been played, it is removed from the hand and put back into the deck.
  6. A driver named `testCards()` creates a deck of cards of all different kinds, then creates a hand object that is filled by drawing cards from the deck by repeatedly calling its `draw()` method, then calls the `play()` method of all cards in the hand, resulting in the cards being put back in the deck.

---

#### Part 5: Game Engine

- **Implemented by:** Tuya Wei
- **Requirements:**
  1. The game engine controls the flow of the game using the concepts of state, transition, and command.
  2. The state represents a certain phase of the game and dictates what are the valid actions or user commands that take place in this phase.
  3. The game engine must trigger a transition to another state based on the input commands.
  4. State transitions occur as depicted in the state transition diagram.
  5. A driver named `testGameStates()` creates a console-driven interface that allows the user to navigate through all the states by typing commands as stated in the above graph.

---

## Tools

- **Visual Studio Code Version: 1.85.2 (Universal)**
- **GitHub**
