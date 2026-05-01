#include "StockBST.h"
#include <iostream>


    StockBST::BSTNode* StockBST::insertHelper(BSTNode* node, const string& ticker, double key, int year) {
        if (node == nullptr) { //if nullptr, then sorted
            return new BSTNode(ticker, key, year);
        }
        if (key < node->key)  //if node has data, sort by lower values going left or higher/equal values going right
            node->left = insertHelper(node->left, ticker, key, year);
        else { // duplicates go right
            node->right = insertHelper(node->right, ticker, key, year);
        }
        return node;
    }

    StockBST::BSTNode* StockBST::searchHelper(BSTNode* node, double key) const {
        if (node == nullptr) {
            return nullptr; //not found
        }
        if (key == node->key) {
            return node; //match found
        }
        if (key < node->key) {
            return searchHelper(node->left, key);
        } else {
            return searchHelper(node->right, key);
        }
    }
    

    void    StockBST::rangeSearchHelper(BSTNode* node, double low, double high, vector<BSTNode*>& results) const {
        // collects all nodes where low <= key <= high
        if (node == nullptr) {
            return; //end at end of tree
        }

        if (low < node->key) {
            rangeSearchHelper(node->left, low, high, results); //if low is less than key, then there could be more nodes that match and call a search
        }
        if (low <= node->key && node->key <= high) { //if this node matches, then add to vector
            results.push_back(node);
        }
        if (node->key <= high) {
            rangeSearchHelper(node->right, low, high, results); //if high is greater than or equal to key, then there could be more nodes that match and call a search
        }
    } //collects sorted

    void    StockBST::inorderHelper(BSTNode* node)  const { 
        if (node==nullptr)
        return;
        inorderHelper(node->left); //by accessing left first, you print the lowest value first and go up
        cout << node->ticker << " (" << node->key << ")\n";
        inorderHelper(node->right);
    }

    void    StockBST::preorderHelper(BSTNode* node) const { 
        if (node==nullptr) {
            return;
        }

        cout << node->ticker << " (" << node->key << ")\n"; //root always prints first
        preorderHelper(node->left);
        preorderHelper(node->right);
    }

    void    StockBST::postorderHelper(BSTNode* node) const {
        if (node==nullptr) {
            return;
        }

        postorderHelper(node->left);
        postorderHelper(node->right);
        cout << node->ticker << " (" << node->key << ")\n"; //root always prints last
    }

    int     StockBST::heightHelper(BSTNode* node)   const {
        if (node == nullptr) {
            return -1;
        }
        int leftH = heightHelper(node->left);
        int rightH = heightHelper(node->right);
        return 1+max(leftH, rightH);
    }

    void    StockBST::clearHelper(BSTNode* node) {
        if (node == nullptr) {
            return;
        }
        clearHelper(node->left);
        clearHelper(node->right);
        delete node; //always access nodes children first and delete the parent last
    }


    StockBST::StockBST() {
        root = nullptr;
    }
    StockBST::~StockBST() {
        clear();
    }

    // Insert a new entry. Duplicates allowed (go to right subtree).
    void StockBST::insert(const string& ticker, double key, int year) {
        root = insertHelper (root, ticker, key, year);
    }

    // Return pointer to a node whose key exactly matches. nullptr if not found.
    StockBST::BSTNode* StockBST::search(double key) const {
        return searchHelper(root, key);
    }

    // Collect all nodes where low <= key <= high into 'results' vector.
    void StockBST::rangeSearch(double low, double high, vector<BSTNode*>& results) const {
        rangeSearchHelper(root, low, high, results);
        return;
    }

    // Tree traversals — each prints: ticker (key) per line
    void StockBST::inorder()   const {
        inorderHelper(root);
    }   // sorted ascending by key
    void StockBST::preorder()  const { 
        preorderHelper(root);
    }
    void StockBST::postorder() const {
        postorderHelper(root);
    }

    // Returns the number of edges on the longest root-to-leaf path. -1 if empty.
    int StockBST::getHeight() const {
        return heightHelper(root);
    }

    // Finds and returns the node with the maximum key (rightmost node).
    // Returns nullptr if tree is empty.
    StockBST::BSTNode* StockBST::findMax() const {
        if (root == nullptr) {
            return nullptr;
        }
        BSTNode* curr = root;
        while (curr->right != nullptr) {
            curr = curr->right;
        }
        return curr;
    }

    // Frees all nodes. Resets root to nullptr.
    void StockBST::clear() {
        clearHelper(root);
        root = nullptr; //clear root once all data deleted
    }