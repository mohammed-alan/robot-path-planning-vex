#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <iostream>

// Node type enumeration
enum NodeType {
    REGULAR,           // Regular node (empty square)
    START_LOCATION,    // Starting location node
    TURNING_NODE,      // Node where robot needs to turn
    OBJECT_DETECTION   // Node where an object is detected
};

// Doubly Linked List Node structure
struct Node {
    int x;              // X coordinate (East)
    int y;              // Y coordinate (North)
    NodeType type;      // Type of node
    Node* prev;         // Pointer to previous node
    Node* next;         // Pointer to next node
    
    // Constructor
    Node(int x_coord, int y_coord, NodeType node_type) : 
        x(x_coord), y(y_coord), type(node_type), prev(nullptr), next(nullptr) {}
};

// Doubly Linked List class
class DoublyLinkedList {
private:
    Node* head;         // Pointer to the first node
    Node* tail;         // Pointer to the last node
    int size;           // Current size of the list
    int capacity;       // Maximum capacity of the list

public:
    // Constructor
    DoublyLinkedList(int max_capacity = 10);
    
    // Destructor
    ~DoublyLinkedList();
    
    // Insert a new node at the end of the list
    bool insert(int x, int y, NodeType type);
    
    // Remove node(s) at the specified index
    bool remove(int index);
    
    // Remove regular nodes between the current position and the latest necessary node
    void removeRegularNodes();
    
    // Remove regular nodes between two necessary nodes
    void removeRegularNodesBetweenNecessary(Node* current, Node* previous);
    
    // Get the current size of the list
    int getSize() const;
    
    // Get the capacity of the list
    int getCapacity() const;
    
    // Check if the list is full
    bool isFull() const;
    
    // Check if the list is empty
    bool isEmpty() const;
    
    // Print the list
    void print() const;
    
    // Get the head of the list
    Node* getHead() const;
    
    // Get the tail of the list
    Node* getTail() const;
    
    // Find the latest necessary node
    Node* findLatestNecessaryNode() const;
};

#endif // DOUBLY_LINKED_LIST_H
