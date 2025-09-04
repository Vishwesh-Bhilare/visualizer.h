#include <iostream>
using namespace std;
#include "visualizer.h"

// Define the Node class with val and next as members
class Node {
public:
    int val;
    Node* next;

    Node(int value) {
        val = value;
        next = nullptr;
    }
};

// Function to create a linked list
Node* createLinkedList() {
    Node* head = nullptr;
    Node* tail = nullptr;
    int n;
    cout << "Enter number of nodes: ";
    cin >> n;
    
    for (int i = 0; i < n; i++) {
        int val;
        cout << "Enter value for node " << (i + 1) << ": ";
        cin >> val;
        
        Node* newNode = new Node(val);
        
        if (head == nullptr) {
            head = newNode;
            tail = head;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    return head;
}

// Function to display the linked list
void displayList(Node* head) {
    Node* current = head;
    cout << "Linked list: ";
    while (current != nullptr) {
        cout << current->val << " ";
        current = current->next;
    }
    cout << endl;
}

// Function to delete a node with a specific value
Node* deleteNode(Node* head, int value) {
    if (head == nullptr) return nullptr;

    // Special case: if the head node contains the value to delete
    if (head->val == value) {
        Node* temp = head;
        head = head->next;
        delete temp;
        return head;
    }

    // Search for the node to delete
    Node* current = head;
    while (current->next != nullptr && current->next->val != value) {
        current = current->next;
    }

    // If the value was found, delete the node
    if (current->next != nullptr) {
        Node* temp = current->next;
        current->next = current->next->next;
        delete temp;
    } else {
        cout << "Value not found in the list." << endl;
    }

    return head;
}

int main() {
    Node* head = createLinkedList();	// initialize the linked list
    
    visualize(head);	// show linked list
    
    int valueToDelete;	// delete a node by value
    cout << "\nEnter value to delete: ";
    cin >> valueToDelete;
    
    head = deleteNode(head, valueToDelete);
    
    visualize(head);
    
    return 0;
}
