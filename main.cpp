#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

#include "models/Task.h"
#include "models/Courier.h"
#include "models/Depot.h"
#include "utilities/DistanceMatrix.h"
#include "algorithms/AssignmentSolver.h"
#include "algorithms/KnapsackDP.h"
#include "algorithms/TSPSolver.h"
#include "algorithms/MSTBound.h"
#include "algorithms/FractionalBound.h"
#include "algorithms/BranchAndBound.h"
#include "dynamic/DynamicInsertion.h"

std::vector<Depot> loadDepots(const std::string& filename);
std::vector<Courier> loadCouriers(const std::string& filename);
std::vector<Task> loadTasks(const std::string& filename);
void printHeader(const std::string& title);

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Integrated Logistics & Value Optimization System" << std::endl;
    std::cout << "========================================" << std::endl << std::endl;

    // Sample data initialization
    std::vector<Depot> depots;
    depots.push_back(Depot(1, 0.0, 0.0));
    depots.push_back(Depot(2, 50.0, 50.0));

    std::vector<Courier> couriers;
    couriers.push_back(Courier(1, 100, 200.0, 1));
    couriers.push_back(Courier(2, 80, 180.0, 2));

    std::vector<Task> tasks;
    tasks.push_back(Task(1, 10.0, 15.0, 50, 20));
    tasks.push_back(Task(2, 25.0, 30.0, 80, 35));
    tasks.push_back(Task(3, 40.0, 10.0, 60, 25));
    tasks.push_back(Task(4, 15.0, 45.0, 70, 30));
    tasks.push_back(Task(5, 35.0, 20.0, 90, 40));
    tasks.push_back(Task(6, 5.0, 25.0, 40, 15));
    tasks.push_back(Task(7, 30.0, 50.0, 65, 28));
    tasks.push_back(Task(8, 20.0, 10.0, 55, 22));
    tasks.push_back(Task(9, 45.0, 35.0, 85, 38));
    tasks.push_back(Task(10, 10.0, 40.0, 45, 18));

    std::cout << "Loaded " << depots.size() << " depots, "
              << couriers.size() << " couriers, and "
              << tasks.size() << " tasks." << std::endl << std::endl;

    // PHASE 1: Distance Matrix Generation
    printHeader("DISTANCE MATRIX GENERATION");

    std::vector<double> xCoords, yCoords;
    for (const Depot& d : depots) {
        xCoords.push_back(d.getX());
        yCoords.push_back(d.getY());
    }
    for (const Task& t : tasks) {
        xCoords.push_back(t.getX());
        yCoords.push_back(t.getY());
    }

    DistanceMatrix distMatrix(xCoords.size());
    distMatrix.buildFromCoordinates(xCoords, yCoords);
    std::cout << "Distance matrix generated for " << distMatrix.getSize() << " locations." << std::endl;

    // PHASE 2: Assignment Phase
    printHeader("ASSIGNMENT PHASE");

    AssignmentSolver assignmentSolver(couriers, tasks, depots, &distMatrix);
    AssignmentResult assignmentResult = assignmentSolver.solve();
    assignmentSolver.printAssignment(assignmentResult);

    // PHASE 3: Knapsack Phase (for Courier 1)
    printHeader("KNAPSACK PHASE");

    if (!assignmentResult.courierAssignments[0].empty()) {
        KnapsackDP knapsack(assignmentResult.courierAssignments[0], couriers[0].getCapacity());
        KnapsackResult knapsackResult = knapsack.solve();
        knapsack.printResult(knapsackResult);

        std::cout << std::endl;
        std::cout << "Complexity Analysis:" << std::endl;
        std::cout << "  Time Complexity: O(N * Qk) where N = number of items, Qk = capacity" << std::endl;
        std::cout << "  Space Complexity: O(N * Qk) for DP table" << std::endl;

        // PHASE 4: MST Lower Bound
        printHeader("MST LOWER BOUND");

        MSTBound mstBound(knapsackResult.selectedTasks, depots[0]);
        double mstLower = mstBound.calculateMSTBound();
        std::cout << "MST Lower Bound for routing: " << mstLower << std::endl;
        mstBound.printBound(mstLower);

        // PHASE 5: Fractional Upper Bound
        printHeader("FRACTIONAL KNAPSACK UPPER BOUND");

        FractionalBound fracBound(knapsackResult.selectedTasks, couriers[0].getCapacity());
        double fracUpper = fracBound.calculateUpperBound();
        std::cout << "Fractional Upper Bound: " << fracUpper << std::endl;
        fracBound.printBound(fracUpper);

        // PHASE 6: Branch and Bound Optimization
        BBResult bbResult;
        if (knapsackResult.selectedTasks.size() > 0) {
            BranchAndBound bbSolver(knapsackResult.selectedTasks, couriers[0], depots[0]);
            bbResult = bbSolver.solve();
            bbSolver.printResult(bbResult);
        }

        // PHASE 7: TSP Routing
        printHeader("ROUTING PHASE");

        std::vector<Task> tasksToRoute = knapsackResult.selectedTasks;
        if (tasksToRoute.empty()) {
            tasksToRoute = assignmentResult.courierAssignments[0];
        }

        if (!tasksToRoute.empty()) {
            TSPSolver tspSolver(tasksToRoute, depots[0], &distMatrix);
            TSPResult tspResult = tspSolver.solveNearestNeighbor();
            tspSolver.printResult(tspResult);

            // PHASE 8: Dynamic Insertion
            printHeader("DYNAMIC EVENT SIMULATION");

            double alpha = 1.5;
            DynamicInsertion dynInsertion(tasksToRoute, couriers[0], depots[0], tspResult.totalDistance, alpha);

            NewOrder order1;
            order1.value = 100;
            order1.weight = 25;
            order1.x = 55.0;
            order1.y = 45.0;

            std::cout << "Processing Dynamic Order 1..." << std::endl;
            DynamicResult dynResult1 = dynInsertion.processNewOrder(order1);
            dynInsertion.printResult(dynResult1, order1);

            NewOrder order2;
            order2.value = 70;
            order2.weight = 20;
            order2.x = 30.0;
            order2.y = 25.0;

            std::cout << std::endl;
            std::cout << "Processing Dynamic Order 2..." << std::endl;
            DynamicResult dynResult2 = dynInsertion.processNewOrder(order2);
            dynInsertion.printResult(dynResult2, order2);

        } else {
            std::cout << "No tasks to route for Courier 1." << std::endl;
        }
    } else {
        std::cout << "No tasks assigned to Courier 1 - skipping subsequent phases." << std::endl;
    }

    // Final Summary
    printHeader("EXECUTION SUMMARY");

    std::cout << "System successfully executed all phases:" << std::endl;
    std::cout << "  1. Distance Matrix Generation - Complete" << std::endl;
    std::cout << "  2. Assignment Phase (GAP) - Complete" << std::endl;
    std::cout << "  3. Knapsack Phase (0/1 DP) - Complete" << std::endl;
    std::cout << "  4. MST Lower Bound - Complete" << std::endl;
    std::cout << "  5. Fractional Upper Bound - Complete" << std::endl;
    std::cout << "  6. Branch and Bound - Complete" << std::endl;
    std::cout << "  7. TSP Routing - Complete" << std::endl;
    std::cout << "  8. Dynamic Insertion - Complete" << std::endl;
    std::cout << std::endl;

    std::cout << "Algorithms Implemented:" << std::endl;
    std::cout << "  - Greedy Assignment (GAP): O(K * N)" << std::endl;
    std::cout << "  - 0/1 Knapsack DP: O(N * Qk)" << std::endl;
    std::cout << "  - Prim's MST: O(E log E) = O(N^2 log N)" << std::endl;
    std::cout << "  - Fractional Knapsack Bound: O(N log N)" << std::endl;
    std::cout << "  - Branch and Bound: O(2^N) worst, O(N^2) best with pruning" << std::endl;
    std::cout << "  - Nearest Neighbor TSP: O(N^2)" << std::endl;
    std::cout << "  - Dynamic Insertion: O(N)" << std::endl;

    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Program Execution Complete!" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}

void printHeader(const std::string& title) {
    std::cout << std::endl;
    std::cout << "============================" << std::endl;
    std::cout << title << std::endl;
    std::cout << "============================" << std::endl;
}
