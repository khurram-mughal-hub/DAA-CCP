#include "FractionalBound.h"
#include <iostream>
#include <algorithm>

FractionalBound::FractionalBound() : capacity(0), currentValue(0), currentWeight(0) {}

FractionalBound::FractionalBound(const std::vector<Task>& tasks, int capacity)
    : tasks(tasks), capacity(capacity), currentValue(0), currentWeight(0) {}

void FractionalBound::setTasks(const std::vector<Task>& tasks) {
    this->tasks = tasks;
}

void FractionalBound::setCapacity(int cap) {
    this->capacity = cap;
}

void FractionalBound::setCurrentState(int value, int weight) {
    this->currentValue = value;
    this->currentWeight = weight;
}

double FractionalBound::calculateUpperBound() {
    if (currentWeight > capacity) {
        return 0.0;
    }

    std::vector<ItemRatio> items;
    for (size_t i = 0; i < tasks.size(); ++i) {
        ItemRatio item;
        item.index = i;
        item.value = tasks[i].getValue();
        item.weight = tasks[i].getWeight();
        if (item.weight > 0) {
            item.ratio = (double)item.value / item.weight;
        } else {
            item.ratio = std::numeric_limits<double>::max();
        }
        items.push_back(item);
    }

    std::sort(items.begin(), items.end());

    double upperBound = static_cast<double>(currentValue);
    int remainingCapacity = capacity - currentWeight;

    for (const ItemRatio& item : items) {
        if (remainingCapacity <= 0) break;

        if (item.weight <= remainingCapacity) {
            upperBound += item.value;
            remainingCapacity -= item.weight;
        } else {
            double fraction = static_cast<double>(remainingCapacity) / item.weight;
            upperBound += fraction * item.value;
            remainingCapacity = 0;
        }
    }

    return upperBound;
}

void FractionalBound::printBound(double bound) const {
    std::cout << "Fractional Knapsack Upper Bound: " << bound << std::endl;
    std::cout << "Algorithm: Greedy by value/weight ratio" << std::endl;
    std::cout << "Complexity: O(N log N) where N = " << tasks.size() << std::endl;
}
