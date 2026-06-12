#include "KnapsackDP.h"
#include <iostream>
#include <algorithm>
#include <vector>

KnapsackDP::KnapsackDP() : capacity(0) {}

KnapsackDP::KnapsackDP(const std::vector<Task>& tasks, int capacity)
    : tasks(tasks), capacity(capacity) {}

void KnapsackDP::setResultTasks(const std::vector<Task>& tasks) {
    this->tasks = tasks;
}

void KnapsackDP::setCapacity(int cap) {
    this->capacity = cap;
}

KnapsackResult KnapsackDP::solve() {
    KnapsackResult result;
    result.totalValue = 0;
    result.totalWeight = 0;

    int n = tasks.size();
    if (n == 0 || capacity <= 0) {
        return result;
    }

    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(capacity + 1, 0));

    for (int i = 1; i <= n; ++i) {
        int weight = tasks[i - 1].getWeight();
        int value = tasks[i - 1].getValue();

        for (int w = 0; w <= capacity; ++w) {
            if (weight > w) {
                dp[i][w] = dp[i - 1][w];
            } else {
                dp[i][w] = std::max(dp[i - 1][w], value + dp[i - 1][w - weight]);
            }
        }
    }

    result.totalValue = dp[n][capacity];

    int w = capacity;
    for (int i = n; i > 0 && w > 0; --i) {
        if (dp[i][w] != dp[i - 1][w]) {
            result.selectedTasks.push_back(tasks[i - 1]);
            result.totalWeight += tasks[i - 1].getWeight();
            w -= tasks[i - 1].getWeight();
        }
    }

    std::reverse(result.selectedTasks.begin(), result.selectedTasks.end());

    return result;
}

void KnapsackDP::printResult(const KnapsackResult& result) const {
    std::cout << "============================" << std::endl;
    std::cout << "KNAPSACK PHASE" << std::endl;
    std::cout << "============================" << std::endl << std::endl;

    std::cout << "Selected Tasks:" << std::endl;
    if (result.selectedTasks.empty()) {
        std::cout << "  (No tasks selected)" << std::endl;
    } else {
        for (const Task& task : result.selectedTasks) {
            std::cout << "  Task " << task.getId()
                      << " (Value: " << task.getValue()
                      << ", Weight: " << task.getWeight() << ")" << std::endl;
        }
    }

    std::cout << std::endl;
    std::cout << "Total Weight: " << result.totalWeight << std::endl;
    std::cout << "Total Value: " << result.totalValue << std::endl;

    std::cout << std::endl;
    std::cout << "Complexity Analysis:" << std::endl;
    std::cout << "  Time: O(N * Qk) = O(" << tasks.size() << " * " << capacity << ")"
              << " = O(" << tasks.size() * capacity << ")" << std::endl;
    std::cout << "  Space: O(N * Qk) = O(" << tasks.size() * capacity << ")" << std::endl;
}
