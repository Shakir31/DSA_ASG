#include "List.h"
#include <iostream>
using namespace std;

/*
============================================================
Function    : List (Constructor)
Description : Initializes an empty linked list with firstNode
              set to NULL and size set to 0.
Input       : None
Return      : None
============================================================
*/
List::List() {
    firstNode = NULL;
    size = 0;
}

/*
============================================================
Function    : ~List (Destructor)
Description : Deallocates all nodes in the linked list to
              prevent memory leaks.
Input       : None
Return      : None
============================================================
*/
List::~List() {
    Node* p = firstNode;
    Node* q = NULL;
    while (p != NULL) {
        q = p->next;
        delete p;
        p = q;
    }
}

/*
============================================================
Function    : print
Description : Displays all items in the list to the console.
              If the list is empty, displays a message.
Input       : None
Return      : None
============================================================
*/
void List::print() {
    if (isEmpty()) {
        cout << "No elements in the list";
        return;
    }
    Node* p = firstNode;
    while (p != NULL) {
        cout << p->item << " ";
        p = p->next;
    }
    cout << endl;
}

/*
============================================================
Function    : add
Description : Adds a new item to the end of the list.
Input       : item - Item to add (ItemType/string)
Return      : True if successful (bool)
============================================================
*/
bool List::add(ItemType item) {
    Node* t = new Node;
    t->item = item;
    t->next = NULL;
    if (firstNode == NULL) {
        firstNode = t;
    }
    else {
        Node* p = firstNode;
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = t;
    }
    size++;
    return true;
}

/*
============================================================
Function    : insert
Description : Inserts an item at the specified index in the
              list. Index 0 inserts at the front.
Input       : index - Position to insert at (int)
              item - Item to insert (ItemType/string)
Return      : True if successful, false if invalid index (bool)
============================================================
*/
bool List::insert(int index, ItemType item) {
    Node* t = new Node;
    t->item = item;
    if (index < 0 || index > getLength()) {
        cout << "Invalid index" << endl;
        return false;
    }
    if (index == 0) {
        t->next = firstNode;
        firstNode = t;
    }
    else {
        Node* p = firstNode;
        for (int i = 0; i < index - 1; i++) {
            p = p->next;
        }
        t->next = p->next;
        p->next = t;
    }
    size++;
    return true;
}

/*
============================================================
Function    : remove
Description : Removes the item at the specified index from
              the list and deallocates the node.
Input       : index - Position to remove from (int)
Return      : None
============================================================
*/
void List::remove(int index) {
    Node* p = firstNode;
    if (index < 0 || index > getLength()) {
        cout << "Invalid index" << endl;
        return;
    }
    if (index == 0) {
        firstNode = firstNode->next;
        delete p;
    }
    else {
        Node* q = NULL;
        for (int i = 0; i < index; i++) {
            q = p;
            p = p->next;
        }
        q->next = p->next;
        delete p;
    }
}

/*
============================================================
Function    : get
Description : Retrieves the item at the specified index.
Input       : index - Position to get from (int)
Return      : Item at index, or "Invalid index" if out of
              bounds (ItemType/string)
============================================================
*/
ItemType List::get(int index) {
    if (index < 0 || index > getLength()) {
        return "Invalid index";
    }
    if (index == 0) {
        return firstNode->item;
    }
    else {
        Node* p = firstNode;
        for (int i = 0; i < index; i++) {
            p = p->next;
        }
        return p->item;
    }
}

/*
============================================================
Function    : isEmpty
Description : Checks if the list is empty.
Input       : None
Return      : True if empty, false otherwise (bool)
============================================================
*/
bool List::isEmpty() {
    if (firstNode == NULL) {
        return true;
    }
    return false;
}

/*
============================================================
Function    : getLength
Description : Returns the number of items in the list.
Input       : None
Return      : Number of items (int)
============================================================
*/
int List::getLength() {
    return size;
}