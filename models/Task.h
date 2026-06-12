#ifndef TASK_H
#define TASK_H

#include <iostream>

class Task {
private:
    int id;
    double x;
    double y;
    int value;
    int weight;

public:
    Task();
    Task(int id, double x, double y, int value, int weight);

    int getId() const;
    double getX() const;
    double getY() const;
    int getValue() const;
    int getWeight() const;

    void setId(int id);
    void setX(double x);
    void setY(double y);
    void setValue(int value);
    void setWeight(int weight);

    void print() const;
};

#endif
