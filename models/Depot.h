#ifndef DEPOT_H
#define DEPOT_H

class Depot {
private:
    int id;
    double x;
    double y;

public:
    Depot();
    Depot(int id, double x, double y);

    int getId() const;
    double getX() const;
    double getY() const;

    void setId(int id);
    void setX(double x);
    void setY(double y);

    void print() const;
};

#endif
