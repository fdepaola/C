// playlist.cpp

#include <iostream>
#include <string>
#include <cstdlib>
#include "playlist.h"
using namespace std;


// make an empty list
Playlist::Playlist( ) {
  count = 0;
  head = NULL;
  tail = NULL;
}

// delete the list
Playlist::~Playlist( ) {
  // remove first one until there is no first one
  while(head != NULL) {
    remove(head->title);
  }
}

// copy constructor
Playlist::Playlist(const Playlist& other) {
  Node* current = other.head;
  head = tail = NULL;
  while(current != NULL) {
    addAtEnd(current->title, current->artist);
    current = current->next;
  }
  count = other.count;
}


// assignment operator
Playlist& Playlist::operator=(const Playlist& other) {
  // clear the list
  while(head != NULL) {
    remove(head->title);
  }

  // copy over the other
  Node* current = other.head;
  head = tail = NULL;
  while(current != NULL) {
    addAtEnd(current->title, current->artist);
    current = current->next;
  }
  count = other.count;

  return *this;
}

// provide user with list count
void Playlist::listCount() {
    cout << count << endl;
}

// add an element to the end
void Playlist::addAtEnd(string tInput, string aInput) {
    Node* node = new Node;
    node->title = tInput;
    node->artist = aInput;
    node->next = NULL;

    // if tail is NULL, it's the ONLY one
    if(tail == NULL) {
        head = node;
        tail = node;
        node->prev = NULL;
    } else {
        node->prev = tail;
        tail->next = node;
        tail = node;
    }
}

// add an element via keyboard input
void Playlist::addManually() {
  // make the node
  Node* node = new Node;
  char tInput[100];
  char aInput[100];
  cout << "Song title: ";
  cin.getline(tInput, 100);
  cout << "Artist name: ";
  cin.getline(aInput, 100);
  node->title = tInput;
  node->artist = aInput;
  node->next = NULL;
  count++;

  // if tail is NULL, it's the ONLY one
  if(tail == NULL) {
    head = node;
    tail = node;
    node->prev = NULL;
  } else {
    node->prev = tail;
    tail->next = node;
    tail = node;
  }
}

// remove an element
void Playlist::remove(string toRemove) {
    Node* current = head;
    
    while(current != NULL) {
        if(current->title == toRemove) {
            //FOUNT IT
            
            if(current->prev == NULL) {
                head = current->next;
                if(head != NULL)
                    head->prev = NULL;
            } else {
                // point previous' next pointer at the one after current

                current->prev->next = current->next;

                // also point next's previous pointer to current's prev
                if(current->next != NULL)
                    current->next->prev = current->prev;
                else
                    tail = current->prev;
            }
            
            //delete node
            delete current;
            return;
        }

        current = current->next;
    }

    // if we got here we didn't find it!
    cout << toRemove << " was not found!" << endl;
}

// remove an element via keyboard input
void Playlist::removeManually() {
    char songRemove[100];
    char artistRemove[100];
    cout << "Song title: ";
    cin.getline(songRemove, 100);
    cout << "Artist name: ";
    cin.getline(artistRemove, 100);
  // find the node with our song
  Node* current = head;

  while(current != NULL) {
    if(current->title == songRemove && current->artist == artistRemove) {
      // FOUND IT

      // if the previous is NULL, we are removing head!
      if(current->prev == NULL) {
        head = current->next;
        if(head != NULL)
          head->prev = NULL;
      } else {
        // point previous's next pointer at the one after current
        current->prev->next = current->next;

        // also point next's previous pointer to current's prev
        if(current->next != NULL)
          current->next->prev = current->prev;
        else
          tail = current->prev;
      }

      // delete node
      delete current;
      count--;
      // return
      return;
    }

    current = current->next;
  }

  // if we got here we didn't find it!
  cout << songRemove << " by " << artistRemove << " was not found in the list!" << endl;
}


// print the list Forward
void Playlist::printForward( ) const {
  // the node we are currently on
  Node* current = head;

  // while we're not at the end
  while(current != NULL) {
    // print the data portion
    cout << current->title << " by " << current-> artist << endl;

    // move onto the next one
    current = current->next;
  }

  cout << endl << endl;
}

// shuffle the list
void Playlist::shuffle() {
    srand(time(0));
    Node* current = head;
    Playlist newList;
    while (count > 0) {
        int r = rand() % count;
        for (int i=0; i<r; i++){
            current = current->next;
        }
        newList.addAtEnd(current->title, current->artist);
        newList.count++;
        remove(current->title);
        count--;
        current = head;
    }
    *this = newList;
}

