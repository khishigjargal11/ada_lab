#include <iostream>
#include <vector>
#include <unordered_set>
#include <cstdlib>
#include <ctime>

using namespace std;
 
struct BSTNode {
    int key;
    BSTNode *left, *right;
    BSTNode(int val) : key(val), left(nullptr), right(nullptr) {}
};

class RandomizedBST {
public:
    BSTNode* root = nullptr;
    BSTNode* insert(BSTNode* node, int key) {
        if (!node) return new BSTNode(key);
        if (rand() % 2) node->left = insert(node->left, key);
        else node->right = insert(node->right, key);
        return node;
    }

    void insert(int key) { root = insert(root, key); }

    int search(int key) {
        BSTNode* node = root;
        int steps = 0;
        while (node) {
            steps++;
            if (key == node->key) return steps;
            node = (key < node->key) ? node->left : node->right;
        }
        return steps;
    }
};

const int MAX_LEVEL = 16;

struct SkipNode {
    int key;
    vector<SkipNode*> forward;
    SkipNode(int k, int level) : key(k), forward(level, nullptr) {}
};

class SkipList {
    float p = 0.5;
    int level = 1;
    SkipNode* header = new SkipNode(-1, MAX_LEVEL);

    int randomLevel() {
        int lvl = 1;
        while ((rand() / double(RAND_MAX)) < p && lvl < MAX_LEVEL) lvl++;
        return lvl;
    }

public:
    void insert(int key) {
        vector<SkipNode*> update(MAX_LEVEL);
        SkipNode* curr = header;
        for (int i = level - 1; i >= 0; i--) {
            while (curr->forward[i] && curr->forward[i]->key < key)
                curr = curr->forward[i];
            update[i] = curr;
        }

        int lvl = randomLevel();
        if (lvl > level) {
            for (int i = level; i < lvl; i++) update[i] = header;
            level = lvl;
        }

        SkipNode* node = new SkipNode(key, lvl);
        for (int i = 0; i < lvl; i++) {
            node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = node;
        }
    }

    int search(int key) {
        SkipNode* curr = header;
        int steps = 0;
        for (int i = level - 1; i >= 0; i--) {
            while (curr->forward[i] && curr->forward[i]->key < key) {
                curr = curr->forward[i];
                steps++;
            }
        }
        curr = curr->forward[0];
        steps++;
        return steps;
    }
};


void testStructures(int n) {
    unordered_set<int> unique_keys;
    while (unique_keys.size() < n) unique_keys.insert(rand());

    vector<int> keys(unique_keys.begin(), unique_keys.end());
    RandomizedBST bst;
    SkipList skip;

    for (int key : keys) {
        bst.insert(key);
        skip.insert(key);
    }

    int total_steps_bst = 0;
    int total_steps_skip = 0;
    for (int i = 0; i < 1'000'000; ++i) {
        int key = (rand() % 2 == 0) ? keys[rand() % n] : rand() + 1'000'000'000;
        total_steps_bst += bst.search(key);
        total_steps_skip += skip.search(key);
    }

    cout << "n = " << n
         << " | BST avg steps: " << (double)total_steps_bst / 1'000'000
         << " | SkipList avg steps: " << (double)total_steps_skip / 1'000'000 << endl;
}

int main() {
    srand(time(0));
    cout << "Randomized BST vs SkipList\n";
    for (int n : {5'000'000, 10'000'000}) {
        testStructures(n);
    }
    return 0;
}
