#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

// Plugin information structure
typedef struct {
    char name[64];
    char description[256];
    char version[16];
} plugin_info_t;

// Plugin information
plugin_info_t plugin_info = {
    "Quick Terminal Integration",
    "Open an embedded terminal inside Mohavim to run scripts or tests without leaving the editor",
    "1.0.0"
};

// Function to execute a command in a subprocess
int execute_command(const char* command) {
    pid_t pid = fork();
    
    if (pid == 0) {
        // Child process
        execl("/bin/sh", "sh", "-c", command, NULL);
        perror("Failed to execute command");
        exit(1);
    } else if (pid > 0) {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status);
    } else {
        // Fork failed
        perror("Failed to fork process");
        return -1;
    }
}

// Plugin initialization function
void plugin_init() {
    printf("Quick Terminal Integration plugin loaded\n");
}

// Plugin cleanup function
void plugin_cleanup() {
    printf("Quick Terminal Integration plugin unloaded\n");
}

// Main plugin function to open terminal
void open_terminal() {
    printf("Quick Terminal Integration - Embedded Terminal\n");
    printf("Type 'exit' to return to Mohavim\n");
    printf("---------------------------------------------\n");
    
    char command[256];
    while (1) {
        printf("$ ");
        fflush(stdout);
        
        if (fgets(command, sizeof(command), stdin) == NULL) {
            break;
        }
        
        // Remove newline character
        command[strcspn(command, "\n")] = 0;
        
        // Check for exit command
        if (strcmp(command, "exit") == 0) {
            break;
        }
        
        // Execute command
        if (strlen(command) > 0) {
            int result = execute_command(command);
            if (result != 0) {
                printf("Command exited with status: %d\n", result);
            }
        }
    }
    
    printf("Returning to Mohavim...\n");
}

// Plugin API functions
void plugin_run() {
    open_terminal();
}

// Export plugin information
plugin_info_t* get_plugin_info() {
    return &plugin_info;
}