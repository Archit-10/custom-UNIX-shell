#include <iostream>
#include <sstream>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>
#include <csignal>

using namespace std;

pid_t foreground_pid = -1;
void sigint_handler(int signo)
{
    if (foreground_pid != -1)
    {
        kill(foreground_pid, SIGINT);
        cout << "Process " << foreground_pid << " interrupted." << endl;
    }
}

void sigtstp_handler(int signo)
{
    if (foreground_pid != -1)
    {
        kill(foreground_pid, SIGTSTP);
        cout << "Process " << foreground_pid << " stopped." << endl;
    }
}

void handle_eof()
{
    cout << "\nExiting shell." << endl;
    exit(0);
}

void execute_command_with_redirection(const string &command)
{
    istringstream iss(command);
    string cmd;
    vector<char *> args;

    while (iss >> cmd)
    {

        if (cmd == ">")
        {
            string output_file;
            iss >> output_file;
            int fd = open(output_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
            {
                perror("open");
                return;
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
            continue;
        }
        else if (cmd == ">>")
        {
            string output_file;
            iss >> output_file;
            int fd = open(output_file.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0)
            {
                perror("open");
                return;
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
            continue;
        }
        else if (cmd == "<")
        {
            string input_file;
            iss >> input_file;
            int fd = open(input_file.c_str(), O_RDONLY);
            if (fd < 0)
            {
                perror("open");
                return;
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
            continue;
        }
        args.push_back(strdup(cmd.c_str()));
    }
    args.push_back(nullptr);

    pid_t child_pid = fork();
    if (child_pid == 0)
    {
        execvp(args[0], args.data());
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    else
    {
        foreground_pid = child_pid;
        int status;
        waitpid(child_pid, &status, 0);
        foreground_pid = -1;
    }

    for (char *arg : args)
    {
        free(arg);
    }
}
