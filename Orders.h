#ifndef ORDERS_H
#define ORDERS_H

#include <iostream>
#include <string>
#include <vector>
#include "LoggingObserver.h"
// Base class for all orders
class Order : public Subject, public ILoggable
{
protected:
    std::string *effect;
    bool *executed;

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
public:
    deployOrder();
    bool validate() const override;
    void execute() override;
};

class advanceOrder : public Order
{
public:
    advanceOrder();
    bool validate() const override;
    void execute() override;
};

class bombOrder : public Order
{
public:
    bombOrder();
    bool validate() const override;
    void execute() override;
};

class blockadeOrder : public Order
{
public:
    blockadeOrder();
    bool validate() const override;
    void execute() override;
};

class airliftOrder : public Order
{
public:
    airliftOrder();
    bool validate() const override;
    void execute() override;
};

class negotiateOrder : public Order
{
public:
    negotiateOrder();
    bool validate() const override;
    void execute() override;
};

// Class to manage a list of orders
class orderList : public Subject, public ILoggable
{
private:
    std::vector<Order *> orders;

public:
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
