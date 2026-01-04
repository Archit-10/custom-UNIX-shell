#include <iostream>
#include <sstream>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fstream>
#include <cstring>

using namespace std;

void foreground(string &cmd) {
    pid_t child_pid = fork();

if (child_pid == -1) {
        perror("Error: fork");
        return;
    }
    
if (child_pid == 0) {
        vector<char*> v;
        istringstream iss(cmd);
        string s;  
        while (iss >> s) {
            v.push_back(strdup(s.c_str()));
        }
        v.push_back(nullptr);

        if (execvp(v[0], v.data()) == -1) {
            perror("Error: execvp");
            for (auto arg : v) free(arg);
            exit(EXIT_FAILURE);
        }
    for (auto arg : v) free(arg);
    } else {
        int value;
        waitpid(child_pid, &value, 0);
    }
}
void background(string &cmd) {
    pid_t child_pid = fork();

    if (child_pid == -1) {
        perror("fork");
        return;
    }

    if (child_pid == 0) {In background:

cpp

for (auto arg : v) free(arg); 
      
        vector<char*> v;
        istringstream iss(cmd);
        string s;

        while (iss >> s) {
            v.push_back(strdup(s.c_str()));
        }
        v.push_back(nullptr);

        if (execvp(v[0], v.data()) == -1) {
            perror("execvp");
              for (auto arg : v) {
                free(arg);
            }
            exit(EXIT_FAILURE);
        }
    } else {
           cout << "Background PID: " << child_pid << endl;
    }
}

void pinfo(int pid) {
  
    stringstream stat_path;
    stat_path << "/proc/" << pid << "/stat";

    ifstream stat_file(stat_path.str());
    if (!stat_file) {
        perror("pinfo");
        return;
    }

    string line;
    getline(stat_file, line);
    istringstream iss(line);

    int process_pid;
    string process_name;
    char process_status;
    int parent_pid;

    iss >> process_pid >> process_name >> process_status >> parent_pid;

    stat_file.close();

    stringstream exe_path;
    exe_path << "/proc/" << pid << "/exe";

    char exe_buffer[1024];
    ssize_t exe_len = readlink(exe_path.str().c_str(), exe_buffer, sizeof(exe_buffer) - 1);
    if (exe_len != -1) {
        exe_buffer[exe_len] = '\0';
    } else {
        perror("pinfo");
    }

      stringstream mem_path;
    mem_path << "/proc/" << pid << "/statm";

    ifstream mem_file(mem_path.str());
    if (!mem_file) {
        perror("pinfo memory");
        return;
    }

    unsigned long size, resident, shared, text, lib, data, dt;
    mem_file >> size >> resident >> shared >> text >> lib >> data >> dt;
    mem_file.close();

    const unsigned long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024;

    cout << "pid -- " << process_pid << endl;
    cout << "Process Status -- ";
    if (process_status == 'R' || process_status == 'S') {
        cout << process_status;
        if (pid == getpid()) {
            cout << "+";  
        }
    } else {
        cout << process_status;
    }
    cout << endl;
    cout << "memory -- ";  
    cout << "Executable Path -- " << exe_buffer << endl;
}


void execute_external_command(const string &cmd) {
    pid_t child_pid = fork();

    if (child_pid == -1) {
        perror("fork");
        return;
    }

    if (child_pid == 0) {
        vector<char*> v;
        istringstream iss(cmd);

        string arg;
        while (iss >> arg) {
            v.push_back(strdup(arg.c_str()));
        }
        v.push_back(nullptr);

        if (execvp(v[0], v.data()) == -1) {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    } else {
        int status;
        waitpid(child_pid, &status, 0);
    }
}















