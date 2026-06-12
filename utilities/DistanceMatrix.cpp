#include "DistanceMatrix.h"
#include <iostream>
#include <iomanip>
#include <cmath>

DistanceMatrix::DistanceMatrix() : size(0) {}

DistanceMatrix::DistanceMatrix(int n) : size(n) {
    initialize(n);
}

void DistanceMatrix::initialize(int n) {
    size = n;
    matrix.resize(n, std::vector<double>(n, 0.0));
}

double DistanceMatrix::calculateEuclidean(double x1, double y1, double x2, double y2) const {
    double dx = x2 - x1;
    double dy = y2 - y1;
    return std::sqrt((dx * dx) + (dy * dy));
}

void DistanceMatrix::setDistance(int i, int j, double dist) {
    if (i >= 0 && i < size && j >= 0 && j < size) {
        matrix[i][j] = dist;
        matrix[j][i] = dist;
    }
}

double DistanceMatrix::getDistance(int i, int j) const {
    if (i >= 0 && i < size && j >= 0 && j < size) {
        return matrix[i][j];
    }
    return 0.0;
}

void DistanceMatrix::buildFromCoordinates(const std::vector<double>& xCoords, const std::vector<double>& yCoords) {
    int n = xCoords.size();
    initialize(n);

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            double dist = calculateEuclidean(xCoords[i], yCoords[i], xCoords[j], yCoords[j]);
            setDistance(i, j, dist);
        }
        matrix[i][i] = 0.0;
    }
}

int DistanceMatrix::getSize() const {
    return size;
}

void DistanceMatrix::print() const {
    std::cout << "Distance Matrix (" << size << " x " << size << "):" << std::endl;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            std::cout << std::fixed << std::setprecision(2) << std::setw(8) << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
