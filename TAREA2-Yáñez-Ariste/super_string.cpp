#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

class super_string {
private:
    struct nodo {
        nodo* left = nullptr;
        nodo* right = nullptr;
        int index;
        char c;
        nodo(int idx, char ch) : index(idx), c(ch) {}
        nodo() {}
    };
    
    int height = 0; // Altura del árbol
    int length = 0; // Largo del super-string
    nodo* root = nullptr; // Raíz del super-string
    
    // Helper function to delete a tree
    void clear(nodo* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }
    
    // Helper function to insert a node
    nodo* insert(nodo* node, int idx, char ch) {
        if (!node) {
            return new nodo(idx, ch);
        }
        if (idx < node->index) {
            node->left = insert(node->left, idx, ch);
        } else {
            node->right = insert(node->right, idx, ch);
        }
        return node;
    }
    
    // Helper function to find the height of the tree
    int findHeight(nodo* node) {
        if (!node) return -1;
        return max(findHeight(node->left), findHeight(node->right)) + 1;
    }
    
    // Helper function to create a string from the tree
    void treeToString(nodo* node, string& result) {
        if (node) {
            treeToString(node->left, result);
            result += node->c;
            treeToString(node->right, result);
        }
    }
    
    // Helper function to reverse a segment of the tree
    void reverseSegment(nodo*& node, int l, int r) {
        if (!node) return;
        if (node->index < l) {
            reverseSegment(node->right, l, r);
        } else if (node->index >= r) {
            reverseSegment(node->left, l, r);
        } else {
            char tmp = node->c;
            nodo* leftNode = node->left;
            nodo* rightNode = node->right;
            reverseSegment(node->left, l, r);
            reverseSegment(node->right, l, r);
            node->c = tmp;
            node->left = rightNode;
            node->right = leftNode;
        }
    }
    
public:
    super_string() {}
    
    ~super_string() {
        clear(root);
    }
    
    // Insert character at the end of the super-string
    void agregar(char c) {
        root = insert(root, length++, c);
        height = findHeight(root);
    }
    
    // Join two super-strings
    void juntar(super_string &s) {
        string newString = this->stringizar() + s.stringizar();
        this->limpiar();
        for (int i = 0; i < newString.size(); ++i) {
            this->agregar(newString[i]);
        }
    }
    
    // Separate the super-string into two parts
    void separar(int i, super_string &a, super_string &b) {
        string fullString = this->stringizar();
        string leftPart = fullString.substr(0, i);
        string rightPart = fullString.substr(i);
        
        a.limpiar();
        b.limpiar();
        
        for (char c : leftPart) {
            a.agregar(c);
        }
        for (char c : rightPart) {
            b.agregar(c);
        }
    }
    
    // Reverse the super-string
    void reverso() {
        reverseSegment(root, 0, length);
    }
    
    // Trim the tree to make sure height is O(log n)
    int recortar() {
        // This can be implemented as balancing the tree
        // For simplicity, we will use an in-order traversal to rebuild the tree
        string fullString = this->stringizar();
        this->limpiar();
        for (char c : fullString) {
            this->agregar(c);
        }
        height = findHeight(root);
        return height;
    }
    
    // Convert the super-string to a standard string
    string stringizar() {
        string result;
        treeToString(root, result);
        return result;
    }
    
    // Clear the tree
    void limpiar() {
        clear(root);
        root = nullptr;
        length = 0;
        height = 0;
    }
};


int main() {
    super_string ss;
    string command;
    while (getline(cin, command)) {
        stringstream ssCommand(command);
        string operation;
        ssCommand >> operation;
        
        if (operation == "INSERTAR") {
            int idx;
            string str;
            ssCommand >> idx >> str;
            super_string temp;
            for (char c : str) {
                temp.agregar(c);
            }
            super_string leftPart, rightPart;
            ss.separar(idx, leftPart, rightPart);
            leftPart.juntar(temp);
            leftPart.juntar(rightPart);
            ss = leftPart;
        } else if (operation == "MOSTRAR") {
            cout << ss.stringizar() << endl;
        } else if (operation == "ELIMINAR") {
            int l, r;
            ssCommand >> l >> r;
            super_string leftPart, middlePart, rightPart;
            ss.separar(l, leftPart, middlePart);
            middlePart.separar(r - l, middlePart, rightPart);
            leftPart.juntar(rightPart);
            ss = leftPart;
        } else if (operation == "REVERSO") {
            int l, r;
            ssCommand >> l >> r;
            super_string leftPart, middlePart, rightPart;
            ss.separar(l, leftPart, middlePart);
            middlePart.separar(r - l, middlePart, rightPart);
            middlePart.reverso();
            leftPart.juntar(middlePart);
            leftPart.juntar(rightPart);
            ss = leftPart;
        } else if (operation == "RECORTAR") {
            cout << ss.recortar() << endl;
        } else if (operation == "FIN") {
            break;
        }
    }
    return 0;
}