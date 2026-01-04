#include <iostream>
#include <sstream>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>

using namespace std;

void execute_pipeline(string cmd)
{
    istringstream iss(cmd);
    vector<string> commands;
    string command;

    while (getline(iss, command, '|'))
    {
        commands.push_back(command);
    }

    int pipefds[2 * (commands.size() - 1)];

    for (size_t i = 0; i < commands.size() - 1; i++)
    {
        if (pipe(pipefds + i * 2) == -1)
        {
            perror("pipe");
            return;
        }
    }

    for (size_t i = 0; i < commands.size(); i++)
    {
        pid_t child_pid = fork();

        if (child_pid == 0)
        {

            if (i > 0)
            {
                dup2(pipefds[(i - 1) * 2], STDIN_FILENO);
            }
            if (i < commands.size() - 1)
            {
                dup2(pipefds[i * 2 + 1], STDOUT_FILENO);
            }

            for (size_t j = 0; j < (commands.size() - 1) * 2; j++)
            {
                close(pipefds[j]);
            }

            istringstream command_stream(commands[i]);
            vector<char *> args;
            string token;
            while (command_stream >> token)
            {
                args.push_back(strdup(token.c_str()));
            }
            args.push_back(nullptr);

            if (execvp(args[0], args.data()) == -1)
            {
                perror("Error executing command");
                exit(EXIT_FAILURE);
            }
        }
        else if (child_pid < 0)
        {
            perror("fork");
            return;
        }
    }

    for (size_t i = 0; i < (commands.size() - 1) * 2; i++)
    {
        close(pipefds[i]);
    }

    for (size_t i = 0; i < commands.size(); i++)
    {
        wait(nullptr);
    }
}
