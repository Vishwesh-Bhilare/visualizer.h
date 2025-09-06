# Linked List Visualizer (`visualizer.h`)

`visualizer.h` is a **C++ header-only utility** that helps you visualize **singly linked lists** using [Graphviz](https://graphviz.org/).  
It automatically generates `.dot` and `.png` files showing the structure of your linked list, highlights potential issues, and even opens the visualization for you.

---

## 📌 How to Use

1. Place `visualizer.h` in the **same folder** as your `.cpp` file.  
2. Include it in your program:
   ```cpp
   #include "visualizer.h"
   ```
   Call the function after creating your linked list:

    ```cpp
    visualize(head);  // default filename: list.png, default title: "Linked List"
    ```

    You can also customize:
   
    ```cpp
    visualize(head, "mylist", "My Linked List");
    ```

## ⚠️ Make sure Graphviz is installed and dot is available in your system PATH.

---

## ✨ Features:
- Visualizes linked list nodes in a graphical .png format
- Detects common errors:
    - Cycles (with Floyd’s cycle detection)
    - Dangling pointers
    - Self-loops
    - Suspicious pointer values
- Customizable labels (supports nodes with val, value, or data fields).
- Color-coded nodes:
    - Green → Head node
    - Light blue → Normal nodes
    - Orange → Dangling pointers
    - Grey (dashed) → NULL pointers
- Legend and error cluster in the generated graph for easy debugging.
- Cross-platform

---

## ⚠️ Disadvantages / Limitations
- Works only for linked list classes/structs that have:
    - A next pointer member
    - A data field named either val, value, or data
    - Requires Graphviz installed on the system
- Currently supports only singly linked lists (no direct support for doubly linked lists)
- Error detection is basic and may not cover all edge cases

---

## 🚀 Possible Future Improvements:
- Support for doubly linked lists (prev pointers)
- More flexible handling of custom node field names (e.g., user-specified label)
- Support for circular linked lists visualization without marking them as errors
- Option to export to formats other than .png (e.g., .svg, .pdf)
- Interactive visualization in a GUI/HTML instead of static PNGs
- More detailed error diagnostics.

---

## 📷 Example

If you create a list like:

```cpp
struct Node {
    int val;
    Node* next;
    Node(int x) : val(x), next(nullptr) {}
};

int main() {
    Node* head = new Node(1);
    head->next = new Node(2);
    head->next->next = new Node(3);

    visualize(head);
}
```

It will generate a file list.png like this:

```
[1] -> [2] -> [3] -> NULL
```
(with styled nodes and legend in the actual PNG).

---

## 🛠 Requirements
- C++11 or later
- Graphviz installed and available in PATH
