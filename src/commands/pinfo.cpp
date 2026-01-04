#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>

using namespace std;


void pinfo(int pid) {
   
    stringstream stat_path;
    stat_path << "/proc/" << pid << "/stat";

    ifstream stat_file(stat_path.str().c_str());
    if (!stat_file) {
        perror("pinfo");
        return;
    }
   
    string s;
    getline(stat_file,s);
    istringstream iss(s);

    int process_pid;
    int parent_pid;
    string process_name;
    char process_status;

    iss >> process_pid >> process_name >> process_status >> parent_pid;

    stat_file.close();

    string status_str;
    if (process_status == 'R' || process_status == 'S') {
        status_str = process_status;
        if (pid == getpid()) {
            status_str += "+";  
        }
    } else if (process_status == 'Z') {
        status_str = "Zombie";
    } else if (process_status == 'T') {
        status_str = "Stopped (on a signal)";
    } else {
        status_str = "Unknown"; 
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
   
    stringstream exe_path;
    exe_path << "/proc/" << pid << "/exe";

    char exe_buffer[1024];
    ssize_t exe_len = readlink(exe_path.str().c_str(), exe_buffer, sizeof(exe_buffer) - 1);
    if (exe_len != -1) {
        exe_buffer[exe_len] = '\0';
    } else {
        perror("pinfo");
    }

    cout << "pid -- " << process_pid << endl;
    cout << "Process Status -- {" << status_str << "}" << endl;
    cout << "memory -- ";  
    cout << "Executable Path -- " << exe_buffer << endl;
}





