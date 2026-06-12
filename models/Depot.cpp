#include "Depot.h"
#include <iostream>

Depot::Depot() : id(0), x(0.0), y(0.0) {}

Depot::Depot(int id, double x, double y) : id(id), x(x), y(y) {}

int Depot::getId() const { return id; }
double Depot::getX() const { return x; }
double Depot::getY() const { return y; }

void Depot::setId(int id) { this->id = id; }
void Depot::setX(double x) { this->x = x; }
void Depot::setY(double y) { this->y = y; }

void Depot::print() const {
    std::cout << "Depot[" << id << "] (" << x << ", " << y << ")" << std::endl;
}
