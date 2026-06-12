#ifndef DYNAMICINSERTION_H
#define DYNAMICINSERTION_H

#include <vector>
#include "../models/Task.h"
#include "../models/Courier.h"
#include "../models/Depot.h"

struct NewOrder {
    int value;
    int weight;
    double x;
    double y;
};

struct DynamicResult {
    bool accepted;
    std::string reason;
    std::vector<Task> updatedTasks;
    double updatedDistance;
};

class DynamicInsertion {
private:
    std::vector<Task> currentTasks;
    Courier courier;
    Depot depot;
    double currentDistance;
    double alpha;

    double calculateInsertionCost(const NewOrder& order);
    double calculateDistance(double x1, double y1, double x2, double y2) const;

public:
    DynamicInsertion();
    DynamicInsertion(const std::vector<Task>& currentTasks,
                     const Courier& courier,
                     const Depot& depot,
                     double currentDistance,
                     double alpha = 1.0);

    DynamicResult processNewOrder(const NewOrder& order);
    int getCurrentTotalWeight() const;
    double getCurrentTotalDistance() const;

    void setAlpha(double alpha);
    void setTasks(const std::vector<Task>& tasks);
    void setCourier(const Courier& courier);
    void setDepot(const Depot& depot);

    void printResult(const DynamicResult& result, const NewOrder& order) const;
};

#endif
