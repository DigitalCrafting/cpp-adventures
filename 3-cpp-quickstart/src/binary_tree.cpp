#include <cstdio>

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

void print(Node *root) {
    int depth = root->depth() - 1; // -1 to start from 0

    fprintf(stdout, "Depth: %d\n", depth);
}

int main(int argc, char** args) {

    fprintf(stdout, "Hello from binary_tree.cpp\n");

    Node *root = new Node(2);
    Node *left = new Node(1);
    Node *right = new Node(3);

    root->left = left;
    root->right = right;

    left->left = new Node(4);

    print(root);

    return 0;
}
