#ifndef COURIER_H
#define COURIER_H

class Courier {
private:
    int id;
    int capacity;
    double timeBudget;
    int assignedDepot;

public:
    Courier();
    Courier(int id, int capacity, double timeBudget, int assignedDepot);

    int getId() const;
    int getCapacity() const;
    double getTimeBudget() const;
    int getAssignedDepot() const;

    void setId(int id);
    void setCapacity(int capacity);
    void setTimeBudget(double timeBudget);
    void setAssignedDepot(int assignedDepot);

    void print() const;
};

#endif
