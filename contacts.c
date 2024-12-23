#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "contacts.h"

// This is the (somewhat famous) djb2 hash
unsigned hash(const char *str) {
    unsigned hash_val = 5381;
    int i = 0;
    while (str[i] != '\0') {
        hash_val = ((hash_val << 5) + hash_val) + str[i];
        i++;
    }
    return hash_val % NUM_BUCKETS;
}

contacts_log_t *create_contacts_log(const char *log_name) {
    contacts_log_t *new_log = malloc(sizeof(contacts_log_t));
    if (new_log == NULL) {
        return NULL;
    }

    strcpy(new_log->log_name, log_name);
    for (int i = 0; i < NUM_BUCKETS; i++) {
        new_log->buckets[i] = NULL;
    }
    new_log->size = 0;

    return new_log;
}

const char *get_contacts_log_name(const contacts_log_t *log) {
    // TODO Not yet implemented
    return log->log_name;
}

int add_contact(contacts_log_t *log, const char *name, unsigned long phone_number, unsigned zip_code) {
    // TODO Not yet implemented
    if (phone_number > 9999999999 || phone_number < 1000000000 || zip_code < 10000 || zip_code > 99999){
        // printf("phone: %lu \n zip: %u\n", phone_number, zip_code);
        printf("Error: Invalid phone number and/or zip code\n");
        return -1;
    }
    node_t *new_contact = malloc(sizeof(node_t));

    strcpy(new_contact->name, name);
    new_contact->phone_number = phone_number;
    new_contact->zip_code = zip_code;
    new_contact->next = NULL;

    if (log->buckets[hash(name)] == NULL){
        log->buckets[hash(name)] = new_contact;
    }

    //adds new_contact to the end of the linkedlist if a collision occurs
    else{
        node_t *curr = log->buckets[hash(name)];
        while (curr->next != NULL){
            curr = curr->next;
        }
        curr->next = new_contact;
    }
    log->size++;
    // printf("%s Successfully added\n", new_contact->name);
    return 0;
}

long find_phone_number(const contacts_log_t *log, const char *name) {
    // TODO Not yet implemented
    node_t *curr = log->buckets[hash(name)];
    while (curr != NULL){
        if (strcmp(curr->name, name) == 0){
            return curr->phone_number;
        }
        curr = curr->next;
    }
    return -1;
}

void print_contacts_log(const contacts_log_t *log) {
    // TODO Not yet implemented
    printf("All contacts in %s:\n", get_contacts_log_name(log));

    for (int i=0; i<NUM_BUCKETS; i++){
        node_t *curr = log->buckets[i];
        while (curr != NULL){
            printf("Name: %s, Phone Number: %ld, Zip Code: %d\n", curr->name, curr->phone_number, curr->zip_code);
            curr = curr->next;
        }
    }
}

void free_contacts_log(contacts_log_t *log) {
    // TODO Not yet implemented
    for (int i=0; i<NUM_BUCKETS; i++){
        node_t *curr = log->buckets[i];
        while (curr != NULL){
            node_t *next = curr->next;
            free(curr);
            curr = next;
        }
    }
    free(log);
}

int write_contacts_log_to_text(const contacts_log_t *log) {
    char file_name[MAX_NAME_LEN + strlen(".txt")];
    strcpy(file_name, log->log_name);
    strcat(file_name, ".txt");
    FILE *f = fopen(file_name, "w");
    if (f == NULL) {
        return -1;
    }

    fprintf(f, "%u\n", log->size);
    for (int i = 0; i < NUM_BUCKETS; i++) {
        node_t *current = log->buckets[i];
        while (current != NULL) {
            fprintf(f, "%s %lu %u\n", current->name, current->phone_number, current->zip_code);
            current = current->next;
        }
    }
    fclose(f);
    return 0;
}



contacts_log_t *read_contacts_log_from_text(const char *file_name) {
    // TODO Not yet implemented
    FILE *file_handle = fopen(file_name, "r");
    if (file_handle == NULL){
        printf("Failed to read contacts log from text file\n");
        return NULL;
    }

    char log_name[MAX_NAME_LEN];
    strncpy(log_name, file_name, strlen(file_name)-4);
    log_name[strlen(file_name)-4] = '\0';

    char name[MAX_NAME_LEN];
    unsigned long phone_number;
    unsigned zip_code;

    contacts_log_t *log = create_contacts_log(log_name);

    fscanf(file_handle, "%u", &log->size);
    
    for (int i = 0; i < log->size; i++){
        if (fscanf(file_handle, "%s %lu %u", name, &phone_number, &zip_code) == EOF){
            break;
        }
        add_contact(log, name, phone_number, zip_code);
    }
    
    fclose(file_handle);
    printf("Contacts log loaded from text file\n");
    return log;
}

int write_contacts_log_to_binary(const contacts_log_t *log) {
    // TODO Not yet implemented
    char file_name[MAX_NAME_LEN + strlen(".bin")];
    strcpy(file_name, log->log_name);
    strcat(file_name, ".bin");
    FILE *file_handle = fopen(file_name, "wb");
    if (file_handle == NULL) {
        return -1;
    }

    fwrite(&log->size, sizeof(log->size), 1, file_handle);
     for (int i = 0; i < (NUM_BUCKETS); i++) {
        node_t *curr = log->buckets[i];
        while (curr != NULL) {
            unsigned int name_length = strlen(curr->name);
            fwrite(&name_length, sizeof(unsigned int), 1, file_handle);
            fwrite(curr->name, sizeof(char), name_length, file_handle);
            fwrite(&curr->phone_number, sizeof(unsigned long), 1, file_handle);
            fwrite(&curr->zip_code, sizeof(unsigned int), 1, file_handle);
            // printf("name_Length %u Name %s Phone %lu zip %u", name_length, curr->name, curr->phone_number, curr->zip_code);        
            curr = curr->next;
        }
    }
    fclose(file_handle);
    return 0;
}

contacts_log_t *read_contacts_log_from_binary(const char *file_name) {
    // TODO Not yet implemented
    FILE *file_handle = fopen(file_name, "rb");
    if (file_handle == NULL){
        printf("Failed to read contacts log from binary file\n");
        return NULL;
    }

    char log_name[MAX_NAME_LEN];
    strncpy(log_name, file_name, strlen(file_name)-4);
    log_name[strlen(file_name)-4] = '\0';

    contacts_log_t *log = create_contacts_log(log_name);

    unsigned int name_length;
    unsigned long phone_number;
    unsigned zip_code;
    fread(&log->size, sizeof(unsigned int), 1, file_handle);
    unsigned int upper_limit = log->size;
    // printf("THIS IS THE LOG SIZE (UPPER BOUND FOR LOOP)%d\n", log->size);
    for (int i=0; i<(upper_limit); i++){
        // printf("THIS IS THE LOG SIZE (UPPER BOUND FOR LOOP)%d\n", log->size);

        fread(&name_length, sizeof(unsigned int), 1, file_handle);
        char *name = malloc(name_length+1);
        fread(name, sizeof(char), name_length, file_handle);
        name[name_length] = '\0';
        fread(&phone_number, sizeof(unsigned long), 1, file_handle);
        fread(&zip_code, sizeof(unsigned int), 1, file_handle);
        // printf("Reading contact: %s, phone: %lu, zip: %u\n", name, phone_number, zip_code);

        add_contact(log, name, phone_number, zip_code);
        free(name);
    }

    fclose(file_handle);
    printf("Contacts log loaded from binary file\n");

    return log;
}
