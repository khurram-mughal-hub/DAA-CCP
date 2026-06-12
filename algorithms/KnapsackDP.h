#ifndef KNAPSACKDP_H
#define KNAPSACKDP_H

#include <vector>
#include "../models/Task.h"

struct KnapsackResult {
    std::vector<Task> selectedTasks;
    int totalValue;
    int totalWeight;
};

class KnapsackDP {
private:
    std::vector<Task> tasks;
    int capacity;

public:
    KnapsackDP();
    KnapsackDP(const std::vector<Task>& tasks, int capacity);

    KnapsackResult solve();
    void setResultTasks(const std::vector<Task>& tasks);
    void setCapacity(int cap);
    void printResult(const KnapsackResult& result) const;
};

#endif
