// main.cpp

#include <iostream>
#include <string>
#include "calc.h"
using namespace std;

int main() {
    string ins = "x";
    while (ins != "") {
        ListStack calculate;
        bool error = false;
        int start = 0;
        getline(cin, ins);
        string toPush;
        int size = ins.length();
        for (int i=0; i<size; i++){   //loop through all of input
            if (error)
                break;
            if (ins[i] == ' '){       //when we find a space, we do the following
                char* inc = new char[i-start+1];
                for (int j=0; j<(i-start+1); j++)
                    inc[j] = 0;
                for (int j=start; j<i; j++)
                    inc[j-start] = ins[j];
                
                toPush = inc; //copy that char array to a string
                delete [] inc;
                start = i+1;
                if (toPush.find_first_not_of("1234567890.") == string::npos){
                    double n = atof(toPush.c_str());
                    calculate.push(n);
                }
            }
                else{
                    try{
                        char symbol = ins[i];
                        if (symbol == '+')
                            calculate.add();
                        if (symbol == '-')
                            calculate.subtract();
                        if (symbol == '*')
                            calculate.multiply();
                        if (symbol == '/'){
                            try{
                                calculate.divide();
                            }catch(DivideByZero e){
                                e.printMessage();
                                error = true;
                            }
                        }
                        if (symbol == '^')
                            calculate.exp();
                        } catch(Invalid e) {
                        e.printMessage();
                        error = true;
                        }
                }
        }
        if (!error){
            if (calculate.count==1)
                cout << calculate.pop() << endl;
            else{
                cout << "Operator mis-match!" << endl;
                while (!empty)
                    calculate.pop();
            }
        }
    }
    return 0;
}
