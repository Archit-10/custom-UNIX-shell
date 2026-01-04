#include <iostream>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <dirent.h>

using namespace std;

bool search(string& present_dir, string& name) {
    DIR* dir = opendir(present_dir.c_str());

    if (!dir) {
        return false;
    }
    
    dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        string entry_name = entry->d_name;
   
        if (entry_name == "." || entry_name == "..") {
            continue;
        }
        string entry_path = present_dir + "/" + entry_name;
      
        if (entry->d_type == DT_DIR) {
            if (search(entry_path, name)) {
               
                closedir(dir);
                return true;
            }
        } else {
           
            if (entry_name == name) {
             
                closedir(dir);
                return true;
            }
        }
    }

    closedir(dir);
    return false;
}

bool search(string& name) {
    string present_dir = ".";  
    return search(present_dir,name);
}



