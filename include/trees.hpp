#pragma once
#include <algorithm>
#include <iostream>
namespace Trees {

template <typename KeyT>
class SearchTree;

template <typename KeyT>
int range_query(const SearchTree<KeyT>& s, const KeyT& fst, const KeyT& snd) {
  int result{0};
  s.count(result, s.top, fst, snd);
  return result;
}

template <typename KeyT>
class SearchTree {
  struct Node {
    KeyT key_;
    Node *left_ = nullptr, *right_ = nullptr;
    int height_ = 0;
    Node(KeyT key) : key_(key) {};
  };
  
  Node* top = nullptr;

 public:
  void count(int& counter, const Node* node, const KeyT& key1,
             const KeyT& key2) const {
    if (node->key_ >= key1 && node->key_ <= key2) {
      counter++;
      if (node->left_) {
        count(counter, node->left_, key1, key2);
      }
      if (node->right_) {
        count(counter, node->right_, key1, key2);
      }
    }
    if (node->key_ < key1 && node->right_) {
      count(counter, node->right_, key1, key2);
    }
    if (node->key_ > key2 && node->left_) {
      count(counter, node->left_, key1, key2);
    }
    return;
  }
  void insert(const KeyT& key) { top = recur_insert(top, key); }
  friend int range_query<KeyT>(const SearchTree<KeyT>& s, const KeyT& fst,
                               const KeyT& snd);

 private:
  Node* recur_insert(Node* node, const KeyT& key) {
    if (node == nullptr) return new Node(key);
    if (key < node->key_)
      node->left_ = recur_insert(node->left_, key);
    else if (key > node->key_)
      node->right_ = recur_insert(node->right_, key);
    else  // Equal keys are not allowed
      return node;
    node->height_ = 1 + std::max(node->left_->height_, node->right_->height_);
    int balance;

    // balance factor of the node
    if (node == nullptr)
      balance = 0;
    else
      balance = node->left_->height_ - node->right_->height_;
    // Left left
    if (balance > 1 && key < node->left_->key_) return rightRotate(node);
    // Right right
    if (balance < -1 && key > node->right_->key_) return leftRotate(node);
    // Left right
    if (balance > 1 && key > node->left_->key_) {
      node->left_ = leftRotate(node->left_);
      return rightRotate(node);
    }
    // Right left
    if (balance < -1 && key < node->right_->key_) {
      node->right_ = leftRotate(node->right_);
      return leftRotate(node);
    }
    return node;
  }
  
  Node* rightRotate(Node* y) {
    Node* x = y->left_;

    // Rotation
    y->left_ = x->right_;
    x->right_ = y;

    x->height_ = 1 + std::max(x->left_->height_, x->right_->height_);
    y->height_ = 1 + std::max(y->left_->height_, y->right_->height_);

    return x;
  }

  Node* leftRotate(Node* x) {
    Node* y = x->right_;

    // Rotation
    x->right_ = y->left_;
    y->left_ = x;

    x->height_ = 1 + std::max(x->left_->height_, x->right_->height_);
    y->height_ = 1 + std::max(y->left_->height_, y->right_->height_);

    return y;
  }
};
}  // namespace Trees

void end() {
  Trees::SearchTree<int> tree{};
  char mode;
  int key, key1, key2;
  while (std::cin >> mode) {
    switch (mode) {
      case 'k':
        std::cin >> key;
        tree.insert(key);
        break;
      case 'q':
        std::cin >> key1;
        std::cin >> key2;
        std::cout << std::endl << Trees::range_query(tree, key1, key2);
        break;

      default:
        break;
    }
  }
  return;
}