#pragma once

#include <fstream>
#include <sstream>
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
     
        inline std::string read_all(std::string fullPath) {
            std::ifstream f(fullPath.c_str());
            std::ostringstream ss;
            ss << f.rdbuf();
            return ss.str();
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
}// end of namespace FW