#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <functional>
#include <algorithm>

using namespace std;

class super_string {
private:
    struct nodo {
        nodo* left = nullptr;
        nodo* right = nullptr;
        int index;
        char c;

        nodo(int i, char ch) { // Constructor sin asignaciones
        }
        nodo() {}
    };

    int height = 0;
    int length = 0;
    nodo* root = nullptr;

    /*****
    * int altura(nodo* node)
    ******
    * Resumen: Calcula la altura de un nodo en el árbol.
    ******
    * Input:
    * nodo* node : Puntero al nodo cuya altura se calculará.
    ******
    * Returns:
    * int : Altura del nodo dado.
    *****/
    int altura(nodo* node) {
        if (!node) {
            return 0;
        }
        int alturaIzquierda = altura(node->left);
        int alturaDerecha = altura(node->right);
        return max(alturaIzquierda, alturaDerecha) + 1;
    }

    nodo* rotateRight(nodo* y) {
        nodo* x = y->left;
        nodo* T2 = x->right;

        x->right = y;
        y->left = T2;

        return x;
    }

    nodo* rotateLeft(nodo* x) {
        nodo* y = x->right;
        nodo* T2 = y->left;

        x->right = T2;
        y->left = x;

        return y;
    }
    /*****
    * void balancearAVL(nodo*& node)
    ******
    * Resumen: Rebalancea el árbol AVL después de una inserción o eliminación.
    ******
    * Input:
    * nodo*& node : Referencia al nodo raíz del subárbol que se balanceará.
    ******
    * Returns:
    * void, No retorna nada.
    *****/
    void balancearAVL(nodo*& node) {
        if (!node) {
            return;
        }

        int altura_izquierda = altura(node->left);
        int altura_derecha = altura(node->right);
        int balance = altura_izquierda - altura_derecha;

        if (balance == -2) {
            if (altura(node->right->left) > altura(node->right->right)) {
                node->right = rotateRight(node->right);
            }
            node = rotateLeft(node);
        } else if (balance == 2) {
            if (altura(node->left->right) > altura(node->left->left)) {
                node->left = rotateLeft(node->left);
            }
            node = rotateRight(node);
        }

        balancearAVL(node->left);
        balancearAVL(node->right);
    }

public:
    super_string() {}
        /*****
    * void limpiar()
    ******
    * Resumen: Limpia la estructura de datos, eliminando todos los nodos del árbol.
    ******
    * Input:
    * Ninguno
    ******
    * Returns:
    * void, No retorna nada.
    *****/

    void limpiar() {
        function<void(nodo*)> deleteAllNodes = [&](nodo* node) {
            if (node) {
                deleteAllNodes(node->left);
                deleteAllNodes(node->right);
                delete node;
            }
        };
        deleteAllNodes(root);
        root = nullptr;
        height = 0;
        length = 0;
    }
    void inorderTraversal(nodo* node, char* result, int& index) {
        if (node) {
            inorderTraversal(node->left, result, index);
            result[index++] = node->c;
            inorderTraversal(node->right, result, index);
        }
    }
    /*****
    * string stringizar()
    ******
    * Resumen: Devuelve una representación en forma de cadena del super_string.
    ******
    * Input:
    * Ninguno
    ******
    * Returns:
    * string, Representación del super_string como cadena.
    *****/
    string stringizar() {
        char* result = new char[length + 1];
        result[length] = '\0';

        int index = 0;
        inorderTraversal(root, result, index);

        string strResult(result);
        delete[] result;
        return strResult;
    }
    /* void eliminar(int l, int r)
    ******
    * Resumen: Elimina los caracteres en el rango [l, r] del super_string.
    ******
    * Input:
    * int l : Índice de inicio del rango a eliminar.
    * int r : Índice de fin del rango a eliminar.
    ******
    * Returns:
    * void, No retorna nada.
    *****/
    void eliminar(int l, int r) {
        function<void(nodo*&, int, int)> remove = [&](nodo*& node, int start, int end) {
            if (!node) return;
            if (node->index >= start && node->index <= end) {
                nodo* temp = node;
                node = nullptr; // Eliminar nodo
                delete temp;
            } else {
                if (node->index < start) {
                    remove(node->right, start, end);
                }
                if (node->index > end) {
                    remove(node->left, start, end);
                }
            }
        };

        remove(root, l, r);
    }
    /*****
    * void actualizarIndices(nodo* node)
    ******
    * Resumen: Actualiza los índices de los nodos en el árbol.
    ******
    * Input:
    * nodo* node : Puntero al nodo raíz del subárbol que se actualizará.
    ******
    * Returns:
    * void, No retorna nada.
    *****/
    void actualizarIndices(nodo* node) {
        if (!node) return;
        actualizarIndices(node->left);
        actualizarIndices(node->right);
        if (node->left) node->left->index = node->index + 1;
        if (node->right) node->right->index = node->index + 1;
    }
    /*****
    * void reverso()
    ******
    * Resumen: Invierte el orden de los caracteres en el super_string.
    ******
    * Input:
    * Ninguno
    ******
    * Returns:
    * void, No retorna nada.
    *****/
    void reverso() {
        function<void(nodo*)> reverseTraversal = [&](nodo* node) {
            if (!node) return;
            reverseTraversal(node->right);
            reverseTraversal(node->left);
            nodo* temp = node->left;
            node->left = node->right;
            node->right = temp;
            actualizarIndices(node);
        };
        reverseTraversal(root);
    }
    /*****
    * int calcularLongitudSubarbol(nodo* node)
    ******
    * Resumen: Calcula la longitud de un subárbol a partir del nodo dado.
    ******
    * Input:
    * nodo* node : Nodo raíz del subárbol.
    ******
    * Returns:
    * int, Longitud del subárbol.
    *****/
    int calcularLongitudSubarbol(nodo* node) {
        if (!node) return 0;
        return 1 + calcularLongitudSubarbol(node->left) + calcularLongitudSubarbol(node->right);
    }
    /* void agregar(char c)
    ******
    * Resumen: Agrega un nuevo caracter al super_string.
    ******
    * Input:
    * char c : El caracter a agregar.
    ******
    * Returns:
    * void, No retorna nada.
    *****/
    void agregar(char c) {
    nodo* newNode = new nodo(); // Crear el nuevo nodo con el constructor por defecto
    newNode->index = length;    // Asignar manualmente el índice
    newNode->c = c;             // Asignar manualmente el carácter
    length++;
    if (!root) {
        root = newNode;
        height = 1;
        return;
    }

    nodo* current = root;
    nodo* parent = nullptr;
    int currentDepth = 1;

    // Búsqueda binaria para encontrar la posición correcta para el nuevo nodo
    while (current) {
        parent = current;
        if (newNode->index <= current->index) {
            current = current->left;
        } else {
            current = current->right;
        }
        currentDepth++;
    }

    // Adjuntar el nuevo nodo a su padre
    if (newNode->index <= parent->index) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }

