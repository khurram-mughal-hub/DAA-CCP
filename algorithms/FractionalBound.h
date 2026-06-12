#ifndef FRACTIONALBOUND_H
#define FRACTIONALBOUND_H

#include <vector>
#include "../models/Task.h"

struct ItemRatio {
    int index;
    double ratio;
    int value;
    int weight;

    bool operator<(const ItemRatio& other) const {
        return ratio > other.ratio;
    }
};

class FractionalBound {
private:
    std::vector<Task> tasks;
    int capacity;
    int currentValue;
    int currentWeight;

public:
    FractionalBound();
    FractionalBound(const std::vector<Task>& tasks, int capacity);

    void setCurrentState(int value, int weight);
    double calculateUpperBound();
    void setTasks(const std::vector<Task>& tasks);
    void setCapacity(int cap);

    void printBound(double bound) const;
};

#endif
