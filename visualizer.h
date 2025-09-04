#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <unordered_map>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <unordered_set>
#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>

template <typename NodeType>
void visualize(NodeType* head, const std::string& filename = "list", const std::string& title = "Linked List") {
    std::ofstream fout(filename + ".dot");
    if (!fout) {
        std::cerr << "Error: Cannot open file for writing\n";
        return;
    }

    fout << "digraph G {\n";
    fout << "  rankdir=LR;\n";
    fout << "  node [shape=record, style=filled, fillcolor=lightblue, fontname=\"Arial\"];\n";
    fout << "  edge [fontname=\"Arial\", fontsize=10];\n";
    fout << "  label=\"" << title << "\";\n";
    fout << "  labelloc=t;\n";

    std::unordered_set<NodeType*> visited;
    std::unordered_map<NodeType*, std::string> nodeNames;
    std::unordered_map<NodeType*, std::string> nodeColors;
    std::vector<std::string> detectedErrors;
    int id = 0;
    bool hasCycle = false;

    // Detect cycles using Floyd's algorithm
    if (head && head->next) {
        NodeType* slow = head;
        NodeType* fast = head->next;
        
        while (fast && fast->next) {
            if (slow == fast) {
                hasCycle = true;
                break;
            }
            slow = slow->next;
            fast = fast->next->next;
        }
    }

    // First pass: identify all nodes and potential issues
    NodeType* curr = head;
    while (curr != nullptr) {
        if (visited.count(curr)) {
            // Try to access value with common field names
            if constexpr (std::is_member_pointer<decltype(&NodeType::val)>::value) {
                detectedErrors.push_back("Cycle detected at node with value: " + std::to_string(curr->val));
            } else if constexpr (std::is_member_pointer<decltype(&NodeType::value)>::value) {
                detectedErrors.push_back("Cycle detected at node with value: " + std::to_string(curr->value));
            } else if constexpr (std::is_member_pointer<decltype(&NodeType::data)>::value) {
                detectedErrors.push_back("Cycle detected at node with value: " + std::to_string(curr->data));
            } else {
                detectedErrors.push_back("Cycle detected at node");
            }
            break;
        }
        visited.insert(curr);
        
        // Check for suspicious pointer values
        if (reinterpret_cast<uintptr_t>(curr->next) < 0x1000 && curr->next != nullptr) {
            if constexpr (std::is_member_pointer<decltype(&NodeType::val)>::value) {
                detectedErrors.push_back("Suspicious pointer value at node: " + std::to_string(curr->val));
            } else if constexpr (std::is_member_pointer<decltype(&NodeType::value)>::value) {
                detectedErrors.push_back("Suspicious pointer value at node: " + std::to_string(curr->value));
            } else if constexpr (std::is_member_pointer<decltype(&NodeType::data)>::value) {
                detectedErrors.push_back("Suspicious pointer value at node: " + std::to_string(curr->data));
            } else {
                detectedErrors.push_back("Suspicious pointer value at node");
            }
        }
        
        // Check for self-loop
        if (curr->next == curr) {
            if constexpr (std::is_member_pointer<decltype(&NodeType::val)>::value) {
                detectedErrors.push_back("Self-loop at node: " + std::to_string(curr->val));
            } else if constexpr (std::is_member_pointer<decltype(&NodeType::value)>::value) {
                detectedErrors.push_back("Self-loop at node: " + std::to_string(curr->value));
            } else if constexpr (std::is_member_pointer<decltype(&NodeType::data)>::value) {
                detectedErrors.push_back("Self-loop at node: " + std::to_string(curr->data));
            } else {
                detectedErrors.push_back("Self-loop at node");
            }
        }
        
        std::string nodeId = "n" + std::to_string(id++);
        nodeNames[curr] = nodeId;
        nodeColors[curr] = (curr == head) ? "green" : "lightblue";
        
        curr = curr->next;
    }

    // Create all nodes
    for (const auto& pair : nodeNames) {
        NodeType* node = pair.first;
        std::string nodeId = pair.second;
        
        // Try to access value with common field names
        fout << nodeId << " [label=\"{" ;
        
        if constexpr (std::is_member_pointer<decltype(&NodeType::val)>::value) {
            fout << node->val;
        } else if constexpr (std::is_member_pointer<decltype(&NodeType::value)>::value) {
            fout << node->value;
        } else if constexpr (std::is_member_pointer<decltype(&NodeType::data)>::value) {
            fout << node->data;
        } else {
            fout << "?";
        }
        
        fout << " | <next> next}\", fillcolor=\"" 
             << nodeColors[node] << "\"];\n";
    }

    // Create edges with proper styling
    for (const auto& pair : nodeNames) {
        NodeType* node = pair.first;
        std::string sourceId = pair.second;

        if (node->next != nullptr) {
            if (nodeNames.count(node->next)) {
                std::string targetId = nodeNames[node->next];
                
                // Check if this edge creates a cycle
                if (visited.count(node->next) && nodeNames[node->next] < sourceId) {
                    fout << sourceId << ":next -> " << targetId 
                         << " [color=red, style=dashed, label=\"cycle\"];\n";
                } else {
                    fout << sourceId << ":next -> " << targetId << ";\n";
                }
            } else {
                // Dangling pointer
                std::string dangId = "dangling_" + std::to_string(reinterpret_cast<uintptr_t>(node->next));
                fout << dangId << " [label=\"DANGLING\\n0x" 
                     << std::hex << reinterpret_cast<uintptr_t>(node->next) << std::dec
                     << "\", shape=box, style=filled, fillcolor=orange];\n";
                fout << sourceId << ":next -> " << dangId 
                     << " [color=red, style=dashed, label=\"dangling\"];\n";
                
                // Try to access value with common field names
                if constexpr (std::is_member_pointer<decltype(&NodeType::val)>::value) {
                    detectedErrors.push_back("Dangling pointer from node: " + std::to_string(node->val));
                } else if constexpr (std::is_member_pointer<decltype(&NodeType::value)>::value) {
                    detectedErrors.push_back("Dangling pointer from node: " + std::to_string(node->value));
                } else if constexpr (std::is_member_pointer<decltype(&NodeType::data)>::value) {
                    detectedErrors.push_back("Dangling pointer from node: " + std::to_string(node->data));
                } else {
                    detectedErrors.push_back("Dangling pointer from node");
                }
            }
        } else {
            // NULL pointer
            fout << sourceId << ":next -> null_" << sourceId << ";\n";
            fout << "null_" << sourceId << " [label=\"NULL\", shape=box, style=dashed, fillcolor=lightgrey];\n";
        }
    }

    // Add error messages if any
    if (!detectedErrors.empty()) {
        fout << "subgraph cluster_errors {\n";
        fout << "  label=\"DETECTED ERRORS\";\n";
        fout << "  style=filled;\n";
        fout << "  fillcolor=red;\n";
        fout << "  fontcolor=white;\n";
        fout << "  fontsize=12;\n";
        
        for (size_t i = 0; i < detectedErrors.size(); ++i) {
            fout << "  error" << i << " [label=\"" << detectedErrors[i] 
                 << "\", shape=note, fillcolor=white, fontcolor=red];\n";
        }
        
        fout << "}\n";
    }

    // Add legend
    fout << "subgraph cluster_legend {\n";
    fout << "  label=\"Legend\";\n";
    fout << "  style=dashed;\n";
    fout << "  legend_head [label=\"Head\", fillcolor=green, shape=record];\n";
    fout << "  legend_normal [label=\"Normal Node\", fillcolor=lightblue, shape=record];\n";
    fout << "  legend_dangling [label=\"Dangling Ptr\", fillcolor=orange, shape=box];\n";
    fout << "  legend_null [label=\"NULL\", fillcolor=lightgrey, shape=box, style=dashed];\n";
    
    if (hasCycle || !detectedErrors.empty()) {
        fout << "  legend_cycle [label=\"Cycle/Error\", fillcolor=red, shape=record];\n";
    }
    
    fout << "}\n";

    fout << "}\n";
    fout.close();

    // Generate PNG using Graphviz
    std::stringstream cmd;
    cmd << "dot -Tpng " << filename << ".dot -o " << filename << ".png";
    int result = system(cmd.str().c_str());
    
    if (result != 0) {
        std::cerr << "Warning: Graphviz dot command failed. Make sure Graphviz is installed.\n";
        return;
    }

    // Auto-open the generated image
    std::string openCmd;
#if defined(_WIN32)
    openCmd = "start " + filename + ".png";
#elif defined(__APPLE__)
    openCmd = "open " + filename + ".png";
#else
    openCmd = "xdg-open " + filename + ".png 2>/dev/null";
#endif
    
    system(openCmd.c_str());
}

#endif // VISUALIZER_H
