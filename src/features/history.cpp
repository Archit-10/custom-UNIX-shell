#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <signal.h>

using namespace std;

const string history_file = "history.txt";
const int max_commands = 20;

void load_history(vector<string> &history)
{
    ifstream file(history_file);
    string line;
    while (getline(file, line) && history.size() < max_commands)
    {
        history.push_back(line);
    }
}

void save_history(const vector<string> &history)
{
    ofstream file(history_file);
    for (const string &command : history)
    {
        file << command << endl;
    }
}

void display_history(const vector<string> &history, int num = max_commands)
{
    int start = max(0, (int)history.size() - num);
    for (int i = start; i < history.size(); ++i)
    {
        cout << history[i] << endl;
    }
}

void add_command_to_history(vector<string> &history, const string &command)
{
    if (history.size() >= max_commands)
    {
        history.erase(history.begin());
    }
    history.push_back(command);
    save_history(history);
}

void handle_exit()
{
    cout << "\nExiting shell." << endl;
    exit(0);
}
