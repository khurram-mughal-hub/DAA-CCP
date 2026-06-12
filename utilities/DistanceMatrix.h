#ifndef DISTANCEMATRIX_H
#define DISTANCEMATRIX_H

#include <vector>
#include <cmath>

class DistanceMatrix {
private:
    std::vector<std::vector<double>> matrix;
    int size;

    double calculateEuclidean(double x1, double y1, double x2, double y2) const;

public:
    DistanceMatrix();
    DistanceMatrix(int n);

    void initialize(int n);
    void setDistance(int i, int j, double dist);
    double getDistance(int i, int j) const;

    void buildFromCoordinates(const std::vector<double>& xCoords, const std::vector<double>& yCoords);

    int getSize() const;
    void print() const;
};

#endif
