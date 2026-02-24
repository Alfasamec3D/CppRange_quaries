#pragma once
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

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
std::ostream& operator<<(std::ostream& os,
                         const Trees::SearchTree<KeyT>& tree) {
  int rows = tree.top->height_ + 1;
  int cols = std::pow(2, tree.top->height_ + 1) - 1;
  std::vector<std::vector<std::string>> ans(rows,
                                            std::vector<std::string>(cols, ""));

  tree.inorder(tree.top, 0, (cols - 1) / 2, ans);

  for (auto& row : ans) {
    for (auto& cell : row) {
      if (cell.empty())
        os << std::setw(6) << ' ';
      else
        os << std::setw(6) << cell;
    }
    os << std::endl;
  }
  return os;
}

template <typename KeyT>
class SearchTree {
  struct Node {
    KeyT key_;
    Node *left_ = nullptr, *right_ = nullptr;
    int height_ = 0;
    int get_height() const {
      if (!left_ && !right_)
        return 0;
      else if (!left_ && right_)
        return 1 + right_->height_;
      else if (left_ && !right_)
        return 1 + left_->height_;
      else
        return 1 + std::max(left_->height_, right_->height_);
    }
    Node(KeyT key) : key_(key) {};
  };
  Node* top = nullptr;

 public:
  void insert(const KeyT& key) { top = recur_insert(top, key); }

  friend int range_query<KeyT>(const SearchTree<KeyT>& s, const KeyT& fst,
                               const KeyT& snd);
  friend std::ostream& operator<< <KeyT>(std::ostream& os,
                                         const Trees::SearchTree<KeyT>& tree);

 private:
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

  Node* recur_insert(Node* node, const KeyT& key) {
    if (node == nullptr) return new Node{key};
    if (key < node->key_)
      node->left_ = recur_insert(node->left_, key);
    else if (key > node->key_)
      node->right_ = recur_insert(node->right_, key);
    else  // Equal keys are not allowed
      return node;

    node->height_ = node->get_height();

    // balance factor of the node
    int balance;

    if (!node->left_ && !node->right_)
      balance = 0;
    else if (!node->left_ && node->right_)
      balance = -1 - node->right_->height_;
    else if (node->left_ && !node->right_)
      balance = node->left_->height_ - -1;
    else
      balance = node->left_->height_ - node->right_->height_;

    try {
      // Left left
      if (balance > 1 && key < node->left_->key_) return rightRotate(node);
    } catch (const std::logic_error& e) {
      std::cerr << "Invalid left left rotation" << e.what() << ' ' << key
                << std::endl;
      throw std::exception{};
    }
    try {
      // Right right
      if (balance < -1 && key > node->right_->key_) return leftRotate(node);
    } catch (const std::logic_error& e) {
      std::cerr << "Invalid Right right rotation" << e.what() << ' ' << key
                << std::endl;
      throw std::exception{};
    }
    // Left right
    try {
      if (balance > 1 && key > node->left_->key_) {
        node->left_ = leftRotate(node->left_);
        return rightRotate(node);
      }
    } catch (const std::logic_error& e) {
      std::cerr << std::endl
                << "Invalid left right rotation" << e.what() << ' ' << key
                << std::endl;
      throw std::exception{};
    }
    try {
      // Right left
      if (balance < -1 && key < node->right_->key_) {
        node->right_ = rightRotate(node->right_);
        return leftRotate(node);
      }
    } catch (const std::logic_error& e) {
      std::cerr << std::endl
                << "Invalid right left rotation of " << node->key_ << ": "
                << e.what() << " Tried to insert key " << key << std::endl;
      throw std::exception{};
    }
    return node;
  }

  Node* rightRotate(Node* y) {
    if (!y) return y;
    Node* x = y->left_;
    if (!x)
      throw std::logic_error(
          "Unexpected null node in tree rightRotation. The node is " +
          std::to_string(y->key_));

    // Rotation
    y->left_ = x->right_;
    x->right_ = y;

    y->height_ = y->get_height();
    x->height_ = x->get_height();

    return x;
  }

  Node* leftRotate(Node* x) {
    if (!x) return x;
    Node* y = x->right_;
    if (!y)
      throw std::logic_error(
          "Unexpected null node in tree leftRotation. The node is " +
          std::to_string(x->key_));

    // Rotation
    x->right_ = y->left_;
    y->left_ = x;

    x->height_ = x->get_height();
    y->height_ = y->get_height();

    return y;
  }

  void inorder(const Node* node, const int& row, const int& col,
               std::vector<std::vector<std::string>>& ans) const {
    if (!node) return;
    int offset = std::pow(2, top->height_ - row - 1);
    if (node->left_) inorder(node->left_, row + 1, col - offset, ans);
    ans[row][col] = std::to_string(node->key_);
    if (node->right_) inorder(node->right_, row + 1, col + offset, ans);
  }
};
}  // namespace Trees

void end();