    // Actualizar la altura del árbol AVL
    height = max(height, currentDepth);
    balancearAVL(root); // Balancear el árbol AVL después de insertar  
    }
    /*****
    * int recortar()
    ******
    * Resumen: Reduce la altura del árbol eliminando nodos en exceso.
    ******
    * Input:
    * Ninguno
    ******
    * Returns:
    * int, La nueva altura del árbol después del recorte.
    *****/

    int recortar() {
    if (!root) {
        return 0; // Si el árbol está vacío, la longitud actual es 0
    }

    int originalHeight = height; // Guardar la altura original del árbol

    auto limpiarSubtree = [&](nodo* subNode) {
        function<void(nodo*)> deleteAllNodes = [&](nodo* node) {
            if (node) {
                deleteAllNodes(node->left);
                deleteAllNodes(node->right);
                delete node;
            }
        };
        deleteAllNodes(subNode);
    };

    // Recortar hasta que la altura del árbol sea la mínima necesaria para estar balanceado
    function<void(nodo*&)>
        recortarTree = [&](nodo*& node) {
            if (!node) {
                return; // Si el nodo es nulo, no hay nada que recortar
            }

            recortarTree(node->left); // Recortar el subárbol izquierdo
            recortarTree(node->right); // Recortar el subárbol derecho

            if (altura(node) > originalHeight) {
                limpiarSubtree(node); // Limpiar el subárbol
                node = nullptr;
            }
        };

    recortarTree(root); // Iniciar el recorte desde la raíz

    balancearAVL(root); // Balancear el árbol después del recorte
    height = altura(root); // Actualizar la altura

    return height;
}

    /*****
    * void juntar(super_string& s)
    ******
    * Resumen: Combina el contenido del super_string actual con otro super_string pasado como argumento.
    ******
    * Input:
    * super_string& s : Referencia al super_string que se combinará con el super_string actual.
    ******
    * Returns:
    * void, No retorna nada.
    *****/
    void juntar(super_string& s) {
        string combined = this->stringizar() + s.stringizar();
        this->limpiar();
        for (char c : combined) {
            this->agregar(c);
        }
    }
    /*****
    * void separar(int i, super_string& a, super_string& b)
    ******
    * Resumen: Divide el super_string en dos sub-strings en el índice dado.
    ******
    * Input:
    * int i : Índice en el cual dividir el super_string.
    * super_string& a : Referencia al super_string que almacenará la primera parte.
    * super_string& b : Referencia al super_string que almacenará la segunda parte.
    ******
    * Returns:
    * void, No retorna nada.
    *****/
    void separar(int i, super_string& a, super_string& b) {
        string fullString = this->stringizar();
        string part1 = fullString.substr(0, i);
        string part2 = fullString.substr(i);

        a.limpiar();
        for (char c : part1) {
            a.agregar(c);
        }

        b.limpiar();
        for (char c : part2) {
            b.agregar(c);
        }
    }
};

int main() {
    super_string ss;
    ifstream inputFile("prueba.txt");

    if (!inputFile) {
        cerr << "Error al abrir el archivo." << endl;
        return 1;
    }

    string line;
    while (getline(inputFile, line)) {
        stringstream ssCommand(line);
        string operation;
        ssCommand >> operation;

        if (operation == "INSERTAR") {
            int idx;
            string str;
            ssCommand >> idx >> str;
            for (char c : str) {
                ss.agregar(c);
            }
        }
        else if (operation == "MOSTRAR") {
            cout << ss.stringizar() << endl;
        }
        else if (operation == "ELIMINAR") {
            int l, r;
            ssCommand >> l >> r;
            ss.eliminar(l, r);
        } else if (operation == "REVERSO") {
            ss.reverso();
        } else if (operation == "RECORTAR") {
            int lengthAfterCut = ss.recortar();
            cout << lengthAfterCut << endl;
        } else if (operation == "JUNTAR") {
            super_string s;
            ss.juntar(s);
        } else if (operation == "SEPARAR") {
            int i;
            ssCommand >> i;
            super_string a, b;
            ss.separar(i, a, b);
            cout << "Parte 1: " << a.stringizar() << endl;
            cout << "Parte 2: " << b.stringizar() << endl;
        } else if (operation == "FIN") {
            break;
        }
    }

}