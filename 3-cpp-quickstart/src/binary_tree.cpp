#include <climits>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#define MyNAN std::numeric_limits<int>::quiet_NaN()

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

    int getLeftChildValue() {
        if (left) {
            return left->value;
        }

        return MyNAN;
    }

    int getRightChildValue() {
        if (right) {
            return right->value;
        }
        return MyNAN;
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

/*
 * Struct representing a touple of values - left and right.
 *
 * Each depth of the Binary Tree will consist of multiple such pairs.
 *
 * */
struct ValueAtDepth {
    int left;
    int right;

    ValueAtDepth(int _left, int _right): left(_left), right(_right){}
};

std::vector<ValueAtDepth> getValuesAtDepth(Node *root, int depth) {
    /*
     * If current node is nullptr, then we need to fill the childrend to preserve spacing
     */
    if (root == nullptr) {
        std::vector<ValueAtDepth> result;
        for (int i = 0; i <= (2^depth); i++) {
            result.push_back(ValueAtDepth(MyNAN, MyNAN));
        }
        return result;
    }


    if (depth == 0) {
        return std::vector<ValueAtDepth>{ValueAtDepth(root->value, MyNAN)};
    }

    if (depth == 1) {
        int leftVal = root->getLeftChildValue();
        int rightVal = root->getRightChildValue();

        return std::vector<ValueAtDepth>{ValueAtDepth(leftVal, rightVal)};
    }

    auto leftValues = getValuesAtDepth(root->left, depth - 1);
    auto rightValues = getValuesAtDepth(root->right, depth - 1);
    std::vector<ValueAtDepth> result = {};
    result.insert(result.end(), leftValues.begin(), leftValues.end());
    result.insert(result.end(), rightValues.begin(), rightValues.end());
    return result; 
}

void prettyPrint(Node *root) {
    int depth = root->depth() - 1; // -1 to start from 0

    for (int i = 0; i <= depth; i++) {
        auto values = getValuesAtDepth(root, i);
        for (auto val : values) {
            std::cout << (val.left != MyNAN ? std::to_string(val.left) : " - "); 
            std::cout << " "; 
            std::cout << (val.right != MyNAN ? std::to_string(val.right) : " - "); 
            std::cout << " ";
        }
        std::cout << "\n";
    }

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
