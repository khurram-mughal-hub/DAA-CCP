#include "TSPSolver.h"
#include <iostream>
#include <cmath>
#include <limits>
#include <algorithm>

TSPSolver::TSPSolver() : distanceMatrix(nullptr) {}

TSPSolver::TSPSolver(const std::vector<Task>& tasks, const Depot& depot, DistanceMatrix* distMatrix)
    : tasks(tasks), depot(depot), distanceMatrix(distMatrix) {}

void TSPSolver::setTasks(const std::vector<Task>& tasks) {
    this->tasks = tasks;
}

void TSPSolver::setDepot(const Depot& depot) {
    this->depot = depot;
}

double TSPSolver::calculateDistance(const Task& a, const Task& b) const {
    double dx = b.getX() - a.getX();
    double dy = b.getY() - a.getY();
    return std::sqrt(dx * dx + dy * dy);
}

double TSPSolver::calculateDistanceToDepot(const Task& task) const {
    double dx = task.getX() - depot.getX();
    double dy = task.getY() - depot.getY();
    return std::sqrt(dx * dx + dy * dy);
}

int TSPSolver::findNearestUnvisited(int current, const std::vector<bool>& visited) const {
    double minDist = std::numeric_limits<double>::max();
    int nearest = -1;

    for (size_t i = 0; i < tasks.size(); ++i) {
        if (!visited[i]) {
            double dist;
            if (current == -1) {
                dist = calculateDistanceToDepot(tasks[i]);
            } else {
                dist = calculateDistance(tasks[current], tasks[i]);
            }

            if (dist < minDist) {
                minDist = dist;
                nearest = static_cast<int>(i);
            }
        }
    }

    return nearest;
}

TSPResult TSPSolver::solveNearestNeighbor() {
    TSPResult result;
    result.totalDistance = 0.0;

    if (tasks.empty()) {
        return result;
    }

    std::vector<bool> visited(tasks.size(), false);
    std::vector<int> route;

    int current = -1;
    int visitedCount = 0;

    while (visitedCount < static_cast<int>(tasks.size())) {
        int nearest = findNearestUnvisited(current, visited);

        if (nearest == -1) break;

        visited[nearest] = true;
        route.push_back(tasks[nearest].getId());

        if (current == -1) {
            result.totalDistance += calculateDistanceToDepot(tasks[nearest]);
        } else {
            result.totalDistance += calculateDistance(tasks[current], tasks[nearest]);
        }

        current = nearest;
        visitedCount++;
    }

    if (current != -1) {
        result.totalDistance += calculateDistanceToDepot(tasks[current]);
    }

    result.routeOrder = route;
    return result;
}

void TSPSolver::printResult(const TSPResult& result) const {
    std::cout << "============================" << std::endl;
    std::cout << "ROUTING" << std::endl;
    std::cout << "============================" << std::endl << std::endl;

    std::cout << "Route:" << std::endl;
    std::cout << "  Depot (" << depot.getX() << ", " << depot.getY() << ")";

    for (int taskId : result.routeOrder) {
        std::cout << " -> Task " << taskId;
    }

    std::cout << " -> Depot" << std::endl;

    std::cout << std::endl;
    std::cout << "Total Distance: " << result.totalDistance << std::endl;

    std::cout << std::endl;
    std::cout << "Algorithm: Nearest Neighbor Heuristic" << std::endl;
    std::cout << "Complexity: O(N^2) where N = " << tasks.size() << std::endl;
}
