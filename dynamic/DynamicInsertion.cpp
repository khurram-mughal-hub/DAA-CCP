#include "DynamicInsertion.h"
#include <iostream>
#include <cmath>
#include <sstream>

DynamicInsertion::DynamicInsertion()
    : currentDistance(0.0), alpha(1.0) {}

DynamicInsertion::DynamicInsertion(const std::vector<Task>& currentTasks,
                                     const Courier& courier,
                                     const Depot& depot,
                                     double currentDistance,
                                     double alpha)
    : currentTasks(currentTasks), courier(courier), depot(depot),
      currentDistance(currentDistance), alpha(alpha) {}

void DynamicInsertion::setAlpha(double alpha) {
    this->alpha = alpha;
}

void DynamicInsertion::setTasks(const std::vector<Task>& tasks) {
    this->currentTasks = tasks;
}

void DynamicInsertion::setCourier(const Courier& courier) {
    this->courier = courier;
}

void DynamicInsertion::setDepot(const Depot& depot) {
    this->depot = depot;
}

double DynamicInsertion::calculateDistance(double x1, double y1, double x2, double y2) const {
    double dx = x2 - x1;
    double dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
}

int DynamicInsertion::getCurrentTotalWeight() const {
    int total = 0;
    for (const Task& task : currentTasks) {
        total += task.getWeight();
    }
    return total;
}

double DynamicInsertion::getCurrentTotalDistance() const {
    return currentDistance;
}

double DynamicInsertion::calculateInsertionCost(const NewOrder& order) {
    if (currentTasks.empty()) {
        return 2 * calculateDistance(depot.getX(), depot.getY(), order.x, order.y);
    }

    double minCost = std::numeric_limits<double>::max();

    double distToNew = calculateDistance(depot.getX(), depot.getY(), order.x, order.y);
    if (distToNew < minCost) {
        minCost = distToNew;
    }

    for (const Task& task : currentTasks) {
        double dist = calculateDistance(task.getX(), task.getY(), order.x, order.y);
        if (dist < minCost) {
            minCost = dist;
        }
    }

    return minCost;
}

DynamicResult DynamicInsertion::processNewOrder(const NewOrder& order) {
    DynamicResult result;
    result.accepted = false;
    result.updatedTasks = currentTasks;
    result.updatedDistance = currentDistance;

    int currentWeight = getCurrentTotalWeight();
    double extraDistance = calculateInsertionCost(order);
    double newTotalDistance = currentDistance + extraDistance;

    std::stringstream reason;
    reason << std::fixed;

    if (currentWeight + order.weight > courier.getCapacity()) {
        reason << "REJECTED: Weight constraint violation. "
               << "Current weight (" << currentWeight << ") + New weight (" << order.weight
               << ") = " << (currentWeight + order.weight)
               << " > Capacity (" << courier.getCapacity() << ")";
        result.reason = reason.str();
        result.accepted = false;
        return result;
    }

    if (newTotalDistance > courier.getTimeBudget()) {
        reason << "REJECTED: Time budget violation. "
               << "Current distance (" << currentDistance << ") + Extra distance (" << extraDistance
               << ") = " << newTotalDistance
               << " > Time budget (" << courier.getTimeBudget() << ")";
        result.reason = reason.str();
        result.accepted = false;
        return result;
    }

    double valueThreshold = alpha * extraDistance;
    if (order.value <= valueThreshold) {
        reason << "REJECTED: Value threshold not met. "
               << "Order value (" << order.value << ") <= alpha * extraDistance ("
               << alpha << " * " << extraDistance << " = " << valueThreshold << ")";
        result.reason = reason.str();
        result.accepted = false;
        return result;
    }

    result.accepted = true;
    int newId = static_cast<int>(currentTasks.size()) + 1;
    for (const Task& t : currentTasks) {
        if (t.getId() >= newId) {
            newId = t.getId() + 1;
        }
    }

    Task newTask(newId, order.x, order.y, order.value, order.weight);
    result.updatedTasks.push_back(newTask);
    result.updatedDistance = newTotalDistance;

    reason << "ACCEPTED: All constraints satisfied. "
           << "Weight: " << (currentWeight + order.weight) << "/" << courier.getCapacity()
           << ", Distance: " << newTotalDistance << "/" << courier.getTimeBudget()
           << ", Value: " << order.value << " > " << valueThreshold;
    result.reason = reason.str();

    currentTasks = result.updatedTasks;
    currentDistance = result.updatedDistance;

    return result;
}

void DynamicInsertion::printResult(const DynamicResult& result, const NewOrder& order) const {
    std::cout << std::endl;
    std::cout << "============================" << std::endl;
    std::cout << "DYNAMIC EVENT" << std::endl;
    std::cout << "============================" << std::endl << std::endl;

    std::cout << "New Order Received:" << std::endl;
    std::cout << "  Value: " << order.value << std::endl;
    std::cout << "  Weight: " << order.weight << std::endl;
    std::cout << "  Location: (" << order.x << ", " << order.y << ")" << std::endl;
    std::cout << std::endl;

    if (result.accepted) {
        std::cout << "Decision: ACCEPTED" << std::endl;
    } else {
        std::cout << "Decision: REJECTED" << std::endl;
    }

    std::cout << "Reason: " << result.reason << std::endl;
    std::cout << std::endl;

    std::cout << "Updated Route:" << std::endl;
    std::cout << "  Tasks Count: " << result.updatedTasks.size() << std::endl;
    std::cout << "  Total Distance: " << result.updatedDistance << std::endl;
}
