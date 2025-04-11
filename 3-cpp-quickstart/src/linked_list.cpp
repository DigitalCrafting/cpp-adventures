#include <cstdio>

struct Node {
    int value;
    Node* next;

    Node(int _val): value(_val), next(nullptr) {};
};

struct LinkedList {
    private:
        int length;
        Node* root;
        Node* current;

    public:
    LinkedList(): length(0), root(nullptr), current(nullptr){};

    void push(int _val) {
        if (!root) {
            root = new Node(_val);
            current = root;
        } else {
            current->next = new Node(_val);
            current = current->next;
        }
        length++;
    }

    int getAt(int index) {
        if (index > length - 1) {
            return -1;
        }


        Node* searched = root;
        for (int i = 0; i < index; i++) {
            searched = searched->next;
        }

        return searched->value;
    }

    int getSize() {
        return length;
    }
};

int main() {
    LinkedList list;
    list.push(1);
    list.push(2);
    list.push(3);
    list.push(4);
    list.push(5);
    list.push(6);

    printf("List size: %d\n", list.getSize());
    printf("5-th element: %d\n", list.getAt(4));

    return 0;
}
