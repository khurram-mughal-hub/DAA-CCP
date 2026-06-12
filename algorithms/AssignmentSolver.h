#ifndef ASSIGNMENTSOLVER_H
#define ASSIGNMENTSOLVER_H

#include <vector>
#include "../models/Task.h"
#include "../models/Courier.h"
#include "../models/Depot.h"
#include "../utilities/DistanceMatrix.h"

struct AssignmentResult {
    std::vector<std::vector<Task>> courierAssignments;
    std::vector<int> totalValues;
    std::vector<int> totalWeights;
    std::vector<double> totalDistances;
};

class AssignmentSolver {
private:
    std::vector<Courier> couriers;
    std::vector<Task> tasks;
    std::vector<Depot> depots;
    DistanceMatrix* distanceMatrix;

    double calculateRouteDistance(const std::vector<Task>& assignedTasks, int depotId);

public:
    AssignmentSolver();
    AssignmentSolver(const std::vector<Courier>& couriers,
                     const std::vector<Task>& tasks,
                     const std::vector<Depot>& depots,
                     DistanceMatrix* distMatrix);

    AssignmentResult solve();
    void printAssignment(const AssignmentResult& result) const;
};

#endif
