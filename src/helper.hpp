#ifndef HELPER_CLASS_HPP
#define HELPER_CLASS_HPP
#include "constants/error_messages.hpp"
#include <iostream>
#include <limits>
#include <string>

namespace Helper {
inline unsigned int read_uint(const std::string& prompt) {
  std::string input;
  std::cout << prompt;
  std::getline(std::cin, input);

  size_t pos;
  unsigned long value;
  try {
    value = std::stoul(input, &pos);
  } catch (std::invalid_argument) {
    throw std::invalid_argument(error_msg::INVALID_INPUT_TYPE);
  } catch (std::out_of_range) { // should not be able to happen
    throw std::out_of_range("Value too long. Please try again.");
  }

  // ensure full consumption and range
  if (pos != input.length() || value > std::numeric_limits<unsigned int>::max())
    throw std::invalid_argument(error_msg::INVALID_INPUT_TYPE);

  return static_cast<unsigned int>(value);
}
} // namespace Helper
#endif