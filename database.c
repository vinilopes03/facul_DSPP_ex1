/**************************************
    A simple database simulation in
    binary file
***************************************/

#include "database.h"
#include <stdlib.h>
#include <stdio.h>


/* Basic database struct */
typedef struct
{
    FILE *file;
    size_t entry_size;
    char[] data_file_path;
}database_t;

/*
    Instantiate a database
    @param array of char
    @param size_t variable;
    @return pointer for database_t
*/
database_t *instantiate_database(char[] data_file_path, size_t entry_size)
{
    database_t database = malloc(sizeof(database_t);
    database->entry_size = entry_size;
    database->file = fopen(data_file_path, "r+");
    if(!database->file) {
        rename(data_file_path ".tmp", data_file_path);
        database->file = fopen(data_file_path, "w+");
    }
    database->data_file_path = data_file_path;
}

/* Put a entry in database
    @param pointer for database_t
    @param a void pointer
    @return void;
    */
void create_entry(database_t *database, void *entry){
    fseek(database->file, 0L, SEEK_END);
    fwrite(entry, database->entry_size, 1, database->file);
}
/*
    Seek for an entry in database
    @param database_t pointer
    @param void pointer;
    @param a boolean pointer funtion (@param void pointer, @param void pointer);
    @return 1 if TRUE, 0 if FALSE
    */
bool seek_entry(database_t *database, void *entry_target, bool (*entry_comparator)(void*, void*)){
    rewind(database->file);
    void *entry = malloc(database->entry_size);
    bool found = false;
    while(!found && fread(entry, database->entry_size, 1, database->file) == database->entry_size)
        found = (*entry_comparator)(entry_target, entry)

    if(found) fseek(database->file, fteel(database->file) - database->entry_size);
    free(entry);
    return found;
}

/* Read a entry from database
    @param database_t pointer
    @param void pointer
    @param a boolean function (@param void pointer, @param void pointer)
    @return void
    */
void *read_entry(database_t *database, void *entry_target, bool (*entry_comparator)(void*, void*)){
    if(!seek_entry(database, entry_target, entry_comparator)) return NULL //Not found
    void *entry = malloc(database->entry_size);
    fread(entry, database->entry_size, 1, database->file);
    return entry;
}

/* Update a entry from database
    @param database_t pointer
    @param void pointer
    @param @param a boolean function (@param void pointer, @param void pointer)
    @param void pointer
    @return void
    */
void update_entry(database_t *database, void *entry_target, bool (*entry_comparator)(void*, void*), void *updated_entry){
    if(!seek_entry(database, entry_target, entry_comparator)) return NULL //Not found
    fwrite(updated_entry, database->entry_size, 1, database->file);
}

/* Read a entry from database
    @param database_t pointer
    @param void pointer
    @@param a boolean function (@param void pointer, @param void pointer)
    @return void
    */
void delete_entry(database_t *database, void *entry_target, bool (*entry_comparator)(void*, void*)){
    FILE tmp_data_file = fopen(database->data_file_path ".tmp", "w");
    rewind(database->file);
    void *entry = malloc(database->entry_size);
    bool found = false;
    while(fread(entry, database->entry_size, 1, database->file) == database->entry_size){
        if((*entry_comparator)(entry_target, entry)) found = true;
        else fwrite(entry, database->entry_size, 1, tmp_data_file);
    }
    free(entry);

    if(found) {
        remove(database->data_file_path);
        rename(database->data_file_path ".tmp", database->data_file_path);
    }

    return;
}
