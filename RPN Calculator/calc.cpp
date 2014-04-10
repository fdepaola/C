// calc.cpp

#include <iostream>
#include <cassert>
#include <math.h>
#include "calc.h"
using namespace std;

Invalid::Invalid() {
}

void Invalid::printMessage(){
        cout << "Invalid Expression Given!" << endl;
}


DivideByZero::DivideByZero() {
}

void DivideByZero::printMessage(){
        cout << "You're trying to divide by zero. Stop it." << endl;
}

ListStack::ListStack() {
    top = NULL;
    count = 0;
}

ListStack::~ListStack() {
    while(!empty()) {
        pop();
    }
}

void ListStack::push(double num) {
    Node* new_node = new Node;
    new_node->number = num;
    new_node->underneath = top;
    top = new_node;
    count++;
}

double ListStack::pop() {
    if (empty()) {
        throw Invalid();
    }
    double top_num = top->number;
    Node* to_delete = top;
    top = top->underneath;
    delete to_delete;
    count--;
    return top_num;
}

bool ListStack::empty() const {
    return top == NULL;
}

void ListStack::add() {
    double sum = pop() + pop();
    push(sum);
}

void ListStack::subtract() {
    double second = pop();
    double diff = pop() - second;
    push(diff);
}

void ListStack::multiply() {
    double product = pop() * pop();
    push(product);
}

void ListStack::divide() {
    double denominator = pop();
    if (denominator == 0.0)
        throw DivideByZero();
    double quotient = pop() / denominator;
    push(quotient);
}

void ListStack::exp() {
    double power = pop();
    double raised = pow(pop(),power);
    push(raised);
}
