#ifndef ORDERS_H
#define ORDERS_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // for std::find
#include "LoggingObserver.h"
#include <random>
#include <ctime>




using namespace std;
class Player;
class Territory;



// Base class for all orders
class Order : public Subject, public ILoggable
{

private:
    bool negotiation=false;

protected:
    std::string *effect;
    bool *executed;
    int id;


public:
    std::string *name;

    // Constructor
    Order();

    // Destructor
    virtual ~Order();

    // Copy constructor
    Order(const Order &other);

    // Assignment operator
    Order &operator=(const Order &other);

    // Virtual methods for validation and execution
    virtual bool validate() const;
    virtual void execute();

    // ToString method
    virtual std::string toString() const;

    // Stream insertion operator
    friend std::ostream &operator<<(std::ostream &os, const Order &order);

    //Part5: Override the stringToLog function from ILoggable
    string stringToLog() const override;
};

// Derived classes for specific order types

class deployOrder : public Order
{

private:
    int armies;
    Territory* target;
    Player* player;

public:
    deployOrder(int armies, Territory* target, Player* player);
    deployOrder(const deployOrder& other);
    deployOrder();
    ~deployOrder();

    bool validate() const override;
    void execute() override;
};

class advanceOrder : public Order
{
private:
    int armies;
    Territory* source;
    Territory* target;
    Player* player;

public:
    bool winOrNot=false;
    advanceOrder(int armirs,Territory* source,Territory* target, Player* player);
    advanceOrder(const advanceOrder& other);
    ~advanceOrder();
    bool validate() const override;
    void battle(int army1,int army2);
    void execute() override;
    void getCard();
};




class bombOrder : public Order
{
private:
    Territory* target;
    Player* player;


public:
    bombOrder( Territory* target, Player* player);
    bool validate() const override;
    void execute() override;
};




class blockadeOrder : public Order
{

private:
    Player* player;
    Player* neutral;
    int armies;
    Territory* target;


public:
    blockadeOrder(int armies,Player* player, Player* neutral,Territory* target);
    blockadeOrder(int armies,Player* player,Territory* target);

    bool validate() const override;
    void execute() override;
};

class airliftOrder : public Order
{
private:
    int armies;
    Territory* source;
    Territory* target;
    Player* player;

public:
    airliftOrder(int armies,Territory* source,Territory* target, Player* player);
    bool validate() const override;
    void execute() override;
};

class negotiateOrder : public Order
{
private:
    Player* player;
    Player* enemy;
public:
    negotiateOrder(Player* player,Player *enemy );
    bool validate() const override;
    void execute() override;
};

// Class to manage a list of orders
class orderList : public Subject, public ILoggable
{
private:
    std::vector<Order *> orders;

public:
    orderList();
    // Destructor
    ~orderList();

    // Methods to add, remove, move, and display orders
    void addOrder(Order *order);
    void removeOrder(int index);
    void moveOrder(int oldIndex, int newIndex);
    void showAllOrders() const;

    // Check if there are more orders
    bool hasMoreOrders() const;

    // Get and remove the next order
    Order *getNextOrder();

    // Stream insertion operator
    friend std::ostream &operator<<(std::ostream &os, const orderList &ordersList);
    
     //Part5: Override the stringToLog function from ILoggable
    string stringToLog() const override;
};

#endif // ORDERS_H
