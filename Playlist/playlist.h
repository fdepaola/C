// playlist.h

#include <iostream>
#include <string>
using namespace std;

class Playlist {
  public:
    // make an empty list
    Playlist( );

    // delete the list
    ~Playlist( );

    // copy constructor and operator=
    Playlist(const Playlist& other);
    Playlist& operator=(const Playlist& other);

    // provide user with list count
    void listCount();

    // add an element to the end
    void addAtEnd(string tInput, string aInput);

    // add an element via keyboard input
    void addManually();

    // remove an element
    void remove(string toRemove);
    void removeManually();

    // print the list Forward
    void printForward( ) const;

    // shuffle the list
    void shuffle();

  private:
    struct Node {
      string title;
      string artist;
      Node* next;
      Node* prev;
    };

    Node* head;
    Node* tail;
    int count;
};


