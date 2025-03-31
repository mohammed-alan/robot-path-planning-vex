#include "doubly_linked_list.h"
#include <iostream>

// Constructor
DoublyLinkedList::DoublyLinkedList(int max_capacity) : 
    head(nullptr), tail(nullptr), size(0), capacity(max_capacity) {}

// Destructor
DoublyLinkedList::~DoublyLinkedList() {
    // Delete all nodes in the list
    Node* current = head;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    head = nullptr;
    tail = nullptr;
    size = 0;
}

// Insert a new node at the end of the list
bool DoublyLinkedList::insert(int x, int y, NodeType type) {
    // Check if the list is full
    if (isFull()) {
        return false;
    }
    
    // Create a new node
    Node* newNode = new Node(x, y, type);
    
    // If the list is empty, set the new node as both head and tail
    if (isEmpty()) {
        head = newNode;
        tail = newNode;
    } else {
        // Otherwise, add the new node at the end
        newNode->prev = tail;
        tail->next = newNode;
        tail = newNode;
    }
    
    // Increment the size
    size++;
    
    return true;
}

// Remove node at the specified index
bool DoublyLinkedList::remove(int index) {
    // Check if the list is empty or if the index is out of bounds
    if (isEmpty() || index < 0 || index >= size) {
        return false;
    }
    
    // Find the node to remove
    Node* current = head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    
    // If the node to remove is the head
    if (current == head) {
        head = current->next;
        if (head != nullptr) {
            head->prev = nullptr;
        } else {
            // If the list becomes empty
            tail = nullptr;
        }
    } 
    // If the node to remove is the tail
    else if (current == tail) {
        tail = current->prev;
        tail->next = nullptr;
    } 
    // If the node to remove is in the middle
    else {
        current->prev->next = current->next;
        current->next->prev = current->prev;
    }
    
    // Delete the node
    delete current;
    
    // Decrement the size
    size--;
    
    return true;
}

// Remove regular nodes between the current position and the latest necessary node
void DoublyLinkedList::removeRegularNodes() {
    if (isEmpty() || size <= 1) {
        return;
    }
    
    // Find the latest necessary node
    Node* latestNecessary = findLatestNecessaryNode();
    
    if (latestNecessary == nullptr || latestNecessary == tail) {
        return;
    }
    
    // Remove regular nodes between the latest necessary node and the tail
    Node* current = latestNecessary->next;
    while (current != nullptr && current != tail) {
        Node* next = current->next;
        if (current->type == REGULAR) {
            // Update links
            current->prev->next = current->next;
            current->next->prev = current->prev;
            
            // Delete the node
            delete current;
            
            // Decrement the size
            size--;
        }
        current = next;
    }
}

// Remove regular nodes between two necessary nodes
void DoublyLinkedList::removeRegularNodesBetweenNecessary(Node* current, Node* previous) {
    if (isEmpty() || size <= 1 || current == nullptr || previous == nullptr) {
        return;
    }
    
    // Start from the node after previous
    Node* node = previous->next;
    
    // Remove regular nodes between previous and current
    while (node != nullptr && node != current) {
        Node* next = node->next;
        if (node->type == REGULAR) {
            // Update links
            node->prev->next = node->next;
            node->next->prev = node->prev;
            
            // Delete the node
            delete node;
            
            // Decrement the size
            size--;
        }
        node = next;
    }
}

// Get the current size of the list
int DoublyLinkedList::getSize() const {
    return size;
}

// Get the capacity of the list
int DoublyLinkedList::getCapacity() const {
    return capacity;
}

// Check if the list is full
bool DoublyLinkedList::isFull() const {
    return size >= capacity;
}

// Check if the list is empty
bool DoublyLinkedList::isEmpty() const {
    return size == 0;
}

// Print the list
void DoublyLinkedList::print() const {
    if (isEmpty()) {
        std::cout << "List is empty" << std::endl;
        return;
    }
    
    Node* current = head;
    int index = 0;
    
    while (current != nullptr) {
        std::cout << "Node " << index << ": (" << current->x << ", " << current->y << ") - ";
        
        // Print the node type
        switch (current->type) {
            case REGULAR:
                std::cout << "REGULAR";
                break;
            case START_LOCATION:
                std::cout << "START_LOCATION";
                break;
            case TURNING_NODE:
                std::cout << "TURNING_NODE";
                break;
            case OBJECT_DETECTION:
                std::cout << "OBJECT_DETECTION";
                break;
        }
        
        std::cout << std::endl;
        
        current = current->next;
        index++;
    }
}

// Get the head of the list
Node* DoublyLinkedList::getHead() const {
    return head;
}

// Get the tail of the list
Node* DoublyLinkedList::getTail() const {
    return tail;
}

// Find the latest necessary node
Node* DoublyLinkedList::findLatestNecessaryNode() const {
    if (isEmpty()) {
        return nullptr;
    }
    
    // Start from the tail and move backwards
    Node* current = tail;
    
    while (current != nullptr) {
        // Check if the current node is a necessary node
        if (current->type != REGULAR) {
            return current;
        }
        
        current = current->prev;
    }
    
    // If no necessary node is found, return the head
    return head;
}
