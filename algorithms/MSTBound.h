#ifndef MSTBOUND_H
#define MSTBOUND_H

#include <vector>
#include "../models/Task.h"
#include "../models/Depot.h"

struct Edge {
    int from;
    int to;
    double weight;

    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

class MSTBound {
private:
    std::vector<Task> tasks;
    Depot depot;

    double calculateDistance(const Task& a, const Task& b) const;
    double calculateDistanceToDepot(const Task& task) const;
    int findParent(std::vector<int>& parent, int i);
    void unionSets(std::vector<int>& parent, std::vector<int>& rank, int x, int y);

public:
    MSTBound();
    MSTBound(const std::vector<Task>& tasks, const Depot& depot);

    double calculateMSTBound();
    void setTasks(const std::vector<Task>& tasks);
    void setDepot(const Depot& depot);

    void printBound(double bound) const;
};

#endif
