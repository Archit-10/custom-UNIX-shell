#include <iostream>
#include <string>
#include <vector>
#include <dirent.h>
#include <cstring>
#include <unistd.h>
#include <signal.h>

using namespace std;

vector<string> get_matching_files(const string &prefix)
{
    vector<string> matching_files;
    DIR *dir = opendir(".");

    if (!dir)
    {
        perror("opendir");
        return matching_files;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr)
    {
        string name = entry->d_name;
        if (name.find(prefix) == 0)
        {
            matching_files.push_back(name);
        }
    }

    closedir(dir);
    return matching_files;
}

void autocomplete(string &command)
{

    size_t last_space = command.find_last_of(' ');
    string prefix = (last_space == string::npos) ? command : command.substr(last_space + 1);

    vector<string> matches = get_matching_files(prefix);

    if (matches.empty())
    {
        return;
    }
    else if (matches.size() == 1)
    {

        command = command.substr(0, last_space + 1) + matches[0];
        cout << "\r" << command << " ";
    }
    else
    {

        cout << "\n";
        for (const string &match : matches)
        {
            cout << match << " ";
        }
        cout << endl;
        cout << "\r" << command;
    }
}

void handle_eof()
{
    cout << "\nExiting shell." << endl;
    exit(0);
}
