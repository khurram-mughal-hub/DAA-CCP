#include "AssignmentSolver.h"
#include <iostream>
#include <algorithm>
#include <queue>

AssignmentSolver::AssignmentSolver() : distanceMatrix(nullptr) {}

AssignmentSolver::AssignmentSolver(const std::vector<Courier>& couriers,
                                     const std::vector<Task>& tasks,
                                     const std::vector<Depot>& depots,
                                     DistanceMatrix* distMatrix)
    : couriers(couriers), tasks(tasks), depots(depots), distanceMatrix(distMatrix) {}

double AssignmentSolver::calculateRouteDistance(const std::vector<Task>& assignedTasks, int depotId) {
    if (assignedTasks.empty()) return 0.0;

    double totalDist = 0.0;

    Depot depot = depots[depotId - 1];

    if (!assignedTasks.empty()) {
        double dx = assignedTasks[0].getX() - depot.getX();
        double dy = assignedTasks[0].getY() - depot.getY();
        totalDist += std::sqrt(dx * dx + dy * dy);
    }

    for (size_t i = 0; i < assignedTasks.size() - 1; ++i) {
        double dx = assignedTasks[i + 1].getX() - assignedTasks[i].getX();
        double dy = assignedTasks[i + 1].getY() - assignedTasks[i].getY();
        totalDist += std::sqrt(dx * dx + dy * dy);
    }

    if (!assignedTasks.empty()) {
        double dx = depot.getX() - assignedTasks.back().getX();
        double dy = depot.getY() - assignedTasks.back().getY();
        totalDist += std::sqrt(dx * dx + dy * dy);
    }

    return totalDist;
}

AssignmentResult AssignmentSolver::solve() {
    AssignmentResult result;
    result.courierAssignments.resize(couriers.size());
    result.totalValues.resize(couriers.size(), 0);
    result.totalWeights.resize(couriers.size(), 0);
    result.totalDistances.resize(couriers.size(), 0.0);

    std::vector<Task> sortedTasks = tasks;
    std::sort(sortedTasks.begin(), sortedTasks.end(),
              [](const Task& a, const Task& b) {
                  return a.getValue() > b.getValue();
              });

    std::vector<int> remainingCapacity(couriers.size());
    std::vector<double> remainingTime(couriers.size());

    for (size_t i = 0; i < couriers.size(); ++i) {
        remainingCapacity[i] = couriers[i].getCapacity();
        remainingTime[i] = couriers[i].getTimeBudget();
    }

    for (const Task& task : sortedTasks) {
        bool assigned = false;
        double bestScore = -1;
        int bestCourier = -1;

        for (size_t i = 0; i < couriers.size(); ++i) {
            if (remainingCapacity[i] >= task.getWeight()) {
                std::vector<Task> testAssignments = result.courierAssignments[i];
                testAssignments.push_back(task);

                double routeDist = calculateRouteDistance(testAssignments, couriers[i].getAssignedDepot());

                if (routeDist <= remainingTime[i]) {
                    double score = (double)task.getValue() / (task.getWeight() + 1);

                    if (score > bestScore) {
                        bestScore = score;
                        bestCourier = i;
                    }
                }
            }
        }

        if (bestCourier != -1) {
            result.courierAssignments[bestCourier].push_back(task);
            result.totalValues[bestCourier] += task.getValue();
            result.totalWeights[bestCourier] += task.getWeight();
            remainingCapacity[bestCourier] -= task.getWeight();

            result.totalDistances[bestCourier] =
                calculateRouteDistance(result.courierAssignments[bestCourier],
                                        couriers[bestCourier].getAssignedDepot());
            remainingTime[bestCourier] = couriers[bestCourier].getTimeBudget() -
                                          result.totalDistances[bestCourier];
            assigned = true;
        }

        if (!assigned) {
            std::cout << "Task " << task.getId() << " could not be assigned to any courier." << std::endl;
        }
    }

    return result;
}

void AssignmentSolver::printAssignment(const AssignmentResult& result) const {
    std::cout << "============================" << std::endl;
    std::cout << "ASSIGNMENT PHASE" << std::endl;
    std::cout << "============================" << std::endl << std::endl;

    for (size_t i = 0; i < couriers.size(); ++i) {
        std::cout << "Courier " << couriers[i].getId() << " Assigned Tasks:" << std::endl;
        if (result.courierAssignments[i].empty()) {
            std::cout << "  (No tasks assigned)" << std::endl;
        } else {
            for (const Task& task : result.courierAssignments[i]) {
                std::cout << "  Task " << task.getId()
                          << " (Value: " << task.getValue()
                          << ", Weight: " << task.getWeight() << ")" << std::endl;
            }
        }
        std::cout << "  Total Value: " << result.totalValues[i] << std::endl;
        std::cout << "  Total Weight: " << result.totalWeights[i] << std::endl;
        std::cout << "  Total Distance: " << result.totalDistances[i] << std::endl;
        std::cout << std::endl;
    }
}
