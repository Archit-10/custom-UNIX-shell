#ifndef HEADER
#define HEADER

#include <iostream>
#include <vector>
#include <string>

using namespace std;

const int max_commands = 20;

void displayPrompt();
bool cd(const std::string &path);
void pwd();
void echo(const std::string &text);
void ls(const std::string &path, bool hidden_files, bool long_format);
void foreground(std::string &cmd);
void background(std::string &cmd);
void pinfo(int pid);
void execute_external_command(const std::string &cmd);
bool search(const std::string &present_dir, const std::string &name);
bool search(const std::string &name);
void execute_command_with_redirection(std::string &cmd);
void execute_pipeline(std::string &cmd);
void execute_command_with_redirection(const std::string &command);
void execute_pipeline(std::string cmd);
void sigint_handler(int signo);
void sigtstp_handler(int signo);
void handle_eof();
void execute_command_with_redirection(const std::string &command);
vector<string> get_matching_files(const std::string &prefix);
void autocomplete(string &command);
void handle_eof();
void load_history(vector<string> &history);
void save_history(const vector<string> &history);
void display_history(const vector<string> &history, int num = max_commands);
void add_command_to_history(vector<string> &history, const string &command);
void handle_exit();
void load_history(vector<string> &history);
void save_history(const vector<string> &history);
void add_command_to_history(vector<string> &history, const string &command);
void handle_exit();
void configure_terminal();
char getch();
void read_command_with_arrows(vector<string> &history);

#endif
