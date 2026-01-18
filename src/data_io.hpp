#ifndef DATA_IO_CLASS_HPP
#define DATA_IO_CLASS_HPP

#include "field.hpp"
#include "user.hpp"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <regex.h>
#include <string>
#include <sys/stat.h>
#include <vector>

class DataIO {

public:
  /**
   * @brief Export the game data to .bin file
   *
   * @param field
   * @param gamemode
   * @param players
   * @param iteration counter to get the last active player
   */
  bool export_data(std::vector<std::string> field, int& gamemode, std::vector<User*>& players, int& iteration) {
    // unimportant variable to check for directory
    struct stat sb;
    // if directory does not exist -> create the directory
    if (stat(dir.c_str(), &sb) != 0) {
      // OS-dependant
#if defined(_WIN32)
      int rc = mkdir(dir.c_str());
#else
      int rc = mkdir(dir.c_str(), 0777);
#endif
      if (rc != 0)
        return false; // directory not created}
    }

    std::ofstream file(dir + file_name, std::ios_base::trunc);
    // check if file created successful
    // note: file is always getting created
    //       if there was already a file with the same name -> deletes it and creates a new one
    if (!file.is_open()) {
      return false; // file not created
    }

    // save field
    for (int i = 0; i < rows; ++i) {
      std::string row = field[i];
      row.resize(cols, ' ');
      file.write(row.data(), cols);
    }

    // save mode
    file.write(reinterpret_cast<char*>(&gamemode), sizeof(gamemode));

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

    return true; // success
  }

  /**
   * @brief Import .bin file to get latest gamestate
   *
   * @param field
   * @param gamemode
   * @param stones_data
   * @param iteration counter to get the last active player
   * @return true if import successful
   * @return false if file does not exist
   */
  bool
  import_data(Field& field, int& gamemode, std::vector<int>& stones_data, int& iteration) {
    std::vector<std::string> field_vector;
    std::ifstream file(dir + file_name, std::ios_base::binary);

    if (!file.is_open()) {
      return false; // file does not exist
    }

    // get field
    for (int i = 0; i < rows; ++i) {
      std::string row(cols, '\0');
      file.read(&row[0], cols);
      field_vector.push_back(row);
    }
    field = Field(field_vector);

    // get mode
    file.read(reinterpret_cast<char*>(&gamemode), sizeof(gamemode));

    // read stones_data
    for (int i = 0; i < 4; i++) {
      int stone_data;
      file.read(reinterpret_cast<char*>(&stone_data),
                sizeof(int));
      stones_data.push_back(stone_data);
    }

    file.read(reinterpret_cast<char*>(&iteration), sizeof(iteration));

    file.close();

    return true; // success
  }

private:
  const std::string dir = "saved";
  const std::string file_name = "/gamestate.bin";
  int rows = 15;
  int cols = 26;
};

#endif