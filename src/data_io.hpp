#ifndef DATA_IO_CLASS_HPP
#define DATA_IO_CLASS_HPP

#include "field.hpp"
#include "user.hpp"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <regex.h>
#include <string>
#include <vector>

class DataIO {

public:
  void export_data(std::vector<std::string> field, int& mode, std::vector<User*>& players, int& active_user) {

    std::cout << "saving game... " << std::endl;
    std::ofstream file(file_name, std::ios_base::trunc);

    if (!file.is_open()) {
      printf("failed :(\n");
      return;
    }

    // save field
    for (int i = 0; i < rows; ++i) {
      file.write(field[i].data(), field[i].size());
    }

    // save mode
    file.write(reinterpret_cast<char*>(&mode), sizeof(mode));

    // save players_data
    std::vector<int> stones_data{players[0]->get_stones_removed(),
                                 players[0]->get_stones_set(),
                                 players[1]->get_stones_removed(),
                                 players[1]->get_stones_set()};
    for (auto stone_data : stones_data) {
      file.write(reinterpret_cast<const char*>(stone_data),
                 sizeof(int));
    }

    // save active_user
    file.write(reinterpret_cast<char*>(&active_user), sizeof(active_user));

    file.close();

    printf("succeeded :)\n\tfilename: %s\n", file_name.c_str());
  }

  void import_data(Field& field, int& mode, std::vector<int>& stones_data, int& active_user) {
    std::vector<std::string> field_vector;
    std::cout << "loading game... " << std::endl;
    std::ifstream file(file_name, std::ios_base::binary);

    if (!file.is_open()) {
      printf("failed :(\n");
      return;
    }

    // read field
    for (int i = 0; i < rows; ++i) {
      std::string row(cols, '\0');
      file.read(&row[0], cols);
      field_vector.push_back(row);
    }

    field = Field(field_vector);

    // read mode
    file.read(reinterpret_cast<char*>(&mode), sizeof(mode));
    // read players_data
    size_t size;
    file.read(reinterpret_cast<char*>(&size), sizeof(size));
    for (size_t i = 0; i < size; ++i) {
      file.read(reinterpret_cast<char*>(&stones_data[i]), sizeof(int));
    }
    // read active_user
    file.read(reinterpret_cast<char*>(&active_user), sizeof(active_user));

    file.close();

    printf("succeeded :)\n\tfilename: %s\n", file_name.c_str());
  }

private:
  std::string file_name = "saved/data.bin";
  int rows = 15;
  int cols = 26;
};

#endif