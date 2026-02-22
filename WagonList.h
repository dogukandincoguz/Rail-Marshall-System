#ifndef WAGONLIST_H
#define WAGONLIST_H

#include "Wagon.h"

class WagonList
{
private:
    Wagon *front;
    Wagon *rear;
    int totalWeight;

public:
    WagonList() : front(nullptr), rear(nullptr), totalWeight(0) {}
    ~WagonList();

    // Disable copy constructor and copy assignment
    // Copying would cause *two lists to share the same wagons*, which leads to double-deletes.
    WagonList(const WagonList &) = delete;
    WagonList &operator=(const WagonList &) = delete;

    // Enable move constructor and move assignment
    // Moves ownership of wagons safely from one list to another.
    WagonList(WagonList &&other) noexcept;
    WagonList &operator=(WagonList &&other) noexcept;

    // Getters
    Wagon *getFront() { return front; }
    Wagon *getRear() { return rear; }
    int getTotalWeight() const;

    // insertion sorted by weight (descending)
    void insertSorted(Wagon *w);

    // Add to end regardless of weight
    void addWagonToRear(Wagon *w);

    // append other list (consume other, constant time)
    void appendList(WagonList &&other);

    // detach wagon with given id (returns pointer or nullptr)
    Wagon *detachById(int id);

    // splits list at id: returns a WagonList containing node(id)...tail
    // original list keeps head...node(prev)
    WagonList splitAtById(int id);

    // Utility Functions
    void clear();
    void print() const;
    bool isEmpty() const;
    Wagon *findById(int id);

    // Declare friend function for operator<<
    friend std::ostream &operator<<(std::ostream &os, const WagonList &list);
};

#endif
