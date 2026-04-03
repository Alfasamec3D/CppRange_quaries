#include <gtest/gtest.h>

#include "trees.hpp"
using namespace Trees;

TEST(InsertTest, BasicCase) {
  SearchTree<int> tree;
  std::vector<int> keys = {1, 2, 3};
  for (const auto& key : keys) tree.insert(key);
  int result = range_query(tree, keys[2], keys[2]);
  int expected_result = 1;
  EXPECT_EQ(result, expected_result);
}

TEST(RangeQueryTest, BasicCase) {
  SearchTree<int> tree;
  std::vector<int> keys = {1, 2, 3};
  for (const auto& key : keys) tree.insert(key);
  int fst = 0;
  int snd = 4;
  int result = range_query(tree, fst, snd);
  int expected_result = 3;
  EXPECT_EQ(result, expected_result);
}

TEST(CopyConstructorTest, BasicCase) {
  SearchTree<int> tree;
  std::vector<int> keys = {1, 2, 3};
  for (const auto& key : keys) tree.insert(key);
  SearchTree<int> copy_tree{tree};
  int new_key = 4;
  tree.insert(new_key);

  // copy_tree got nodes of tree
  int result1 = range_query(copy_tree, keys[0], keys[2]);
  int expected_result1 = 3;
  EXPECT_EQ(result1, expected_result1);

  // old tree is valid and has old nodes
  int result2 = range_query(tree, keys[0], keys[2]);
  int expected_result2 = 3;
  EXPECT_EQ(result2, expected_result2);
}

TEST(CopyAssignmentTest, BasicCase) {
  SearchTree<int> tree;
  std::vector<int> keys = {1, 2, 3};
  for (const auto& key : keys) tree.insert(key);

  SearchTree<int> copy_tree;
  std::vector<int> other_keys = {10, 20, 30};
  for (const auto& key : other_keys) copy_tree.insert(key);
  copy_tree = tree;

  // copy_tree got nodes of tree
  int result1 = range_query(copy_tree, keys[0], keys[2]);
  int expected_result1 = 3;
  EXPECT_EQ(result1, expected_result1);

  // old tree is valid and has old nodes
  int result2 = range_query(tree, keys[0], keys[2]);
  int expected_result2 = 3;
  EXPECT_EQ(result2, expected_result2);

  // copy_tree lost old nodes
  int result3 = range_query(copy_tree, other_keys[0], other_keys[2]);
  int expected_result3 = 0;
  EXPECT_EQ(result3, expected_result3);
}

TEST(MoveConstructorTest, BasicCase) {
  SearchTree<int> tree;
  std::vector<int> keys = {1, 2, 3};
  for (const auto& key : keys) tree.insert(key);

  SearchTree<int> move_tree{std::move(tree)};

  // move_tree have nodes of tree
  int result1 = range_query(move_tree, keys[0], keys[2]);
  int expected_result1 = 3;
  EXPECT_EQ(result1, expected_result1);

  // old tree is valid and has no nodes
  int result2 = range_query(tree, keys[0], keys[2]);
  int expected_result2 = 0;
  EXPECT_EQ(result2, expected_result2);
}

TEST(MoveAssignmentTest, BasicCase) {
  SearchTree<int> tree;
  std::vector<int> keys = {1, 2, 3};
  for (const auto& key : keys) tree.insert(key);

  SearchTree<int> move_tree;
  std::vector<int> other_keys = {10, 20, 30};
  for (const auto& key : other_keys) move_tree.insert(key);
  move_tree = std::move(tree);

  // move_tree got nodes of tree
  int result1 = range_query(move_tree, keys[0], keys[2]);
  int expected_result1 = 3;
  EXPECT_EQ(result1, expected_result1);

  // old tree is valid and has no nodes
  int result2 = range_query(tree, keys[0], keys[2]);
  int expected_result2 = 0;
  EXPECT_EQ(result2, expected_result2);

  // move_tree lost old nodes
  int result3 = range_query(move_tree, other_keys[0], other_keys[2]);
  int expected_result3 = 0;
  EXPECT_EQ(result3, expected_result3);
}