#include "BranchAndBound.h"
#include <iostream>
#include <algorithm>
#include <limits>
#include <cmath>

BranchAndBound::BranchAndBound() {}

BranchAndBound::BranchAndBound(const std::vector<Task>& tasks, const Courier& courier, const Depot& depot)
    : tasks(tasks), courier(courier), depot(depot),
      mstBound(tasks, depot), fracBound(tasks, courier.getCapacity()) {}

void BranchAndBound::setTasks(const std::vector<Task>& tasks) {
    this->tasks = tasks;
    mstBound.setTasks(tasks);
    fracBound.setTasks(tasks);
}

void BranchAndBound::setCourier(const Courier& courier) {
    this->courier = courier;
    fracBound.setCapacity(courier.getCapacity());
}

void BranchAndBound::setDepot(const Depot& depot) {
    this->depot = depot;
    mstBound.setDepot(depot);
}

double BranchAndBound::calculateExtraDistance(int taskId, const std::vector<int>& selectedIds) {
    if (selectedIds.empty()) {
        Task task = tasks[taskId];
        double dx = task.getX() - depot.getX();
        double dy = task.getY() - depot.getY();
        return 2 * std::sqrt(dx * dx + dy * dy);
    }

    Task newTask = tasks[taskId];
    double minIncrease = std::numeric_limits<double>::max();

    for (int id : selectedIds) {
        Task existingTask = tasks[id];
        double dx1 = newTask.getX() - existingTask.getX();
        double dy1 = newTask.getY() - existingTask.getY();
        double dist = std::sqrt(dx1 * dx1 + dy1 * dy1);
        if (dist < minIncrease) {
            minIncrease = dist;
        }
    }

    double dx = newTask.getX() - depot.getX();
    double dy = newTask.getY() - depot.getY();
    double depotDist = std::sqrt(dx * dx + dy * dy);
    minIncrease = std::min(minIncrease, depotDist);

    return minIncrease;
}

void BranchAndBound::updateBounds(BBNode& node, const std::vector<Task>& remainingTasks) {
    fracBound.setCurrentState(node.currentValue, node.currentWeight);
    fracBound.setTasks(remainingTasks);
    node.upperBound = fracBound.calculateUpperBound();

    std::vector<Task> selectedTaskObjs;
    for (int id : node.selectedItems) {
        selectedTaskObjs.push_back(tasks[id]);
    }
    for (const Task& t : remainingTasks) {
        selectedTaskObjs.push_back(t);
    }
    mstBound.setTasks(selectedTaskObjs);
    node.lowerBound = mstBound.calculateMSTBound();
}

BBResult BranchAndBound::solve() {
    BBResult result;
    result.bestValue = 0;
    result.totalWeight = 0;
    result.totalDistance = 0.0;
    result.nodesExplored = 0;
    result.nodesPruned = 0;

    if (tasks.empty()) {
        return result;
    }

    std::priority_queue<BBNode> pq;

    BBNode root;
    root.level = -1;
    root.currentValue = 0;
    root.currentWeight = 0;
    root.currentDistance = 0.0;

    std::vector<Task> remainingTasks(tasks.begin(), tasks.end());
    updateBounds(root, remainingTasks);

    pq.push(root);

    double bestLowerBound = std::numeric_limits<double>::max();

    std::cout << std::endl;
    std::cout << "============================" << std::endl;
    std::cout << "BRANCH AND BOUND" << std::endl;
    std::cout << "============================" << std::endl << std::endl;

    while (!pq.empty()) {
        BBNode current = pq.top();
        pq.pop();
        result.nodesExplored++;

        std::cout << "Exploring Node at Level " << current.level << std::endl;
        std::cout << "  Current Value: " << current.currentValue << std::endl;
        std::cout << "  Current Weight: " << current.currentWeight << std::endl;
        std::cout << "  Upper Bound: " << current.upperBound << std::endl;
        std::cout << "  Lower Bound (MST): " << current.lowerBound << std::endl;

        if (current.upperBound <= result.bestValue) {
            std::cout << "  PRUNED: Upper bound <= best value" << std::endl << std::endl;
            result.nodesPruned++;
            continue;
        }

        if (current.lowerBound > courier.getTimeBudget()) {
            std::cout << "  PRUNED: Lower bound > time budget" << std::endl << std::endl;
            result.nodesPruned++;
            continue;
        }

        std::cout << "  EXPANDED" << std::endl << std::endl;

        int nextLevel = current.level + 1;

        if (nextLevel >= static_cast<int>(tasks.size())) {
            if (current.currentValue > result.bestValue) {
                result.bestValue = current.currentValue;
                result.totalWeight = current.currentWeight;
                result.totalDistance = current.currentDistance;

                result.selectedTasks.clear();
                for (int id : current.selectedItems) {
                    result.selectedTasks.push_back(tasks[id]);
                }
            }
            continue;
        }

        BBNode includeNode;
        includeNode.level = nextLevel;
        includeNode.currentValue = current.currentValue + tasks[nextLevel].getValue();
        includeNode.currentWeight = current.currentWeight + tasks[nextLevel].getWeight();
        includeNode.selectedItems = current.selectedItems;
        includeNode.selectedItems.push_back(nextLevel);

        double extraDist = calculateExtraDistance(nextLevel, current.selectedItems);
        includeNode.currentDistance = current.currentDistance + extraDist;

        if (includeNode.currentWeight <= courier.getCapacity() &&
            includeNode.currentDistance <= courier.getTimeBudget()) {

            std::vector<Task> remainingAfterInclude;
            for (int i = nextLevel + 1; i < static_cast<int>(tasks.size()); ++i) {
                remainingAfterInclude.push_back(tasks[i]);
            }
            updateBounds(includeNode, remainingAfterInclude);

            if (includeNode.upperBound > result.bestValue &&
                includeNode.lowerBound <= courier.getTimeBudget()) {
                pq.push(includeNode);
            } else {
                result.nodesPruned++;
            }
        }

        BBNode excludeNode;
        excludeNode.level = nextLevel;
        excludeNode.currentValue = current.currentValue;
        excludeNode.currentWeight = current.currentWeight;
        excludeNode.currentDistance = current.currentDistance;
        excludeNode.selectedItems = current.selectedItems;

        std::vector<Task> remainingAfterExclude;
        for (int i = nextLevel + 1; i < static_cast<int>(tasks.size()); ++i) {
            remainingAfterExclude.push_back(tasks[i]);
        }
        updateBounds(excludeNode, remainingAfterExclude);

        if (excludeNode.upperBound > result.bestValue) {
            pq.push(excludeNode);
        } else {
            result.nodesPruned++;
        }
    }

    return result;
}

void BranchAndBound::printResult(const BBResult& result) const {
    std::cout << "Branch and Bound Complete!" << std::endl << std::endl;
    std::cout << "Best Value: " << result.bestValue << std::endl;
    std::cout << "Total Weight: " << result.totalWeight << std::endl;
    std::cout << "Total Distance: " << result.totalDistance << std::endl;
    std::cout << "Nodes Explored: " << result.nodesExplored << std::endl;
    std::cout << "Nodes Pruned: " << result.nodesPruned << std::endl;
    std::cout << std::endl;

    std::cout << "Selected Tasks:" << std::endl;
    for (const Task& task : result.selectedTasks) {
        std::cout << "  Task " << task.getId()
                  << " (Value: " << task.getValue()
                  << ", Weight: " << task.getWeight() << ")" << std::endl;
    }
}
