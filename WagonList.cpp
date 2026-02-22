#include "WagonList.h"

#include <iostream>

WagonList::~WagonList() { clear(); }

void WagonList::clear()
{
    // TODO: Delete all Wagon objects in this list and reset pointers.
    Wagon *curr = front;
    while (curr){
        Wagon *nextWagon = curr->getNext();
        delete curr;
        curr = nextWagon;
    }
    front = nullptr;
    rear = nullptr;
    totalWeight = 0;
}

WagonList::WagonList(WagonList &&other) noexcept
{
    // TODO: Implement move constructor.
    // Transfer ownership of 'other' list’s nodes into this list
    // and leave 'other' in an empty but valid state.
    this->front = other.front;
    this->rear = other.rear;
    this->totalWeight = other.totalWeight;
    other.front = nullptr;
    other.rear = nullptr;
    other.totalWeight = 0;

}

WagonList &WagonList::operator=(WagonList &&other) noexcept
{
    // Operation version of the move constructor.
    // TODO: Implement it.
    if (this != &other){
        this->clear(); // Clear current list

        // Move ownership
        this->front = other.front;
        this->rear = other.rear;
        this->totalWeight = other.totalWeight;

        // Leave other in a valid empty state
        other.front = nullptr;
        other.rear = nullptr;
        other.totalWeight = 0;
    }

    return *this;
}

Wagon *WagonList::findById(int id)
{
    // TODO: Find and return the Wagon with given ID.
    // Return nullptr if not found.

    // Iterate through the list to find the wagon
    Wagon *found = front;
    while (found) {
        // Check if the current wagon's ID matches the target ID
        if (found->getID() == id) {
            return found;
        }
        // Move to the next wagon in the list if not found
        found = found->getNext();
    }
    return nullptr;
}

void WagonList::addWagonToRear(Wagon *w)
{
    // TODO: Add a Wagon to the rear (end) of the list.
    // This function does not respect the weight order
    // it inserts to end regardless of the weight

    Wagon *toAdd = w;
    // Check if list is empty
    if (isEmpty()){
        front = toAdd;
        rear = toAdd;
        totalWeight += toAdd->getWeight();
        return;
    }
    // Setting pointers to add at the end
    getRear()->setNext(toAdd);
    toAdd->setPrev(getRear());
    rear = toAdd;
    totalWeight += toAdd->getWeight();
}

int WagonList::getTotalWeight() const { return totalWeight; }

bool WagonList::isEmpty() const
{
    // TODO: Return true if the list has no wagons.
    if (getTotalWeight() == 0){
        return true;
    }
    return false;
}

void WagonList::insertSorted(Wagon *wagon)
{
    // TODO: Insert wagon into this list in descending order of weight.

    Wagon* toInsert = wagon;
    // Check if list is empty
    if (isEmpty()){
        front = toInsert;
        rear = toInsert;
        totalWeight += toInsert->getWeight();
        return;
}
    // Check if toInsert goes to front 
    if (toInsert->getWeight() >= front->getWeight()){
        toInsert->setNext(front);
        front->setPrev(toInsert);
        front = toInsert;
        totalWeight += toInsert->getWeight();
        return;
}
    // Check if toInsert goes to rear
    if (toInsert->getWeight() <= rear->getWeight()){
        rear->setNext(toInsert);
        toInsert->setPrev(rear);
        rear = toInsert;
        totalWeight += toInsert->getWeight();
        return;
}
    // Insert in the middle
    Wagon* current = front->getNext();
    while (current){
        if (toInsert->getWeight() >= current->getWeight()){
            Wagon* prevWagon = current->getPrev();
            prevWagon->setNext(toInsert);
            toInsert->setPrev(prevWagon);
            toInsert->setNext(current);
            current->setPrev(toInsert);
            totalWeight += toInsert->getWeight();
            return;
        }
    current = current->getNext();
}
}

