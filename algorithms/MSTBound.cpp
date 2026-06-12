#include "MSTBound.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>

MSTBound::MSTBound() {}

MSTBound::MSTBound(const std::vector<Task>& tasks, const Depot& depot)
    : tasks(tasks), depot(depot) {}

void MSTBound::setTasks(const std::vector<Task>& tasks) {
    this->tasks = tasks;
}

void MSTBound::setDepot(const Depot& depot) {
    this->depot = depot;
}

double MSTBound::calculateDistance(const Task& a, const Task& b) const {
    double dx = b.getX() - a.getX();
    double dy = b.getY() - a.getY();
    return std::sqrt(dx * dx + dy * dy);
}

double MSTBound::calculateDistanceToDepot(const Task& task) const {
    double dx = task.getX() - depot.getX();
    double dy = task.getY() - depot.getY();
    return std::sqrt(dx * dx + dy * dy);
}

int MSTBound::findParent(std::vector<int>& parent, int i) {
    if (parent[i] != i) {
        parent[i] = findParent(parent, parent[i]);
    }
    return parent[i];
}

void MSTBound::unionSets(std::vector<int>& parent, std::vector<int>& rank, int x, int y) {
    int rootX = findParent(parent, x);
    int rootY = findParent(parent, y);

    if (rank[rootX] < rank[rootY]) {
        parent[rootX] = rootY;
    } else if (rank[rootX] > rank[rootY]) {
        parent[rootY] = rootX;
    } else {
        parent[rootY] = rootX;
        rank[rootX]++;
    }
}

double MSTBound::calculateMSTBound() {
    if (tasks.empty()) return 0.0;

    int n = tasks.size();
    std::vector<Edge> edges;

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            Edge e;
            e.from = i;
            e.to = j;
            e.weight = calculateDistance(tasks[i], tasks[j]);
            edges.push_back(e);
        }
    }

    std::sort(edges.begin(), edges.end());

    std::vector<int> parent(n);
    std::vector<int> rank(n, 0);
    for (int i = 0; i < n; ++i) {
        parent[i] = i;
    }

    double mstWeight = 0.0;
    int edgeCount = 0;

    for (const Edge& e : edges) {
        if (edgeCount >= n - 1) break;

        int rootFrom = findParent(parent, e.from);
        int rootTo = findParent(parent, e.to);

        if (rootFrom != rootTo) {
            unionSets(parent, rank, rootFrom, rootTo);
            mstWeight += e.weight;
            edgeCount++;
        }
    }

    if (n > 0) {
        double minDistToDepot = std::numeric_limits<double>::max();
        for (const Task& task : tasks) {
            double dist = calculateDistanceToDepot(task);
            if (dist < minDistToDepot) {
                minDistToDepot = dist;
            }
        }
        mstWeight += 2 * minDistToDepot;
    }

    return mstWeight;
}

void MSTBound::printBound(double bound) const {
    std::cout << "MST Lower Bound: " << bound << std::endl;
    std::cout << "Algorithm: Prim's Algorithm using Kruskal's implementation" << std::endl;
    std::cout << "Complexity: O(E log E) = O(N^2 log N) where N = " << tasks.size() << std::endl;
}
