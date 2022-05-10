#pragma once

#include <Universal/tinyFileDialog/tinyfiledialogs.h>

namespace vwDialogs {

    char const* open_file(const char* startDir, char const* patterns[], int numPattern){
        return tinyfd_openFileDialog("Load File...", startDir, numPattern, patterns, NULL, 0);
    }

    char const* save_file(const char* startDir, char const* patterns[], int numPattern){
        return tinyfd_saveFileDialog("Save File...", startDir, numPattern, patterns, NULL);
    }

    char const* select_directory(const char* startDir){
        return tinyfd_selectFolderDialog("Select directory ... ", startDir);
    }

    /*
    int tinyfd_messageBox (
        char const * const aTitle , // ""
        char const * const aMessage , // "" may contain \n \t
        char const * const aDialogType , // "ok" "okcancel" "yesno" "yesnocancel"
        char const * const aIconType , // "info" "warning" "error" "question"
        int const aDefaultButton ) ;
        // 0 for cancel/no , 1 for ok/yes , 2 for no in yesnocancel
    */

    inline int message_box(const char*  title = "", const char* message="", const char* dialog_type="ok", const char* icon_type="info", int default_button=0) {
        return tinyfd_messageBox(title, message, dialog_type, icon_type,default_button);
    }


    inline std::string get_filename(std::string file_pathname, std::string deliminator) {
        int index = FW::stringtools::lastIndexOf(file_pathname, deliminator);

        if (index < -1 || index + 1 >= file_pathname.length()) return "UNDEFINED";

        std::string filename = file_pathname.substr(index + 1, file_pathname.length());
       
        index = FW::stringtools::lastIndexOf(filename, "."); // get rid of file type suffix

//printf("get_filename %s %i\n", filename.c_str(), index);

        filename = filename.substr(0, index);

        return filename;
    }



}