#include "trees.hpp"

int main() {
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
        std::cout << Trees::range_query(tree, key1, key2) << ' ';
        break;

      default:
        return 1;
    }
  }
  return 0;
}