void WagonList::appendList(WagonList &&other)
{
   // TODO: Append another WagonList to this one (merge them).
   // Use move semantics to avoid deep copies. (Double && at the parameter already makes it so)
   // 'other' should end up empty after this operation
   // At merge lists (blocks) will be protected 
   // But the one with heavier wagon at the front will be before the other list

   // Check if other list is empty
    if (other.isEmpty()){
        return;
    }
    // Check if this list is empty
    if (this->isEmpty()){
        this->front = other.front;
        this->rear = other.rear;
        this->totalWeight = other.totalWeight;
        other.front = nullptr;
        other.rear = nullptr;
        other.totalWeight = 0;
        return;
    }
   // Merge based on front wagon weights
   if (this->getFront()->getWeight() >= other.getFront()->getWeight())
   {
       this->getRear()->setNext(other.getFront());
       other.getFront()->setPrev(this->getRear());
       this->rear = other.rear;
   }
   else
   {
       other.getRear()->setNext(this->getFront());
       this->getFront()->setPrev(other.getRear());
       this->front = other.getFront();
   }
   this->totalWeight += other.getTotalWeight();
   other.front = nullptr;
   other.rear = nullptr;
   other.totalWeight = 0;
}
Wagon *WagonList::detachById(int id)
{
    // TODO: Remove a specific wagon (by ID) from this list and return it.
    // Use: std::cout << "Wagon " << toRemove->id << " detached from Wagon List. " << std::endl;
    // Return nullptr if wagon not found.

    // Find id of the wagon to remove
    Wagon* toRemove = findById(id);
    // Check if found
    if (toRemove){
        Wagon* prevWagon = toRemove->getPrev();
        Wagon* nextWagon = toRemove->getNext();
        if (prevWagon){
            prevWagon->setNext(nextWagon);
        } else {         // removing front
            front = nextWagon;
        }
        if (nextWagon){
            nextWagon->setPrev(prevWagon);
        } else {       // removing rear
            rear = prevWagon;
        }
        // Detach the wagon from the list
        toRemove->setPrev(nullptr);
        toRemove->setNext(nullptr);
        totalWeight -= toRemove->getWeight();
        std::cout << "Wagon " << toRemove->getID() << " detached from Wagon List. " << std::endl;
        return toRemove;
    }
    return nullptr;
}


WagonList WagonList::splitAtById(int id)
{
    WagonList newList; // return-by-value (will be moved automatically)

    // TODO: Split this list into two lists at the wagon with given ID.
    // The wagon with 'id' becomes the start of the new list.
    // Return the new WagonList (move return).
    // If 'id' not found, return an empty list.
    
    Wagon* splitWagon = findById(id);
    // Check for nullptr
    
    if (!splitWagon){
        return newList; // empty
    }
    Wagon* prevWagon = splitWagon->getPrev();

    // Check if prevWagon is nullptr (splitting at front)
    
    if (prevWagon){
        prevWagon->setNext(nullptr);
        this->rear = prevWagon;
    } else {
        this->front = nullptr;
        this->rear = nullptr;
    }
    // Detach the split wagon from the original list
    splitWagon->setPrev(nullptr);
    // Set up the new list
    newList.front = splitWagon;
    Wagon* current = splitWagon;
    int newTotalWeight = 0;
    // Calculate total weight for new list and set rear
    while (current){
        newTotalWeight += current->getWeight();
        if (!current->getNext()){
            newList.rear = current;
        }
        current = current->getNext();
    }
    newList.totalWeight = newTotalWeight;
    this->totalWeight -= newTotalWeight;
    return newList; // moved, not copied
}

// Print is already implemented
void WagonList::print() const
{

    std::cout << *this << std::endl;
    return;
}

// << operator is already implemented
std::ostream &operator<<(std::ostream &os, const WagonList &list)
{
    if (list.isEmpty())
        return os;

    Wagon *current = list.front;

    while (current)
    {
        os << "W" << current->getID() << "(" << current->getWeight() << "ton)";
        if (current->getNext())
            os << " - ";
        current = current->getNext();
    }
    return os;
}
