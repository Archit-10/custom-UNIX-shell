#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <signal.h>
#include <termios.h>
#include <fcntl.h>

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

void configure_terminal()
{
    struct termios newt;
    tcgetattr(STDIN_FILENO, &newt);
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

char getch()
{
    char ch;
    read(STDIN_FILENO, &ch, 1);
    return ch;
}

void read_command_with_arrows(vector<string> &history)
{
    int index = history.size();
    string current_command;

    while (true)
    {

        cout << "\r" << string(80, ' ') << "\r";
        cout << "<Name@UBUNTU:~> " << current_command;
        cout.flush();

        char ch = getch();

        if (ch == 27)
        {
            char next_ch = getch();
            if (next_ch == '[')
            {
                char arrow = getch();
                if (arrow == 'A')
                {
                    if (index > 0)
                    {
                        index--;
                        current_command = history[index];
                    }
                }
                else if (arrow == 'B')
                {
                    if (index < history.size())
                    {
                        index++;
                        current_command = (index < history.size()) ? history[index] : "";
                    }
                }
            }
        }
        else if (ch == '\n')
        {
            cout << endl;
            if (!current_command.empty())
            {
                add_command_to_history(history, current_command);
                cout << "Executing command: " << current_command << endl;
            }
            break;
        }
        else
        {
            current_command += ch;
        }
    }
}
