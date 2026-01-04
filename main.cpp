#include "header_files.h"

int main()
{
    // Testing displayPrompt()
    while (true)
    {
        displayPrompt();

        string command;
        getline(cin, command);

        // Exit condition for testing
        if (command == "exit")
        {
            break; // Exit the loop
        }

        // For now, just print the entered command
        cout << "You entered: " << command << endl;
    }
    return 0; // Return success
}
