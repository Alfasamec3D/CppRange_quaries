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
  if (!s.top_) return 0;
  int result = 0;
  std::vector<const typename SearchTree<KeyT>::Node*> stack;
  stack.push_back(s.top_);
  while (!stack.empty()) {
    const typename SearchTree<KeyT>::Node* current = stack.back();
    stack.pop_back();
    if (current->key_ >= fst && current->key_ <= snd) ++result;

    if (current->left_ && current->key_ > fst) stack.push_back(current->left_);
    if (current->right_ && current->key_ < snd)
      stack.push_back(current->right_);
  }
  return result;
}

template <typename KeyT>
std::ostream& operator<<(std::ostream& os,
                         const Trees::SearchTree<KeyT>& tree) {
  int rows = tree.top_->height_ + 1;
  int cols = std::pow(2, tree.top_->height_ + 1) - 1;
  std::vector<std::vector<std::string>> ans(rows,
                                            std::vector<std::string>(cols, ""));

  tree.inorder(tree.top_, 0, (cols - 1) / 2, ans);

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
  enum Node_color { Red, Black };
  struct Node {
    KeyT key_;
    Node *parent_ = nullptr, *left_ = nullptr, *right_ = nullptr;
    Node_color color_ = Red;
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
  Node* top_ = nullptr;

 public:
  void insert(const KeyT& key) {
    Node* y = nullptr;
    Node* x = top_;
    while (x) {
      y = x;
      if (key < x->key_)
        x = x->left_;
      else if (key > x->key_)
        x = x->right_;
      else
        return;
    }
    Node* z = new Node{key};
    z->parent_ = y;
    if (!y)
      top_ = z;
    else if (z->key_ < y->key_)
      y->left_ = z;
    else
      y->right_ = z;
    insert_fixup(z);
  }

  friend int range_query<KeyT>(const SearchTree<KeyT>& s, const KeyT& fst,
                               const KeyT& snd);
  friend std::ostream& operator<< <KeyT>(std::ostream& os,
                                         const Trees::SearchTree<KeyT>& tree);

  SearchTree() = default;

  ~SearchTree() {
    if (!top_) return;

    std::vector<Node*> stack;
    stack.push_back(top_);

    while (!stack.empty()) {
      Node* current = stack.back();
      stack.pop_back();

      if (current->right_) stack.push_back(current->right_);
      if (current->left_) stack.push_back(current->left_);

      delete current;
    }
  }

  SearchTree(const SearchTree& other) {
    if (!other.top_) {
      top_ = nullptr;
      return;
    }
    top_ = new Node(other.top_->key_);
    top_->color_ = other.top_->color_;
    top_->height = other.top_->height_;

    std::vector<std::pair<const Node*, Node*>> stack;
    stack.push_back({other.top_, top_});

    while (!stack.empty()) {
      auto pair = stack.back();
      stack.pop_back();

      const Node* src = pair.first;
      Node* dest = pair.second;

      if (src->right_) {
        dest->right_ = new Node(src->right_->key_);
        dest->right_->color_ = src->right_->color_;
        dest->right_->height_ = src->right->height;
        dest->right_->parent_ = dest;

        stack.push_back({src->right_, dest->right_});
      }

      if (src->left_) {
        dest->left_ = new Node(src->left_->key_);
        dest->left_->color_ = src->left_->color_;
        dest->left_->height_ = src->left_->height_;
        dest->left_->parent_ = dest;

        stack.push_back({src->left_, dest->left_});
      }
    }
  }

  SearchTree& operator=(const SearchTree& other) {
    if (this == &other) return *this;
    SearchTree temp{other};
    std::swap(top_, temp.top_);
    return *this;
  }

  SearchTree(SearchTree&& other) noexcept : top_(other.top_) {
    other.top_ = nullptr;
  }

  SearchTree& operator=(SearchTree&& other) noexcept {
    if (this == &other) return *this;

    destroy_tree(top_);
    top_ = other.top_;
    other.top_ = nullptr;
    return *this;
  }

 private:
  void insert_fixup(Node* z) {
    while (z->parent_ && z->parent_->color_ == Red) {
      if (z->parent_ == z->parent_->parent_->left_) {
        // z->parent_->parent_ exists because if z->parent_ is top_, then
        // z->parent_ is black. Since we enter a loop iteration only if
        // z->parent_ is red, we know that z->parent_ can not be top_. hence
        // z->parent_->parent_exists

        // y - uncle of z
        Node* y = z->parent_->parent_->right_;
        // case 1:
        if (y && y->color_ == Red) {
          z->parent_->color_ = Black;
          y->color_ = Black;
          z->parent_->parent_->color_ = Red;
          z = z->parent_->parent_;
        } else {
          // case 2:
          if (z == z->parent_->right_) {
            z = z->parent_;
            leftRotate(z);
            // turned case 2 into case 3
          }
          // case 3:
          z->parent_->color_ = Black;
          z->parent_->parent_->color_ = Red;
          rightRotate(z->parent_->parent_);
        }
      } else {
        Node* y = z->parent_->parent_->left_;
        // case 1:
        if (y && y->color_ == Red) {
          z->parent_->color_ = Black;
          y->color_ = Black;
          z->parent_->parent_->color_ = Red;
          z = z->parent_->parent_;
        } else {
          // case 2:
          if (z == z->parent_->left_) {
            z = z->parent_;
            rightRotate(z);
            // turned case 2 into case 3
          }
          // case 3:
          z->parent_->color_ = Black;
          z->parent_->parent_->color_ = Red;
          leftRotate(z->parent_->parent_);
        }
      }
    }
    top_->color_ = Black;
  }

  void rightRotate(Node* y) {
    if (!y) return;
    Node* x = y->left_;
    if (!x)
      throw std::logic_error(
          "Unexpected null left node in tree rightRotation. The node is " +
          std::to_string(y->key_));

    // Rotation
    y->left_ = x->right_;
    if (x->right_) x->right_->parent_ = y;
    x->parent_ = y->parent_;
    if (!y->parent_)
      top_ = x;
    else if (y == y->parent_->right_)
      y->parent_->right_ = x;
    else
      y->parent_->left_ = x;
    x->right_ = y;
    y->parent_ = x;

    y->height_ = y->get_height();
    x->height_ = x->get_height();
  }

  void leftRotate(Node* x) {
    if (!x) return;
    Node* y = x->right_;
    if (!y)
      throw std::logic_error(
          "Unexpected null right node in tree leftRotation. The node is " +
          std::to_string(x->key_));

    // Rotation
    x->right_ = y->left_;
    if (y->left_) y->left_->parent_ = x;
    y->parent_ = x->parent_;
    if (!x->parent_)
      top_ = y;
    else if (x == x->parent_->left_)
      x->parent_->left_ = y;
    else
      x->parent_->right_ = y;
    y->left_ = x;
    x->parent_ = y;

    x->height_ = x->get_height();
    y->height_ = y->get_height();
  }

  void inorder(const Node* node, const int& row, const int& col,
               std::vector<std::vector<std::string>>& ans) const {
    if (!node) return;
    int offset = std::pow(2, top_->height_ - row - 1);
    if (node->left_) inorder(node->left_, row + 1, col - offset, ans);
    ans[row][col] = std::to_string(node->key_);
    if (node->right_) inorder(node->right_, row + 1, col + offset, ans);
  }
};
}  // namespace Trees

void end();