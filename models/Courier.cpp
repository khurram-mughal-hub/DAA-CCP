#include "Courier.h"
#include <iostream>

Courier::Courier() : id(0), capacity(0), timeBudget(0.0), assignedDepot(0) {}

Courier::Courier(int id, int capacity, double timeBudget, int assignedDepot)
    : id(id), capacity(capacity), timeBudget(timeBudget), assignedDepot(assignedDepot) {}

int Courier::getId() const { return id; }
int Courier::getCapacity() const { return capacity; }
double Courier::getTimeBudget() const { return timeBudget; }
int Courier::getAssignedDepot() const { return assignedDepot; }

void Courier::setId(int id) { this->id = id; }
void Courier::setCapacity(int capacity) { this->capacity = capacity; }
void Courier::setTimeBudget(double timeBudget) { this->timeBudget = timeBudget; }
void Courier::setAssignedDepot(int assignedDepot) { this->assignedDepot = assignedDepot; }

void Courier::print() const {
    std::cout << "Courier[" << id << "] Capacity: " << capacity
              << ", TimeBudget: " << timeBudget
              << ", Depot: " << assignedDepot << std::endl;
}
