#include <cstdio>

struct Node {
    int value;
    Node *left;
    Node *right;

    Node(int val): value(val), left(nullptr), right(nullptr) {};

    int depth() {
        int dLeft = 1;
        int dRight = 0;

        if (left) 
            dLeft = left->depth();

        if (right)
            dRight = right->depth();

        return (dLeft > dRight ? dLeft : dRight);
    }

};

void print(Node *root) {
    int depth = root->depth();

    fprintf(stdout, "Depth: %d\n", depth);
}

int main(int argc, char** args) {

    fprintf(stdout, "Hello from binary_tree.cpp\n");

    Node *root = new Node(2);
    Node *left = new Node(1);
    Node *right = new Node(3);

    root->left = left;
    root->right = right;

    print(root);

    return 0;
}
