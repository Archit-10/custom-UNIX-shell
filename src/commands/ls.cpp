#include <iostream>
#include <vector>
#include <sstream>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <ctime>

using namespace std;


void ls(const string &path, bool hidden_files, bool long_format) {
    DIR *dir = opendir(path.c_str());
    if (dir == nullptr) {
        perror("opendir");
        return;
    }
    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr) {   
        if (!hidden_files && entry->d_name[0] == '.') {
            continue;
        }
        struct stat file_info;
        string entry_path = path + "/" + entry->d_name;
        if (long_format && stat(entry_path.c_str(), &file_info) == 0) {
           
            cout << (S_ISDIR(file_info.st_mode) ? "d" : "-");
            cout << ((file_info.st_mode & S_IRUSR) ? "r" : "-");
            cout << ((file_info.st_mode & S_IWUSR) ? "w" : "-");
            cout << ((file_info.st_mode & S_IXUSR) ? "x" : "-");
            cout << ((file_info.st_mode & S_IRGRP) ? "r" : "-");
            cout << ((file_info.st_mode & S_IWGRP) ? "w" : "-");
            cout << ((file_info.st_mode & S_IXGRP) ? "x" : "-");
            cout << ((file_info.st_mode & S_IROTH) ? "r" : "-");
            cout << ((file_info.st_mode & S_IWOTH) ? "w" : "-");
            cout << ((file_info.st_mode & S_IXOTH) ? "x" : "-");
            cout << " " << file_info.st_nlink << " ";

            struct passwd *pwd = getpwuid(file_info.st_uid);
            if (pwd != nullptr) {
                cout << pwd->pw_name << " ";
            } else {
                cout << file_info.st_uid << " ";
            }

            struct group *grp = getgrgid(file_info.st_gid);
            if (grp != nullptr) {
                cout << grp->gr_name << " ";
            } else {
                cout << file_info.st_gid << " ";
            }

            cout << file_info.st_size << " ";

            char time_buffer[80];
            strftime(time_buffer, 80, "%b %d %H:%M", localtime(&file_info.st_mtime));
            cout << time_buffer << " ";
        }

        cout << entry->d_name << (long_format ? "\n" : " ");
    }
  cout << endl;
    closedir(dir);
}

void parse_flags(const string &flag_string, bool &hidden_files, bool &long_format) {
    for (char flag : flag_string) {
        if (flag == 'a') hidden_files = true;
        if (flag == 'l') long_format = true;
    }
}

void ls_command(vector<string> &args) {
    bool hidden_files = false;
    bool long_format = false;
    vector<string> directories;

    for (size_t i = 0; i < args.size(); ++i) {
        string &arg = args[i];

        if (arg == "-a") {
            hidden_files = true;
        } else if (arg == "-l") {
            long_format = true;
        } else if (arg.find("-") == 0) {  
            parse_flags(arg.substr(1), hidden_files, long_format);
        } else if (arg == "~") {
            arg = getenv("HOME"); 
            directories.push_back(arg);
        } else if (arg == ".") {
            directories.push_back("."); 
        } else if (arg == "..") {
            directories.push_back(".."); 
        }
         else { 
            directories.push_back(arg);
        }
    }
  
    if (directories.empty()) {
        directories.push_back("."); 
    }
    
    for (string &directory : directories) {
         cout << directory << ":" << endl;
        ls(directory, hidden_files, long_format);
    }
}






