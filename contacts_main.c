#include <stdio.h>
#include <string.h>

#include "contacts.h"

#define MAX_CMD_LEN 128

/*
 * This is in general *very* similar to the list_main file seen in lab 2
 * One big difference is the notion of switching between contact logs in one
 * run of the program.
 * You have to create or load a contact log from a file before you can do things
 * like add, lookup, or write.
 * The code below has to check if contact log is NULL before the operations
 * occur. Also, the user has to explicitly clear the current contact log before
 * they can create or load in a new one.
 */
int main(int argc, char **argv) {
    contacts_log_t *log = NULL;

    if (argc > 1) {
        const char *file_name = argv[1];
        int filename_len = strlen(file_name);
        const char *last_four = &file_name[filename_len-4];
        if (strcmp(last_four, ".txt") == 0){
            log = read_contacts_log_from_text(file_name);
        }
        else if (strcmp(last_four, ".bin") == 0){
            log = read_contacts_log_from_binary(file_name);
        }
        else {
            printf("Error: Unknown contacts log file extension\n");
        }
    }


    printf("CSCI 2021 Contact Log System\n");
    printf("Commands:\n");
    printf("  create <name>:            creates a new log with specified name\n");
    printf("  log:                      shows the name of the active contacts log\n");
    printf("  add <name> <phone> <zip>: adds a new contact\n");
    printf("  lookup <name>:            searches for a phone number by contact name\n");
    printf("  clear:                    resets current contacts log\n");
    printf("  print:                    shows all contacts in active log\n");
    printf("  write_text:               saves all contacts to text file\n");
    printf("  read_text <file_name>:    loads contacts from text file\n");
    printf("  write_bin:                saves all contacts to binary file\n");
    printf("  read_bin <file_name>:     loads contacts from binary file\n");
    printf("  exit:                     exits the program\n");

    char cmd[MAX_CMD_LEN];
    while (1) {
        printf("contacts> ");
        if (scanf("%s", cmd) == EOF) {
            printf("\n");
            break;
        }

        if (strcmp("exit", cmd) == 0) {
            break;
        }

        else if (strcmp("create", cmd) == 0) {
            scanf("%s", cmd); // Read in new log name
            if (log != NULL) {
                printf("Error: You already have an contacts log.\n");
                printf("You can remove it with the \'clear\' command\n");
            } else {
                log = create_contacts_log(cmd);
                if (log == NULL) {
                    printf("Contacts log creation failed\n");
                }
            }
        }

        // TODO Add cases for other commands
        // Read in the command and (possibly) additional arguments with scanf()
        else if (strcmp("log", cmd) == 0){
            if (log == NULL){
                printf("Error: You must create or load a contacts log first\n");
            }
            else{
                printf("%s\n", get_contacts_log_name(log));
            }
        }
        
        else if (strcmp("add", cmd) == 0){
            unsigned long phone_number;
            unsigned zip_code;
            char name[MAX_NAME_LEN];
            scanf("%s %ld %d", name, &phone_number, &zip_code);
            
            if (log == NULL){
                printf("Error: You must create or load a contacts log first\n");
            }
            else{
                add_contact(log, name, phone_number, zip_code);
            }
        }

        else if (strcmp("print", cmd) == 0){
            if (log == NULL){
                printf("Error: You must create or load a contacts log first\n");
            }
            else{
                print_contacts_log(log);
            }
        }

        else if (strcmp("lookup", cmd) == 0){
            scanf("%s", cmd);
            if (log == NULL){
                printf("Error: You must create or load a contacts log first\n");
            }
            else{
                if ((find_phone_number(log, cmd)) != -1){
                    printf("%s: %ld\n", cmd, find_phone_number(log, cmd));
                }
                else{
                    printf("No phone number for '%s' found\n", cmd);
                }
            }
        }

        else if (strcmp("clear", cmd) == 0){
            if (log == NULL){
                printf("Error: No contacts log to clear\n");
            }
            else{
                free_contacts_log(log);
                log = NULL;
            }
        }
        else if(strcmp("write_text", cmd) == 0){
            if (log == NULL){
                printf("Error: You must create or load a contacts log first\n");
            }
            else{
                if (write_contacts_log_to_text(log) == 0){
                    char file_name[MAX_NAME_LEN + strlen(".txt")];
                    strcpy(file_name, get_contacts_log_name(log));
                    strcat(file_name, ".txt");
                    printf("Contacts log successfully written to %s\n", file_name);
                }
            }
        }

        else if(strcmp("read_text", cmd) == 0){
            scanf("%s", cmd);
            if (log != NULL){
                printf("Error: You must clear current contacts log first\n");
            }
            else{
                log = read_contacts_log_from_text(cmd);
            }
        }

        else if(strcmp("write_bin", cmd) == 0){
            if (log == NULL){
                printf("Error: You must create or load a contacts log first\n");
            }
            else{
                write_contacts_log_to_binary(log);
                char file_name[MAX_NAME_LEN + strlen(".bin")];
                strcpy(file_name, get_contacts_log_name(log));
                strcat(file_name, ".bin");
                printf("Contacts log successfully written to %s\n", file_name);
            }
        }

        else if(strcmp("read_bin", cmd) == 0){
            scanf("%s", cmd);
            if (log != NULL){
                printf("Error: You must clear current contacts log first\n");
            }
            else{
                log = read_contacts_log_from_binary(cmd);
            }
        }

        else {
            printf("Unknown command %s\n", cmd);
        }
    }

    if (log != NULL) {
        free_contacts_log(log);
    }
    return 0;
}
