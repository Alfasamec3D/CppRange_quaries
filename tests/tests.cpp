#include <gtest/gtest.h>

#include <fstream>
#include <sstream>

#include "trees.hpp"

TEST(EndFunctionTest, BasicScenario) {
  std::ifstream file("tests/tests.txt");
  ASSERT_TRUE(file.is_open()) << "Cannot open tests.txt";

  std::string input;
  std::string expected;

  while (std::getline(file, input)) {
    ASSERT_TRUE(std::getline(file, expected))
        << "Missing expected output for input: " << input;

    std::istringstream input_stream(input);
    std::ostringstream output_stream;

    auto* cin_backup = std::cin.rdbuf(input_stream.rdbuf());
    auto* cout_backup = std::cout.rdbuf(output_stream.rdbuf());
    try {
      end();
    } catch (const std::exception& e) {
      std::cerr << "Logic error caught: " << std::endl << input;
    }
    std::cin.rdbuf(cin_backup);
    std::cout.rdbuf(cout_backup);

    EXPECT_EQ(output_stream.str(), expected);
  }
}