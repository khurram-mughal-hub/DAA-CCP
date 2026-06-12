#ifndef TSPSOLVER_H
#define TSPSOLVER_H

#include <vector>
#include "../models/Task.h"
#include "../models/Depot.h"
#include "../utilities/DistanceMatrix.h"

struct TSPResult {
    std::vector<int> routeOrder;
    double totalDistance;
};

class TSPSolver {
private:
    std::vector<Task> tasks;
    Depot depot;
    DistanceMatrix* distanceMatrix;

    double calculateDistance(const Task& a, const Task& b) const;
    double calculateDistanceToDepot(const Task& task) const;
    int findNearestUnvisited(int current, const std::vector<bool>& visited) const;

public:
    TSPSolver();
    TSPSolver(const std::vector<Task>& tasks, const Depot& depot, DistanceMatrix* distMatrix);

    TSPResult solveNearestNeighbor();
    void setTasks(const std::vector<Task>& tasks);
    void setDepot(const Depot& depot);

    void printResult(const TSPResult& result) const;
};

#endif
