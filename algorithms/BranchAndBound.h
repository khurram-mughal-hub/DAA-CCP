#ifndef BRANCHANDBOUND_H
#define BRANCHANDBOUND_H

#include <vector>
#include <queue>
#include "../models/Task.h"
#include "../models/Courier.h"
#include "../models/Depot.h"
#include "MSTBound.h"
#include "FractionalBound.h"

struct BBNode {
    int level;
    int currentValue;
    int currentWeight;
    double currentDistance;
    double upperBound;
    double lowerBound;
    std::vector<int> selectedItems;

    bool operator<(const BBNode& other) const {
        return upperBound < other.upperBound;
    }
};

struct BBResult {
    std::vector<Task> selectedTasks;
    int bestValue;
    int totalWeight;
    double totalDistance;
    int nodesExplored;
    int nodesPruned;
};

class BranchAndBound {
private:
    std::vector<Task> tasks;
    Courier courier;
    Depot depot;
    MSTBound mstBound;
    FractionalBound fracBound;

    double calculateExtraDistance(int taskId, const std::vector<int>& selectedIds);
    void updateBounds(BBNode& node, const std::vector<Task>& remainingTasks);

public:
    BranchAndBound();
    BranchAndBound(const std::vector<Task>& tasks, const Courier& courier, const Depot& depot);

    BBResult solve();
    void setTasks(const std::vector<Task>& tasks);
    void setCourier(const Courier& courier);
    void setDepot(const Depot& depot);

    void printResult(const BBResult& result) const;
};

#endif
