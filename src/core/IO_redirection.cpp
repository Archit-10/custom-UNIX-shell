#include <iostream>
#include <sstream>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <fstream>

using namespace std;

void execute_command_with_redirection(string cmd)
{
    istringstream iss(cmd);
    vector<string> args;
    string token;

    while (iss >> token)
    {
        args.push_back(token);
    }

    int input_fd = -1, output_fd = -1;
    vector<string> final_args;

    for (size_t i = 0; i < args.size(); ++i)
    {
        if (args[i] == ">")
        {

            if (i + 1 < args.size())
            {
                output_fd = open(args[i + 1].c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (output_fd < 0)
                {
                    perror("Error opening output file");
                    return;
                }
                i++;
            }
            else
            {
                cerr << "No output file specified" << endl;
                return;
            }
        }
        else if (args[i] == ">>")
        {

            if (i + 1 < args.size())
            {
                output_fd = open(args[i + 1].c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
                if (output_fd < 0)
                {
                    perror("Error opening output file");
                    return;
                }
                i++;
            }
            else
            {
                cerr << "No output file specified" << endl;
                return;
            }
        }
        else if (args[i] == "<")
        {

            if (i + 1 < args.size())
            {
                input_fd = open(args[i + 1].c_str(), O_RDONLY);
                if (input_fd < 0)
                {
                    perror("Error opening input file");
                    return;
                }
                i++;
            }
            else
            {
                cerr << "No input file specified" << endl;
                return;
            }
        }
        else
        {
            final_args.push_back(args[i]);
        }
    }

    pid_t child_pid = fork();

    if (child_pid == 0)
    {

        if (input_fd != -1)
        {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }

        if (output_fd != -1)
        {
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }

        vector<char *> c_args;
        for (const auto &arg : final_args)
        {
            c_args.push_back(strdup(arg.c_str()));
        }
        c_args.push_back(nullptr);

        if (execvp(c_args[0], c_args.data()) == -1)
        {
            perror("Error executing command");
            exit(EXIT_FAILURE);
        }
    }
    else if (child_pid > 0)
    {

        int status;
        waitpid(child_pid, &status, 0);
    }
    else
    {
        perror("fork");
    }
}
