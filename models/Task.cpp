#include "Task.h"
#include <iostream>

Task::Task() : id(0), x(0.0), y(0.0), value(0), weight(0) {}

Task::Task(int id, double x, double y, int value, int weight)
    : id(id), x(x), y(y), value(value), weight(weight) {}

int Task::getId() const { return id; }
double Task::getX() const { return x; }
double Task::getY() const { return y; }
int Task::getValue() const { return value; }
int Task::getWeight() const { return weight; }

void Task::setId(int id) { this->id = id; }
void Task::setX(double x) { this->x = x; }
void Task::setY(double y) { this->y = y; }
void Task::setValue(int value) { this->value = value; }
void Task::setWeight(int weight) { this->weight = weight; }

void Task::print() const {
    std::cout << "Task[" << id << "] (" << x << ", " << y
              << ") Value: " << value << ", Weight: " << weight << std::endl;
}
