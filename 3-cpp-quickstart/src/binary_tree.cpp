#include <climits>
#include <iostream>
#include <string>
#include <array>
#include <cstdint>

struct Node {
    int value;
    Node *left;
    Node *right;

    Node(int val): value(val), left(nullptr), right(nullptr) {};

    int depth() {
        int dLeft = 0;
        int dRight = 0;

        if (left) 
            dLeft = left->depth();

        if (right)
            dRight = right->depth();

        return 1 + (dLeft > dRight ? dLeft : dRight);
    }

};

void print_preOrder(Node *node) {
    if (node == nullptr)
        return;

    std::cout << node->value << " ";

    print_preOrder(node->left);
    print_preOrder(node->right);
}

void print_inOrder(Node *node) {
    if (node == nullptr)
        return;

    print_inOrder(node->left);

    std::cout << node->value << " ";

    print_inOrder(node->right);
}

void print_postOrder(Node *node) {
    if (node == nullptr)
        return;

    print_postOrder(node->left);
    print_postOrder(node->right);

    std::cout << node->value << " ";
}

std::string leftPad(std::string str, unsigned int pads) {
    if (str.length() >= pads) return str;

    int nbrOfSpaces = pads - str.size();
    std::string spaces = "";

    for (int i = 0; i < nbrOfSpaces; i++) {
        spaces += " ";
    }

    return spaces + str;
}

std::array<int, 2> getValuesAtDepth(Node *root, int depth) {
    std::array<int, 2> result = {INT_MAX, INT_MAX};

    return result; 
}

void prettyPrint(Node *root) {
    int depth = root->depth() - 1; // -1 to start from 0

    std::cout << "Depth: " << depth <<"\n";
}

int main() {

    fprintf(stdout, "Hello from binary_tree.cpp\n");

    Node *root = new Node(2);
    Node *left = new Node(1);
    Node *right = new Node(3);

    root->left = left;
    root->right = right;

    left->left = new Node(4);
    right->left = new Node(5);
    right->right = new Node(6);

    prettyPrint(root);

    std::cout << "In order: ";
    print_inOrder(root);
    std::cout << "\n";
    std::cout << "Pre order: ";
    print_preOrder(root);
    std::cout << "\n";
    std::cout << "Post order: ";
    print_postOrder(root);
    std::cout << "\n";

    return 0;
}
