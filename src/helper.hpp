#ifndef HELPER_CLASS_HPP
#define HELPER_CLASS_HPP
#include "constants/error_messages.hpp"
#include <iostream>
#include <string>

inline int read_int(const std::string& prompt) {
  std::string input;
  while (true) {
    std::cout << prompt;
    std::getline(std::cin, input);

    try {
      size_t pos;
      int value = std::stoi(input, &pos);
      // check if whole line was int
      if (pos != input.length()) {
        std::cout << error_msg::INVALID_INPUT_TYPE << std::endl;
        continue;
      }

      return value;
    } catch (...) {
      std::cout << error_msg::INVALID_INPUT_TYPE << std::endl;
    }
  }
}
#endif