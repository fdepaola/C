// calc.h

#include <iostream>
#include <cassert>
using namespace std;

class Invalid {
public:
    Invalid();
    void printMessage();
};

class DivideByZero {
public:
    DivideByZero();
    void printMessage();
};

class ListStack {
public:
    ListStack();
    ~ListStack();
    void push(double num);
    double pop();
    bool empty() const;
    void add();
    void subtract();
    void multiply();
    void divide();
    void exp();
    int count;

private:
    struct Node {
        double number;
        Node* underneath;
    };
    
    Node* top;
};

