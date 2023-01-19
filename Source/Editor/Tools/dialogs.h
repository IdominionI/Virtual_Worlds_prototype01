#pragma once

#include <Universal/tinyFileDialog/tinyfiledialogs.h>

#include <Universal/Framework/FWstring_tools.h>
/*
                APPLICATION DIALOGUE FUNCTIONS

   This is a series of functions to act as a gateway to the third party
   tinyfiledialogs tools that give the functionality similar to Qt file
   dialogue and QMessage classes.

   The file and folder dialogues give a pathnmae to a files and directories
   to load and save data.

   The MessageBox dialogue gives access to interactivly display a popup 
   message box in which to inform the user of some aspect of the applications
   functioning, or request the user to perform some task.
*/


namespace vwDialogs {

    //char const* open_file(const char* startDir, char const* patterns[], int numPattern){
    inline char const* open_file(const char* startDir, char const* patterns[], int numPattern){
        return tinyfd_openFileDialog("Load File...", startDir, numPattern, patterns, NULL, 0);
    }

    inline char const* save_file(const char* startDir, char const* patterns[], int numPattern){
        return tinyfd_saveFileDialog("Save File...", startDir, numPattern, patterns, NULL);
    }

    inline char const* select_directory(const char* startDir){
        char const *dirpath = tinyfd_selectFolderDialog("Select directory ... ", startDir);

        if (!dirpath) return "";// Did not select a directory

        return dirpath;
    }

    /* Structure of the tinyfd_messageBox parameters that are needed to be defined
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


    // Function to seperate a filename from the given file pathname as a standard C++ string

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