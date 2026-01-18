#ifndef HELPER_CLASS_HPP
#define HELPER_CLASS_HPP
#include "constants/error_messages.hpp"
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

#ifdef _WIN32
const std::string clear_cmd = "cls";
#else
const std::string clear_cmd = "clear";
#endif

namespace Helper {
static const int ANIMATION_TIME = 10;

struct close_game : std::runtime_error {
  close_game() : std::runtime_error("close game") {}
};

inline int read_uint(const std::string& prompt) {
  std::string input;
  std::cout << prompt;
  std::getline(std::cin, input);

  size_t pos;
  long value;
  if (input == "stop")
    throw close_game();
  try {
    value = std::stoul(input, &pos);
  } catch (std::invalid_argument) {
    throw std::invalid_argument(error_msg::INVALID_INPUT_TYPE);
  } catch (std::out_of_range) { // should not be able to happen
    throw std::out_of_range("Value too long. Please try again.");
  }

  // ensure full consumption and range
  if (pos != input.length() || value > std::numeric_limits<int>::max())
    throw std::invalid_argument(error_msg::INVALID_INPUT_TYPE);

  return static_cast<int>(value);
}

/**
 * @brief Clears the console
 *
 */
inline void clear_console() {
  system(clear_cmd.c_str());
  std::cout << std::endl;
}

} // namespace Helper
#endif