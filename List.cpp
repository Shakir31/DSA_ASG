#include "List.h"
#include <iostream>
using namespace std;
List::List() {
    firstNode = NULL;
    size = 0;
}
List::~List() {
    Node* p = firstNode;
    Node* q = NULL;
    while (p != NULL) {
        q = p->next;
        delete p;
        p = q;
    }
}
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
bool List::isEmpty() {
    if (firstNode == NULL) {
        return true;
    }
    return false;
}
int List::getLength() {
    return size;
}