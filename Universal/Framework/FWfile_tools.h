#pragma once

#include <fstream>
#include <filesystem>
#include <string>
#include <sys/stat.h>

// Essential to have functions defined as inline functions

namespace FW {
    namespace filetools {

        inline std::string read_all(std::fstream &stream) { // need this variable to be referened
            std::string file_text(std::istreambuf_iterator<char>{stream}, {});
            return file_text; // Cannot return above directly
        }
     
        inline bool directory_exists(std::string dir_pathname) {
            struct stat buffer;
            return (stat(dir_pathname.c_str(), &buffer) == 0);
        }

        inline bool file_exists(std::string file_pathname) {
             return std::filesystem::exists(file_pathname);
            //return false;
        }

    }
}