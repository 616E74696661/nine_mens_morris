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
  void export_data(std::vector<std::string> field, int& mode, std::vector<User*>& players, int& iteration) {

    std::cout << "saving game... " << std::endl;
    std::ofstream file(file_name, std::ios_base::trunc);

    if (!file.is_open()) {
      printf("failed :(\n");
      return;
    }

    // save field
    for (int i = 0; i < rows; ++i) {
      std::string row = field[i];
      row.resize(cols, ' ');
      file.write(row.data(), cols);
    }

    // save mode
    file.write(reinterpret_cast<char*>(&mode), sizeof(mode));

    // save players_data
    std::vector<int> stones_data{players[0]->get_stones_set(),
                                 players[0]->get_stones_removed(),
                                 players[1]->get_stones_set(),
                                 players[1]->get_stones_removed()};

    for (auto stone_data : stones_data) {
      file.write(reinterpret_cast<const char*>(&stone_data), sizeof(int));
    }

    // save active_user
    file.write(reinterpret_cast<char*>(&iteration), sizeof(iteration));

    file.close();

    printf("succeeded :)\n\tfilename: %s\n", file_name.c_str());
  }

  void import_data(Field& field, int& mode, std::vector<int>& stones_data, int& iteration) {
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

    // read stones_data
    for (int i = 0; i < 4; i++) {
      int stone_data;
      file.read(reinterpret_cast<char*>(&stone_data),
                sizeof(int));
      stones_data.push_back(stone_data);
    }

    file.read(reinterpret_cast<char*>(&iteration), sizeof(iteration));

    file.close();

    printf("succeeded :)\n\tfilename: %s\n", file_name.c_str());
  }

private:
  std::string file_name = "saved/data.bin";
  int rows = 15;
  int cols = 26;
};

#endif