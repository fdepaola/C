// main.cpp

#include "playlist.h"

int main() {

    //create a Playlist object
    Playlist list;

    //main menu
    cout << endl << "WELCOME TO FRANKIETUNES" << endl << "Menu:" << endl << "[a]dd" << endl << "[r]emove" << endl << "[c]ount" << endl << "[p]lay" << endl << "[s]huffle" << endl << "[q]uit" << endl << endl;
    char input[10];
    while (input[0] != 'q') {
        cin.getline(input, 10);
        if (input[0] == 'a')
            list.addManually();
        if (input[0] == 'p')
            list.printForward();
        if (input[0] == 'r')
            list.removeManually();
        if (input[0] == 'c')
            list.listCount();
        if (input[0] == 's')
            list.shuffle();
    }
    return 0;
}
