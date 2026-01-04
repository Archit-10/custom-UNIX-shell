#include <iostream>
#include <unistd.h>
#include <cstdlib> 
#include <cstring>

using namespace std;

bool cd(const string &path) {
    if (path.empty()) {
        const char *home_dir = getenv("HOME");
        if (home_dir == nullptr) {
            cerr << "Error: HOME variable is not set" << endl;
            return false;
        }
        if (chdir(home_dir) != 0) {
            perror("chdir");
            return false;
        }
    } else {
        if (path == "~") {
            const char *home_dir = getenv("HOME");
            if (home_dir == nullptr) {
                cerr << "Error: HOME variable is not set." << endl;
                return false;
            }
            if (chdir(home_dir) != 0) {
                perror("chdir");
                return false;
            }
        } else if (path == ".") {
            
        } else if (path == "..") {
            if (chdir("..") != 0) {
                perror("chdir");
                return false;
            }
        } else if (path == "-") {
            const char *previous_dir = getenv("OLDPWD");
            if (previous_dir == nullptr) {
                cerr << "Error: OLDPWD variable is not set." << endl;
                return false;
            }
            if (chdir(previous_dir) != 0) {
                perror("chdir");
                return false;
            }
        } else {
            if (chdir(path.c_str()) != 0) {
                perror("chdir");
                return false;
            }
        }
    }
    setenv("OLDPWD", current_directory, 1);
    return true;
}

void pwd() {
    const int BUFFER_SIZE = 8192;
    char current_directory[BUFFER_SIZE];
    if (getcwd(current_directory, BUFFER_SIZE) != nullptr) {
        cout << current_directory << endl;
    } else {
        perror("getcwd");
    }
}

void echo(const string &text) {
    cout << text << endl;
}



